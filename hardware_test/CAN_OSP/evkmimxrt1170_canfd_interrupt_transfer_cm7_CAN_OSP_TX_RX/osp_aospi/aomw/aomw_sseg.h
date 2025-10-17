// aomw_sseg.h - driver for a quad 7-segment display (driven by four IOXs)
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
#ifndef _AOMW_SSEG_H_
#define _AOMW_SSEG_H_

#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"
#include "fsl_lpuart.h"
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"

// Number of 7-segment modules
#define AOMW_SSEG_COUNT 4 // Quad 7-segment display 


// The I2C addresses of the IOX drivers for the 7-segment modules
#define AOMW_SSEG_0_DADDR7_SAIDSENSE 0x38 // Most significant module (left-most)
#define AOMW_SSEG_1_DADDR7_SAIDSENSE 0x39
#define AOMW_SSEG_2_DADDR7_SAIDSENSE 0x3A 
#define AOMW_SSEG_3_DADDR7_SAIDSENSE 0x3B // Least significant module (right-most)


// Clears the quad 7-segment display (all segments off).
aoresult_t aomw_sseg_clr();
// Sets the quad 7-segment display to the pattern encoded in segs an array of length AOMW_SSEG_COUNT.
aoresult_t aomw_sseg_set(const uint8_t * segs);
// Shows first 4 chars of str on the quad 7-segment display (pads with spaces if shorter). Dots can be part of str.
aoresult_t aomw_sseg_print(const char*str);
// Formatted print to the quad 7-segment display.
aoresult_t aomw_sseg_printf(const char * fmt, ... );


// Tests if a quad 7-segment display is connected to the I2C bus of OSP node (SAID) with address `addr`. 
aoresult_t aomw_sseg_present(uint16_t addr );
// Associates this software driver to the quad 7-segment connected to the I2C bus of SAID with address `addr`.
aoresult_t aomw_sseg_init(uint16_t addr);


#endif


