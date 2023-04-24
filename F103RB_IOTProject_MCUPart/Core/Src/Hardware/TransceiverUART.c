/*
 * TransceiverUART.c
 *
 *  Created on: Apr 23, 2023
 *      Author: ntpt
 */

#include "Hardware/TransceiverUART.h"
#include "main.h"
#include "globals.h"
#include "stm32f1xx_hal_uart.h"
#include "usart.h"

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    // huart2 is used only, no other huart hardware is used so we can guard like this
    if (huart == &huart2)
    {
        txcv_uart2.rxDataSize = Size;
        if (txcv_uart2.rxCb != NULL)
        	txcv_uart2.rxCb(&txcv_uart2);  // should set flag to process this message
    }
    else
    {
        return;
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    // huart2 is used only, no other huart hardware is used so we can guard like this
    if (huart == &huart2)
    {
        Hardware_TransceiverUART_TxBufferUnlock(&txcv_uart2);
        if (txcv_uart2.txCb != NULL)
        	txcv_uart2.txCb(&txcv_uart2);  // should set flag to process this message
    }
    else
    {
        return;
    }
}

void Hardware_TransceiverUART_Init(TransceiverUART *txcv, UART_HandleTypeDef *handle,
                                   ReceiveCallback rxCb, SendCallback txCb)
{
    txcv->handle = handle;
    txcv->rxDataSize = txcv->txDataSize = 0;
    txcv->rxBufferLock = txcv->txBufferLock = 0;
    txcv->rxCb = rxCb;
    txcv->txCb = txCb;
}

void Hardware_TransceiverUART_Send(TransceiverUART *txcv)
{
    if (Hardware_TransceiverUART_TxBufferIsLock(txcv))
        return;
    else
    {
        Hardware_TransceiverUART_TxBufferLock(txcv);
        HAL_UART_Transmit_IT(txcv->handle, txcv->txBuffer, txcv->txDataSize);
    }
}

void Hardware_TransceiverUART_Receive(TransceiverUART *txcv)
{
    if (Hardware_TransceiverUART_RxBufferIsLock(txcv))
        return;
    else
    {
        Hardware_TransceiverUART_RxBufferLock(txcv);
        HAL_UARTEx_ReceiveToIdle_IT(txcv->handle, txcv->rxBuffer, RECEIVE_BUFFER_SIZE);
    }
}

void Hardware_TransceiverUART_TxBufferLock(TransceiverUART *txcv) { txcv->rxBufferLock = 1; }

void Hardware_TransceiverUART_TxBufferUnlock(TransceiverUART *txcv) { txcv->rxBufferLock = 0; }

uint8_t Hardware_TransceiverUART_TxBufferIsLock(TransceiverUART *txcv)
{
    return (txcv->rxBufferLock == 1);
}

void Hardware_TransceiverUART_RxBufferLock(TransceiverUART *txcv) { txcv->txBufferLock = 1; }

void Hardware_TransceiverUART_RxBufferUnlock(TransceiverUART *txcv) { txcv->txBufferLock = 0; }

uint8_t Hardware_TransceiverUART_RxBufferIsLock(TransceiverUART *txcv)
{
    return (txcv->txBufferLock == 1);
}
