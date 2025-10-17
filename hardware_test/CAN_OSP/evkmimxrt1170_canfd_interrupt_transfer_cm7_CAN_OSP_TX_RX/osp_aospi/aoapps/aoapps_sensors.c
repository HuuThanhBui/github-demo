// aoapps_sensors.cpp - the "sensors" app controls the SAIDsense board, with temperature (AS6212), light (SFH5721), and rotary (AS5600) sensors
/*****************************************************************************
 * Copyright 2025 by ams OSRAM AG                                            *
 * All rights are reserved.                                                  *
 *                                                                           *
 * IMPORTANT - PLEASE READ CAREFULLY BEFORE COPYING, INSTALLING OR USING     *
 * THE SOFTWARE.                                                             *
 *                                                                           *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       *
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT         *
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS         *
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT  *
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,     *
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT          *
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     *
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY     *
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT       *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE     *
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.      *
 *****************************************************************************/
// #include <Arduino.h>            // PRINTF
#include <aoresult.h>           // AORESULT_ASSERT, aoresult_t
#include <aoosp.h>              // aoosp_send_clrerror()
#include <aocmd.h>              // aocmd_cint_isprefix()
//#include <aoui32.h>             // aoui32_but_wentdown()
#include <aomw.h>               // aomw_topo_build_start()
#include <aoapps.h>             // aoapps_mngr_register()


/*
SENSORS - This is one of the stock applications

DESCRIPTION
- Measures the temperature using an AS6212 temperature sensor.
- Measures the angle of the knob using an AS5600 rotary sensor.
- Measures the light intensity using an SFH5721 ambient light sensor.
- Only one of the three sensors is active at one time.
- To change the active sensor, either press the X button to cycle to the next,
  or press one of the selector buttons TEMP, ROTARY, ALS (and EXT) on 
  the SAIDsense board.
- The indicator LED (paired with the selector button) of the active 
  sensor switches on.
- The quad 7-segment display shows the readout of the active sensor.
- When the temperature sensor is active, the OSP chains shows a red part
  and a blue part that change with temperature.
- When the angle sensor is active, the knob angle (0-360) determines how 
  many triplets are yellow (0..num triplets).
- When ambient light sensor is active, the higher the ambient light level,
  the greener the triplets.
- This app is written for the SAIDsense board.
- For backward compatibility with older board, this app supports a board 
  without light sensor, rotary sensor, or selector.

BUTTONS
- The X button toggles between temperature, angle or light sensor being active.
  - Upon a press the display shows the units for a short moment.
  - Active sensor has its indicator LED on.
- The Y button scrolls the type number of the sensor.

NOTES
- When the app quits, the display and indicator LEDs switches off.

GOAL
- To show a sensor can be used in OSP (e.g. for climate control, light adaption).
*/


// === Animation state machine ===============================================


// Which sensor is active (stored in aoapps_sensors_sseg_mode)
#define AOAPPS_SENSORS_SSEG_MODE_TEMP        1    // temperature               
#define AOAPPS_SENSORS_SSEG_MODE_LIGHT       2    // light level
#define AOAPPS_SENSORS_SSEG_MODE_ANGLE       3    // knob angle
static int      aoapps_sensors_sseg_mode;         // see AOAPPS_SENSORS_SSEG_MODE_XXX


// For backward compatibility, I2C devices might not be present
static int      aoapps_sensors_temp_present;      // the temperature sensor presence (if not, use a SAID)
static int      aoapps_sensors_angle_present;     // the rotary sensor presence
static int      aoapps_sensors_light_present;     // the light sensor presence
static int      aoapps_sensors_sseg_present;      // the quad 7-segment display presence
static int      aoapps_sensors_selector_present;  // the selector (4 buttons, 4 indicator LEDs) presence


// Readings from the sensors                                                           
static double   aoapps_sensors_temp_current;      // last measured temperature (in degree C)
static double   aoapps_sensors_angle_current;     // the last measured angle (deg 360) of the knob
static double   aoapps_sensors_light_current;     // last measured light level (lux)


// Managing display updates
#define  AOAPPS_SENSORS_ANIM_MS             50    // time between sensor updates
#define  AOAPPS_SENSORS_HOLD_MS           1500    // time to hold (do not update) display (show units on display)
#define  AOAPPS_SENSORS_SCROLLMS           200    // ms between text scrolls
static uint32_t aoapps_sensors_lastms;            // last time stamp (in ms) a flag was shown (for auto change)
static uint32_t aoapps_sensors_holdms;            // start of display hold, to temporarily show sensor name on display
static const char * aoapps_sensors_scrollptr;     // pointer to string to scroll on display
static uint32_t aoapps_sensors_scrollms;          // last time stamp (in ms) text was scrolled


// Updates aoapps_sensors_temp_current with temp sensor measurement (if sensor absent, use SAID)
#define  AOAPPS_SENSORS_SAID_ADDR        0x001    // SAID to use when no temperature sensor found
static aoresult_t aoapps_sensors_temp_measure() {
  aoresult_t result;
  if( aoapps_sensors_temp_present ) {
    int millicelsius;
    for( int retries=1; retries<5; retries++ ) {
      result= aomw_as6212_temp_get(&millicelsius);
      if( result==aoresult_ok ) break;
      delay(10);
    }
    if( result!=aoresult_ok ) return result;
    aoapps_sensors_temp_current= millicelsius/1000.0;
  } else {
    uint8_t tempraw;
    result= aoosp_send_readtemp(AOAPPS_SENSORS_SAID_ADDR, &tempraw);
    if( result!=aoresult_ok ) return result;
    aoapps_sensors_temp_current= aoosp_prt_temp_said(tempraw);
  }
  return aoresult_ok;
}


// Updates aoapps_sensors_angle_current with rotary sensor measurement (if sensor present)
static aoresult_t aoapps_sensors_angle_measure() {
  aoresult_t result;
  if( aoapps_sensors_angle_present ) {
    int angle;
    result= aomw_as5600_angle_get(&angle); 
    if( result!=aoresult_ok ) return result;
    // Do not want 0.0 as well as 360.0 in range, so divide by MAX+1
    aoapps_sensors_angle_current = angle*360.0/(AOMW_AS5600_ANGLE_MAX+1);
  }
  return aoresult_ok;
}


// Updates aoapps_sensors_light_current with light measurement (if light sensor present)
static aoresult_t aoapps_sensors_light_measure() {
  aoresult_t result;
  if( aoapps_sensors_light_present ) {
    int als;
    result= aomw_sfh5721_als_get(&als); 
    if( result!=aoresult_ok ) return result;
    // SFH5721 datasheet: lux=512*als/(DGAIN*AGAIN*2^IT)
    // We have DGAIN=1, AGAIN=4, IT=7(=25ms), so lux=als
    aoapps_sensors_light_current = als;
  }
  return aoresult_ok;
}


// Writes sensor value to the display depending on aoapps_sensors_sseg_mode
static aoresult_t aoapps_sensors_display() {
  if( ! aoapps_sensors_sseg_present ) return aoresult_ok;
  if( millis()-aoapps_sensors_holdms<AOAPPS_SENSORS_HOLD_MS ) return aoresult_ok;

  aoresult_t result;
  
  if( *aoapps_sensors_scrollptr != 0 ) {
    if( millis()-aoapps_sensors_scrollms<AOAPPS_SENSORS_SCROLLMS ) return aoresult_ok;
    result= aomw_sseg_printf(aoapps_sensors_scrollptr);
    if( result!=aoresult_ok ) return result;
    aoapps_sensors_scrollptr++;
    aoapps_sensors_scrollms= millis();
    return aoresult_ok;
  }

  switch( aoapps_sensors_sseg_mode ) {
    case AOAPPS_SENSORS_SSEG_MODE_TEMP:
      //PRINTF("temp '%5.1f'\n",aoapps_sensors_temp_current);
      result= aomw_sseg_printf("%5.1f",aoapps_sensors_temp_current); // deg C
      if( result!=aoresult_ok ) return result;
    break;
    case AOAPPS_SENSORS_SSEG_MODE_ANGLE:
      //PRINTF("angle '%5.1f'\n",aoapps_sensors_angle_current);
      result= aomw_sseg_printf("%5.1f",aoapps_sensors_angle_current); // deg 360
      if( result!=aoresult_ok ) return result;
    break;
    case AOAPPS_SENSORS_SSEG_MODE_LIGHT:
      //PRINTF("light '%4.0f'\n",aoapps_sensors_light_current);
      result= aomw_sseg_printf("%4.0f",aoapps_sensors_light_current); // lux
      if( result!=aoresult_ok ) return result;
    break;
    default:
      result= aomw_sseg_printf("????");
      if( result!=aoresult_ok ) return result;
    break;
  }

  return aoresult_ok;
}


// === Color triplets ========================================================


// OLD
// // Maps delta between temp_current and temp_average to chain as a red/blue color
// static double   aoapps_sensors_temp_average;      // the average temperature (over long period; baseline)
// static aoresult_t aoapps_sensors_colortriplets_temp() {
//   // Update average
//   #define  AOAPPS_SENSORS_FILTER_WEIGHT     0.980   // Weight for "old" temperature baseline
//   aoapps_sensors_temp_average=
//     ((    AOAPPS_SENSORS_FILTER_WEIGHT) * aoapps_sensors_temp_average  +
//      (1.0-AOAPPS_SENSORS_FILTER_WEIGHT) * aoapps_sensors_temp_current);
// 
//   // What is the delta we need to map to the OSP chain?
//   float t_delta = aoapps_sensors_temp_current - aoapps_sensors_temp_average;
//   // Compute color change on scale 
//   #define TMAX 1.5 // This delta in temperature (deg C) would map the color from mid to either max or min
//   float    r1 = (0.5 + t_delta/TMAX)*AOMW_TOPO_BRIGHTNESS_MAX;
//   float    b1 = (0.5 - t_delta/TMAX)*AOMW_TOPO_BRIGHTNESS_MAX;
//   // Setup RGB color
//   uint16_t r2 = constrain(r1,0,AOMW_TOPO_BRIGHTNESS_MAX);
//   uint16_t b2 = constrain(b1,0,AOMW_TOPO_BRIGHTNESS_MAX);
//   aomw_topo_rgb_t col = { r2,0,b2, "blue..red" }; 
//   // Distribute over the whole chain
//   for( int tix=0; tix<aomw_topo_numtriplets(); tix++ ) {
//     aoresult_t result= aomw_topo_settriplet( tix, &col ); 
//     if( result!=aoresult_ok ) return result;
//   }
//   return aoresult_ok;
// }


// Maps delta between temp_current and temp_average to chain as a red/blue color
static double aoapps_sensors_temp_average;      // the average temperature (over long period; baseline)
static aoresult_t aoapps_sensors_colortriplets_temp() {
  // Update average
  #define  AOAPPS_SENSORS_FILTER_WEIGHT     0.98   // Weight for "old" temperature baseline
  aoapps_sensors_temp_average=
    ((    AOAPPS_SENSORS_FILTER_WEIGHT) * aoapps_sensors_temp_average  +
     (1.0-AOAPPS_SENSORS_FILTER_WEIGHT) * aoapps_sensors_temp_current);
  // Establish temp boundaries (asymmetrical because making warm is easier) 
  double tempmin = aoapps_sensors_temp_average - 2.0;
  double tempmax = aoapps_sensors_temp_average + 3.0;
  // Map current temperature to point on OSP chain (map has 'long' as type of args, so *100)
  int midtix = map( aoapps_sensors_temp_current*100, tempmax*100, tempmin*100, 0 , aomw_topo_numtriplets() );
  //PRINTF("c=%.1f a=%.1f [%.1f,%.1f]  --  ", aoapps_sensors_temp_current, aoapps_sensors_temp_average, tempmin, tempmax );
  //PRINTF("%d %d %d\n",0,midtix,aomw_topo_numtriplets() );
  // render yellow bar
  for( int tix=0; tix<aomw_topo_numtriplets(); tix++ ) {
    aomw_topo_rgb_t col = tix<=midtix ? aomw_topo_blue : aomw_topo_red;
    aoresult_t result= aomw_topo_settriplet( tix, &col ); 
    if( result!=aoresult_ok ) return result;
  }
  return aoresult_ok;
}


// Maps light level to a brightness on green
static aoresult_t aoapps_sensors_colortriplets_light() {
  int green1 = AOMW_TOPO_BRIGHTNESS_MAX*aoapps_sensors_light_current/100;
  // Add a 2500 offset and a 4x gain, for clearer effect
  int green2 = max(0,min(4*(green1-2000),AOMW_TOPO_BRIGHTNESS_MAX)); 
  //PRINTF("%.2f %d %d\n",aoapps_sensors_light_current,green1,green2);
  // Compose the color
  aomw_topo_rgb_t col = { 0x0000, uint16_t(green2), 0x0000, "autogreen" }; 
  // Distribute over the whole chain
  for( int tix=0; tix<aomw_topo_numtriplets(); tix++ ) {
    aoresult_t result= aomw_topo_settriplet( tix, &col ); 
    if( result!=aoresult_ok ) return result;
  }
  return aoresult_ok;
}


// Maps knob angle to a yellow bar
static aoresult_t aoapps_sensors_colortriplets_angle() {
  // Where does the yellow bar stop?
  int stoptix;
  if( aoapps_sensors_angle_current<180 )
    stoptix= aomw_topo_numtriplets() * aoapps_sensors_angle_current / 180;
  else
    stoptix= aomw_topo_numtriplets() * (360-aoapps_sensors_angle_current) / 180;
  // render yellow bar
  const aomw_topo_rgb_t yellow = { 0x1FFF,0x1FFF,0x0000, "dimyellow" };
  for( int tix=0; tix<aomw_topo_numtriplets(); tix++ ) {
    aomw_topo_rgb_t col = tix<=stoptix ? yellow : aomw_topo_off;
    aoresult_t result= aomw_topo_settriplet( tix, &col ); 
    if( result!=aoresult_ok ) return result;
  }
  return aoresult_ok;
}


// colors the triplets, depending on active sensor
static aoresult_t aoapps_sensors_colortriplets() {
  switch( aoapps_sensors_sseg_mode ) {
    case AOAPPS_SENSORS_SSEG_MODE_TEMP : return aoapps_sensors_colortriplets_temp();
    case AOAPPS_SENSORS_SSEG_MODE_ANGLE: return aoapps_sensors_colortriplets_angle();
    case AOAPPS_SENSORS_SSEG_MODE_LIGHT: return aoapps_sensors_colortriplets_light();
    default: return aoresult_ok;
  }
}


// === Mode switching ========================================================


// Check if buttons are pressed to switch mode
static aoresult_t aoapps_sensors_mode_switch( int forcemode) {
  aoresult_t result;
  
  // Poll ui32 buttons (for X press)
  (void)0; // aoui32_but_scan() is already called by app manager
  // Does X request a new mode?  
  int mode= forcemode; // 0= no new mode
  if( aoui32_but_wentdown(AOUI32_BUT_X) ) {
    switch( aoapps_sensors_sseg_mode ) { // switch to next mode
      case AOAPPS_SENSORS_SSEG_MODE_TEMP : mode= AOAPPS_SENSORS_SSEG_MODE_ANGLE; break;
      case AOAPPS_SENSORS_SSEG_MODE_ANGLE: mode= AOAPPS_SENSORS_SSEG_MODE_LIGHT; break;
      case AOAPPS_SENSORS_SSEG_MODE_LIGHT: mode= AOAPPS_SENSORS_SSEG_MODE_TEMP; break;
    }
    // Correct non presence
    if( mode==AOAPPS_SENSORS_SSEG_MODE_ANGLE && !aoapps_sensors_angle_present ) mode= AOAPPS_SENSORS_SSEG_MODE_LIGHT;
    if( mode==AOAPPS_SENSORS_SSEG_MODE_LIGHT && !aoapps_sensors_light_present ) mode= AOAPPS_SENSORS_SSEG_MODE_TEMP;
    // Even if temp not present, we have temp from SAID
  }
  if( aoui32_but_wentdown(AOUI32_BUT_Y) ) {
    switch( aoapps_sensors_sseg_mode ) { // switch to next mode
      case AOAPPS_SENSORS_SSEG_MODE_TEMP : aoapps_sensors_scrollptr = "    AS6212    "; break;
      case AOAPPS_SENSORS_SSEG_MODE_ANGLE: aoapps_sensors_scrollptr = "    AS5600    "; break;
      case AOAPPS_SENSORS_SSEG_MODE_LIGHT: aoapps_sensors_scrollptr = "    SFH 5721    "; break;
    }
    aoapps_sensors_scrollms= millis();
  }

  // Poll selector buttons
  if( aoapps_sensors_selector_present ) {
    result= aomw_iox4b4l_but_scan( );
    if( result!=aoresult_ok ) return result;
    // Does selector request a new mode
    if( aomw_iox4b4l_but_wentdown(AOMW_IOX4B4L_BUT0) ) {    
      mode= AOAPPS_SENSORS_SSEG_MODE_TEMP; // Even if temp not present, we have temp from SAID
    }
    if( aomw_iox4b4l_but_wentdown(AOMW_IOX4B4L_BUT1) ) {
      if( aoapps_sensors_angle_present ) mode= AOAPPS_SENSORS_SSEG_MODE_ANGLE; else mode= aoapps_sensors_sseg_mode; // stay in current, but show units
    }
    if( aomw_iox4b4l_but_wentdown(AOMW_IOX4B4L_BUT2) ) {
      if( aoapps_sensors_light_present ) mode= AOAPPS_SENSORS_SSEG_MODE_LIGHT; else mode= aoapps_sensors_sseg_mode; // stay in current, but show units
    }
    if( aomw_iox4b4l_but_wentdown(AOMW_IOX4B4L_BUT3) ) {
      mode= AOAPPS_SENSORS_SSEG_MODE_TEMP; // Fall back to temp
    }
  }
  
  // Is there a new mode?
  if( mode!=0 ) { // yes, flash units and update indicator on selector 
    // Record new mode
    aoapps_sensors_sseg_mode= mode;
    // determine unit and indicator id
    const char * unit;
    int ind;
    if( mode==AOAPPS_SENSORS_SSEG_MODE_TEMP  ) { unit=" #C "; ind=AOMW_IOX4B4L_LED0; }
    if( mode==AOAPPS_SENSORS_SSEG_MODE_ANGLE ) { unit="#360"; ind=AOMW_IOX4B4L_LED1; }
    if( mode==AOAPPS_SENSORS_SSEG_MODE_LIGHT ) { unit="lu><"; ind=AOMW_IOX4B4L_LED2; }
    // flash units on display
    if( aoapps_sensors_sseg_present ) {
      result= aomw_sseg_printf(unit); 
      if( result!=aoresult_ok ) return result;
      aoapps_sensors_holdms = millis();
    }
    // update indicator
    if( aoapps_sensors_selector_present ) {
      result= aomw_iox4b4l_led_set(ind);
      if( result!=aoresult_ok ) return result;
    }
    // Suppress scroll
    aoapps_sensors_scrollptr="";
  }
  
  return aoresult_ok;
}


// === Top-level state machine ===============================================


// The application manager entry point (start)
static aoresult_t aoapps_sensors_start() {
  aoresult_t result;

  // Assume none is present (vars are set below, but there are returns for errors)
  aoapps_sensors_temp_present=0;     
  aoapps_sensors_angle_present=0;    
  aoapps_sensors_light_present=0;    
  aoapps_sensors_sseg_present=0; 
  aoapps_sensors_selector_present=0; 

  // Is there a temperature sensor in the OSP chain?
  uint16_t addr_temp;
  result= aomw_topo_i2cfind( AOMW_AS6212_DADDR7_SAIDSENSE, &addr_temp );
  if( result!=aoresult_ok && result!=aoresult_dev_noi2cdev ) return result;
  aoapps_sensors_temp_present= result==aoresult_ok;
  // Init sensor
  if( aoapps_sensors_temp_present ) {
    result= aomw_as6212_init( addr_temp );
    if( result!=aoresult_ok ) return result;
    // Set highest conversion rate for the temperature sensor (8Hz=125ms)
    result= aomw_as6212_convrate_set(125);
    if( result!=aoresult_ok ) return result;
    PRINTF("sensors: using temp sensor %02X on SAID %03X \n",AOMW_AS6212_DADDR7_SAIDSENSE,addr_temp);
  } else {
    PRINTF("sensors: no temp sensor found, falling back on SAID %03X\n", AOAPPS_SENSORS_SAID_ADDR);
  }

  // Is there a rotary sensor in the OSP chain?
  uint16_t addr_angle;
  result= aomw_topo_i2cfind( AOMW_AS5600_DADDR7_SAIDSENSE, &addr_angle );
  if( result!=aoresult_ok && result!=aoresult_dev_noi2cdev ) return result;
  aoapps_sensors_angle_present= result==aoresult_ok;
  // Init sensor
  if( aoapps_sensors_angle_present ) {
    result= aomw_as5600_init( addr_angle );
    if( result!=aoresult_ok ) return result;
    PRINTF("sensors: using rotation sensor %02X on SAID %03X\n",AOMW_AS5600_DADDR7_SAIDSENSE,addr_angle);
  } else {
    PRINTF("sensors: no rotation sensor found\n");
  }

  // Is there a light sensor in the OSP chain?
  uint16_t addr_light;
  result= aomw_topo_i2cfind( AOMW_SFH5721_DADDR7_SAIDSENSE, &addr_light );
  if( result!=aoresult_ok && result!=aoresult_dev_noi2cdev ) return result;
  aoapps_sensors_light_present= result==aoresult_ok;
  // Init sensor
  if( aoapps_sensors_light_present ) {
    result= aomw_sfh5721_init( addr_light );
    if( result!=aoresult_ok ) return result;
    PRINTF("sensors: using light sensor %02X on SAID %03X\n",AOMW_SFH5721_DADDR7_SAIDSENSE,addr_light);
  } else {
    PRINTF("sensors: no light sensor found\n");
  }

  // Is there a quad 7-segment display in the OSP chain?
  uint16_t addr_sseg;
  result= aomw_topo_i2cfind( AOMW_SSEG_0_DADDR7_SAIDSENSE, &addr_sseg );
  if( result!=aoresult_ok && result!=aoresult_dev_noi2cdev ) return result;
  aoapps_sensors_sseg_present= result==aoresult_ok;
  // Init display
  if( aoapps_sensors_sseg_present ) {
    result= aomw_sseg_init( addr_sseg );
    if( result!=aoresult_ok ) return result;
    PRINTF("sensors: using display %02X on SAID %03X\n",AOMW_SSEG_0_DADDR7_SAIDSENSE,addr_sseg);
  } else {
    PRINTF("sensors: no display found\n");
  }

  // Is there a selector in the OSP chain?
  uint16_t addr_selector;
  result= aomw_topo_i2cfind( AOMW_IOX4B4L_DADDR7_SAIDSENSE, &addr_selector );
  if( result!=aoresult_ok && result!=aoresult_dev_noi2cdev ) return result;
  aoapps_sensors_selector_present= result==aoresult_ok;
  // Init selector, the I/O-expander on the SAIDsense board
  if( aoapps_sensors_selector_present ) {
    result= aomw_iox4b4l_init(addr_selector, AOMW_IOX4B4L_DADDR7_SAIDSENSE, AOMW_IOX4B4L_PINCFG_SAIDSENSE); 
    if( result!=aoresult_ok ) return result;
    PRINTF("sensors: using selector %02X on SAID %03X\n",AOMW_IOX4B4L_DADDR7_SAIDSENSE,addr_selector);
  } else {
    // Is there a pre-production SAIDsense V2 in the OSP chain?
    uint16_t addr_selector;
    result= aomw_topo_i2cfind( AOMW_IOX4B4L_DADDR7_SAIDSENSEV2, &addr_selector );
    if( result!=aoresult_ok && result!=aoresult_dev_noi2cdev ) return result;
    aoapps_sensors_selector_present= result==aoresult_ok;
    // Init selector, the I/O-expander on the SAIDsense board
    if( aoapps_sensors_selector_present ) {
      result= aomw_iox4b4l_init(addr_selector, AOMW_IOX4B4L_DADDR7_SAIDSENSEV2, AOMW_IOX4B4L_PINCFG_SAIDSENSEV2); 
      if( result!=aoresult_ok ) return result;
      PRINTF("sensors: using selector %02X on SAID %03X\n",AOMW_IOX4B4L_DADDR7_SAIDSENSEV2,addr_selector);
    } else {
      PRINTF("sensors: no selector found\n");
    }
  }

  // Set up state machine
  aoapps_sensors_scrollptr="";
  // Get first temp measurement for baseline (average)
  result= aoapps_sensors_temp_measure();
  if( result!=aoresult_ok ) return result;
  aoapps_sensors_temp_average= aoapps_sensors_temp_current;
  // Mode (which sensor is active)
  aoapps_sensors_sseg_mode= AOAPPS_SENSORS_SSEG_MODE_TEMP; // always available (because fallback on SAID)
  aoapps_sensors_mode_switch(aoapps_sensors_sseg_mode);
  // Record time stamp of last update
  aoapps_sensors_lastms= millis();
  
  return aoresult_ok;
}


// The application manager entry point (step)
static aoresult_t aoapps_sensors_step() {
  aoresult_t result;

  // Update mode for button presses - checking buttons must be done each call to prevent missing a wentdown()
  result= aoapps_sensors_mode_switch();
  if( result!=aoresult_ok ) return result;

  // Bail out if to early
  if( millis()-aoapps_sensors_lastms < AOAPPS_SENSORS_ANIM_MS ) return aoresult_ok;

  // Get temperature
  result= aoapps_sensors_temp_measure();
  if( result!=aoresult_ok ) { PRINTF("sensors: error reading temperature sensor\n"); return result; }
  // Get knob angle
  result= aoapps_sensors_angle_measure();
  if( result!=aoresult_ok ) { PRINTF("sensors: error reading rotation sensor\n"); return result; }
  // Get light level
  result= aoapps_sensors_light_measure();
  if( result!=aoresult_ok ) { PRINTF("sensors: error reading light sensor\n"); return result; }
  
  // Update display
  result= aoapps_sensors_display();
  if( result!=aoresult_ok ) { PRINTF("sensors: error updating display\n"); return result; }

  // Update color of nodes in chain
  result= aoapps_sensors_colortriplets();
  if( result!=aoresult_ok ) { PRINTF("sensors: error updating triplets\n"); return result; }

  aoapps_sensors_lastms= millis();
  // return success
  return aoresult_ok;
}


// The application manager entry point (stop)
static void aoapps_sensors_stop() {
  // Clear the quad 7-segment display
  if( aoapps_sensors_sseg_present ) {
    aomw_sseg_clr();
  }
  // Clear the indicator LEDs of the selector
  if( aoapps_sensors_selector_present ) {
    aomw_iox4b4l_led_off(AOMW_IOX4B4L_LEDALL);
  }
}


// === Public ================================================================


/*!
    @brief  Registers the sensor app with the app manager.
    @note   This app continuously reads out a sensor and shows its value on a
            quad 7-segment display. It animates the OSP chain depending 
            on the chosen sensor and the sensor value.
    @note   This app is written for the SAIDsense demo board. It can handle
            temperature sensor, magnetic rotary sensor and the ambient light 
            sensor on that board. Older boards do not have the rotary or 
            light sensor, the app can handle that (prevents selecting them).
    @note   If no sensor is found, the app uses only the temp mode, and uses 
            a SAID for that.
*/
void aoapps_sensors_register() {
  aoapps_mngr_register("sensors", "Sensors", "modality", "show type",
    AOAPPS_MNGR_FLAGS_WITHTOPO | AOAPPS_MNGR_FLAGS_WITHREPAIR,
    aoapps_sensors_start, aoapps_sensors_step, aoapps_sensors_stop,
    0, 0 /* no config command */ );
}


/*!
    @brief  Resets the hardware controlled by the sensor app.
            This switches off the (segments of the) quad 7-segment display 
            and the indicator lights of the selector.
    @return aoresult_ok iff successful
    @note   This function is supposed to be called in setup() of executables
            that contain the sensor app to prevent the display and indicator 
            LED from staying on after a reboot (see notes below for an
            explanation).
    @note   The sensor app uses the quad 7-segment display to show the current 
            sensor readout. A reboot of the executable restarts the app manager, 
            which restarts the first app. In case this first app is not the 
            sensor app, the quad 7-segment display would stay on.
    @note   The sensor app uses the indicator LEDs of the selector to show which 
            sensor is active. A reboot of the executable restarts the app 
            manager, which restarts the first app. In case this first app is not 
            the sensor app, the indicator LED would stay on.
*/
aoresult_t aoapps_sensors_resethw() {
  aoresult_t result;
  uint16_t   addr;

  // Init chain and find I2C bridges
  result= aomw_topo_build();
  if( result!=aoresult_ok ) return result;

  // Is there a (quad 7-segment) display in the chain (search for first segment only)
  result= aomw_topo_i2cfind( AOMW_SSEG_0_DADDR7_SAIDSENSE, &addr );
  if( result!=aoresult_ok ) return result;
  // Init display; this switches the segments off
  result= aomw_sseg_init( addr );
  if( result!=aoresult_ok ) return result;

  // Is there a SAIDsense (old V2) selector in the chain?
  result= aomw_topo_i2cfind( AOMW_IOX4B4L_DADDR7_SAIDSENSEV2, &addr );
  if( result==aoresult_ok ) { 
    // Init selector; this switches the indicator LEDs off
    result= aomw_iox4b4l_init(addr, AOMW_IOX4B4L_DADDR7_SAIDSENSEV2, AOMW_IOX4B4L_PINCFG_SAIDSENSEV2);
    if( result!=aoresult_ok ) return result;
  }

  // Is there a SAIDsense selector in the chain?
  result= aomw_topo_i2cfind( AOMW_IOX4B4L_DADDR7_SAIDSENSE, &addr );
  if( result==aoresult_ok ) { 
    // Init selector; this switches the indicator LEDs off
    result= aomw_iox4b4l_init(addr, AOMW_IOX4B4L_DADDR7_SAIDSENSE, AOMW_IOX4B4L_PINCFG_SAIDSENSE);
    if( result!=aoresult_ok ) return result;
  }

  return aoresult_ok;
}

