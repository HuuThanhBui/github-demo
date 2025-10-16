/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_lpspi.h"
#include "pin_mux.h"
#include "board.h"

#include "fsl_common.h"
#include "WS2812_SPI.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Master related */
#define EXAMPLE_LPSPI_MASTER_BASEADDR         (LPSPI4)
#define EXAMPLE_LPSPI_MASTER_IRQN             (LPSPI4_IRQn)

#define LPSPI_MASTER_CLK_FREQ (CLOCK_GetRootClockFreq(kCLOCK_Root_Lpspi4))

#define EXAMPLE_LPSPI_DEALY_COUNT 0xFFFFFU
#define TRANSFER_SIZE     64U     /*! Transfer dataSize */
#define TRANSFER_BAUDRATE 2500000U /*! Transfer baudrate - 500k */

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t masterRxData[TRANSFER_SIZE] = {0U};
uint8_t masterTxData[TRANSFER_SIZE] = {0U};

int brightness = 50;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    uint32_t srcClock_Hz;
    uint32_t errorCount;
    uint32_t loopCount = 1U;
    uint32_t i;
    lpspi_master_config_t masterConfig;
    lpspi_transfer_t masterXfer;

    /*Master config*/
    LPSPI_MasterGetDefaultConfig(&masterConfig);
//    masterConfig.cpha = kLPSPI_ClockPhaseFirstEdge;
//    masterConfig.cpol = kLPSPI_ClockPolarityActiveHigh;
    masterConfig.baudRate = TRANSFER_BAUDRATE;
    masterConfig.betweenTransferDelayInNanoSec = 1000000000U / (masterConfig.baudRate * 2U);
    
    srcClock_Hz = LPSPI_MASTER_CLK_FREQ;
    LPSPI_MasterInit(EXAMPLE_LPSPI_MASTER_BASEADDR, &masterConfig, srcClock_Hz);

    if (SysTick_Config(SystemCoreClock / 1000U))
	{
		while (1)
		{
		}
	}


	for (int i=0; i<8; i++)
	{
	  setLED(i, 0, 0, 0);
	}
	WS2812_Send();

    while (1)
    {
		for (int i=0; i<8; i++)
		{
		  setLED(i, 255, 0, 0);
		}
		WS2812_Send();
		SDK_DelayAtLeastUs(1000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

		for (int i=0; i<8; i++)
		{
		  setLED(i, 0, 255, 0);
		}
		WS2812_Send();
		SDK_DelayAtLeastUs(1000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

		for (int i=0; i<8; i++)
		{
		  setLED(i, 0, 0, 255);
		}
		WS2812_Send();
		SDK_DelayAtLeastUs(1000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    }
}
