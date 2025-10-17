/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_flexio_uart.h"
#include "fsl_lpuart.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_FLEXIO_BASE  FLEXIO2
#define FLEXIO_UART_TX_PIN 4U
#define FLEXIO_UART_RX_PIN 5U

#define DEMO_LPUART            LPUART8
#define DEMO_LPUART_IRQn       LPUART8_IRQn
#define DEMO_LPUART_IRQHandler LPUART8_IRQHandler

#define FLEXIO_CLOCK_FREQUENCY (CLOCK_GetRootClockFreq(kCLOCK_Root_Flexio2))
#define DEMO_LPUART_CLK_FREQ BOARD_DebugConsoleSrcFreq()
#define ECHO_BUFFER_LENGTH 50

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* UART user callback */
void FLEXIO_UART_UserCallback(FLEXIO_UART_Type *base, flexio_uart_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
flexio_uart_handle_t g_uartHandle;
lpuart_handle_t g_lpuartHandle;
FLEXIO_UART_Type uartDev;

uint8_t g_tipString[30] = {0};

uint8_t g_txBuffer[ECHO_BUFFER_LENGTH] = {0};
uint8_t g_rxBuffer[ECHO_BUFFER_LENGTH] = {0};
volatile bool rxBufferEmpty            = true, rxlogBufferEmpty = true;
volatile bool txBufferFull             = false, txlogBufferFull = false;
volatile bool txOnGoing                = false, txlogOnGoing = false;
volatile bool rxOnGoing                = false, rxlogOnGoing = false;
uint8_t indx = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/
/* UART user callback */
void FLEXIO_UART_UserCallback(FLEXIO_UART_Type *base, flexio_uart_handle_t *handle, status_t status, void *userData)
{
    userData = userData;
//    GPIO_PinWrite(GPIO8, 10, 0);
//    GPIO_PinWrite(GPIO8, 11, 0);
//    GPIO_PinWrite(GPIO8, 12, 0);
//    GPIO_PinWrite(GPIO8, 13, 0);

    if (kStatus_FLEXIO_UART_TxIdle == status)
    {
        txBufferFull = false;
        txOnGoing    = false;
    }

    if (kStatus_FLEXIO_UART_RxIdle == status)
    {
        rxBufferEmpty = false;
        rxOnGoing     = false;
    }
}

void LPUART_UserCallback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{
    userData = userData;

    if (kStatus_LPUART_TxIdle == status)
    {
        txlogBufferFull = false;
        txlogOnGoing    = false;
    }

    if (kStatus_LPUART_RxIdle == status)
    {
        rxlogBufferEmpty = false;
        rxlogOnGoing     = false;
    }
}

uint8_t pid_Calc (uint8_t ID)
{
	if(ID>0x3F)
	{
		return 0;
	}
	uint8_t IDBuf[6];
	for (int i=0; i<6; i++)
	{
		IDBuf[i] = (ID>>i)&0x01;
	}

	uint8_t P0 = (IDBuf[0] ^ IDBuf[1] ^ IDBuf[2] ^ IDBuf[4])&0x01;
	uint8_t P1 = ~((IDBuf[1] ^ IDBuf[3] ^ IDBuf[4] ^ IDBuf[5])&0x01);

	ID = ID | (P0<<6) | (P1<<7);
	return ID;
}

uint8_t checksum_Calc (uint8_t PID, uint8_t *data, uint8_t size)
{
	uint8_t buffer[size+2];
	uint16_t sum=0;
	buffer[0] = PID;
	for (int i=0; i<size; i++)
	{
		buffer[i+1] = data[i];
	}

	for (int i=0; i<size+1; i++)
	{
		sum = sum + buffer[i];
		if (sum>0xff) sum = sum-0xff;
	}

	sum = 0xff-sum;
	return sum;
}

/*!
 * @brief Main function
 */
int main(void)
{
	gpio_pin_config_t led_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
    lpuart_config_t config_log;
    flexio_uart_config_t config;
    flexio_uart_transfer_t xfer;
    flexio_uart_transfer_t sendXfer;
    flexio_uart_transfer_t receiveXfer;
    status_t result = kStatus_Success;

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();

    /*
	 * config.baudRate_Bps = 115200U;
	 * config.parityMode = kLPUART_ParityDisabled;
	 * config.stopBitCount = kLPUART_OneStopBit;
	 * config.txFifoWatermark = 0;
	 * config.rxFifoWatermark = 0;
	 * config.enableTx = false;
	 * config.enableRx = false;
	 */
	LPUART_GetDefaultConfig(&config_log);
	config_log.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
	config_log.enableTx     = true;
	config_log.enableRx     = true;

	LPUART_Init(LPUART8, &config_log, DEMO_LPUART_CLK_FREQ);
	LPUART_TransferCreateHandle(DEMO_LPUART, &g_lpuartHandle, LPUART_UserCallback, NULL);
	LPUART_EnableInterrupts(LPUART8, kLPUART_RxDataRegFullInterruptEnable);
	EnableIRQ(DEMO_LPUART_IRQn);

	GPIO_PinInit(GPIO8, 19, &led_config);
	GPIO_PinInit(GPIO8, 20, &led_config);

	GPIO_PinInit(GPIO8, 10, &led_config);
	GPIO_PinInit(GPIO8, 11, &led_config);
	GPIO_PinInit(GPIO8, 12, &led_config);
	GPIO_PinInit(GPIO8, 13, &led_config);

    /*
     * config.enableUart = true;
     * config.enableInDoze = false;
     * config.enableInDebug = true;
     * config.enableFastAccess = false;
     * config.baudRate_Bps = 115200U;
     * config.bitCountPerChar = kFLEXIO_UART_8BitsPerChar;
     */
    FLEXIO_UART_GetDefaultConfig(&config);
    config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
    config.enableUart   = true;

    uartDev.flexioBase      = BOARD_FLEXIO_BASE;
    uartDev.TxPinIndex      = FLEXIO_UART_TX_PIN;
    uartDev.RxPinIndex      = FLEXIO_UART_RX_PIN;
    uartDev.shifterIndex[0] = 0U;
    uartDev.shifterIndex[1] = 1U;
    uartDev.timerIndex[0]   = 0U;
    uartDev.timerIndex[1]   = 1U;

    result = FLEXIO_UART_Init(&uartDev, &config, FLEXIO_CLOCK_FREQUENCY);
    if (result != kStatus_Success)
    {
        return -1;
    }

    FLEXIO_UART_TransferCreateHandle(&uartDev, &g_uartHandle, FLEXIO_UART_UserCallback, NULL);

//    /* Send g_tipString out. */
//    xfer.data     = g_tipString;
//    xfer.dataSize = sizeof(g_tipString) - 1;
//    txOnGoing     = true;
//    FLEXIO_UART_TransferSendNonBlocking(&uartDev, &g_uartHandle, &xfer);

//    /* Wait send finished */
//    while (txOnGoing)
//    {
//    }

    /* Start to echo. */
    sendXfer.data        = g_txBuffer;
    sendXfer.dataSize    = ECHO_BUFFER_LENGTH;
    receiveXfer.data     = g_rxBuffer;
    receiveXfer.dataSize = ECHO_BUFFER_LENGTH;

    GPIO_PinWrite(GPIO8, 19, 1);
    GPIO_PinWrite(GPIO8, 20, 1);

    GPIO_PinWrite(GPIO8, 10, 1);
    GPIO_PinWrite(GPIO8, 11, 1);
    GPIO_PinWrite(GPIO8, 12, 1);
    GPIO_PinWrite(GPIO8, 13, 1);

    while (1)
    {
    	DEMO_LPUART->STAT |= LPUART_STAT_BRK13_MASK;
		DEMO_LPUART->BAUD &= ~LPUART_BAUD_SBNS_MASK;
		DEMO_LPUART->CTRL |= LPUART_CTRL_SBK_MASK;

		while (!(DEMO_LPUART->STAT & LPUART_STAT_TDRE_MASK)) {
			;
		}

		DEMO_LPUART->CTRL &= ~LPUART_CTRL_SBK_MASK;

		while (!(DEMO_LPUART->STAT & LPUART_STAT_TC_MASK)) {
			;
		}

		g_tipString[0] = 0x55;
		g_tipString[1] = pid_Calc(0x34);

		for (int i=0; i<8; i++)
		{
			g_tipString[i+2] = indx++;
			if (indx >255) indx = 0;
		}

		g_tipString[10] = checksum_Calc(g_tipString[1], g_tipString+2, 8);   //lin 2.1 includes PID, for line v1 use PID =0


		LPUART_WriteBlocking(DEMO_LPUART, g_tipString, 11);
		indx = 0;





















//        SDK_DelayAtLeastUs(1000000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        /* If RX is idle and g_rxBuffer is empty, start to read data to g_rxBuffer. */
        if ((!rxOnGoing) && rxBufferEmpty)
        {
            rxOnGoing = true;
            FLEXIO_UART_TransferReceiveNonBlocking(&uartDev, &g_uartHandle, &receiveXfer, NULL);
        }

        /* If TX is idle and g_txBuffer is full, start to send data. */
        if ((!txOnGoing) && txBufferFull)
        {
            txOnGoing = true;
            FLEXIO_UART_TransferSendNonBlocking(&uartDev, &g_uartHandle, &sendXfer);
//            LPUART_WriteBlocking(DEMO_LPUART, (uint8_t *)sendXfer.data, sendXfer.dataSize);
        }

        /* If g_txBuffer is empty and g_rxBuffer is full, copy g_rxBuffer to g_txBuffer. */
        if ((!rxBufferEmpty) && (!txBufferFull))
        {
            memcpy(g_txBuffer, g_rxBuffer, ECHO_BUFFER_LENGTH);
            rxBufferEmpty = true;
            txBufferFull  = true;
//            LPUART_WriteBlocking(DEMO_LPUART, g_rxBuffer, 1);
        }
    }
}
