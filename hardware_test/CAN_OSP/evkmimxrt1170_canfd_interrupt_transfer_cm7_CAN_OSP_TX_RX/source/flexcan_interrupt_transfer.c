#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_lpspi.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_lpuart.h"
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "aospi.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#define CYCLES_PER_LOOP_TUNE 8

void delay_micro(uint32_t us)
{
    uint64_t total_cycles_required = (uint64_t)us * (SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY / 1000000U);

    uint32_t iterations = (uint32_t)(total_cycles_required / CYCLES_PER_LOOP_TUNE);

    for (uint32_t i = 0; i < iterations; i++) {
        __asm__ __volatile__ ("nop");
    }
}

void tele_reset() {
	const uint8_t reset[] = {0xA0, 0x00, 0x00, 0x22};
	aoresult_t result = aospi_tx( reset, sizeof reset );
	PRINTF("reset(0x000) %s\n", aoresult_to_str(result,1) );
}


void tele_initbidir() {
  const uint8_t initbidir[] = {0xA0, 0x04, 0x02, 0xA9};
  aoresult_t result = aospi_tx( initbidir, sizeof initbidir);
  PRINTF("initbidir(0x001) %s\n", aoresult_to_str(result,1) );
}


void tele_initloop() {
  const uint8_t initloop[] = {0xA0, 0x04, 0x03, 0x86};
  aoresult_t result = aospi_tx( initloop, sizeof initloop);
  PRINTF("initloop(0x001) %s\n", aoresult_to_str(result,1) );
}


void tele_clrerror() {
  const uint8_t clrerror[] = {0xA0, 0x00, 0x01, 0x0D};
  aoresult_t result = aospi_tx( clrerror, sizeof clrerror);
  PRINTF("clrerror(0x000) %s\n", aoresult_to_str(result,1) );
}


void tele_goactive() {
  const uint8_t goactive[] = {0xA0, 0x00, 0x05, 0xB1};
  aoresult_t result = aospi_tx( goactive, sizeof goactive);
  PRINTF("goactive(0x000) %s\n", aoresult_to_str(result,1) );
}


void tele_setpwmchn_hi() {
  const uint8_t setpwmchn[] = {0xA0, 0x07, 0xCF, 0x00, 0xFF, 0x08, 0x88, 0x00, 0x11, 0x08, 0x88, 0x94};
  aoresult_t result = aospi_tx( setpwmchn, sizeof setpwmchn);
  PRINTF("setpwmchn(0x001,0,0x0888,0x0011,0x0888) %s\n", aoresult_to_str(result,1) );
}


void tele_setpwmchn_lo() {
  const uint8_t setpwmchn[] = {0xA0, 0x07, 0xCF, 0x00, 0xFF, 0x00, 0x11, 0x08, 0x88, 0x00, 0x11, 0x17};
  aoresult_t result = aospi_tx( setpwmchn, sizeof setpwmchn);
  PRINTF("setpwmchn(0x001,0,0x0011,0x0888,0x0011) %s\n", aoresult_to_str(result,1) );
}

int main(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    aospi_init(aospi_phy_mcua);

	tele_reset();
	delay_micro(150000);
	aospi_dirmux_set_bidir();
	tele_initbidir();
	delay_micro(15000);
	tele_clrerror();
	tele_goactive();
	tele_setpwmchn_hi();

    while (1)
    {
    	tele_setpwmchn_lo();
    	delay_micro(1000000);
    	tele_setpwmchn_hi();
    	delay_micro(1000000);
    }
}
