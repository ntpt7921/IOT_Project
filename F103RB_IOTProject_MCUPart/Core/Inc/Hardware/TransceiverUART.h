/*
 * TransceiverUART.h
 *
 *  Created on: Apr 23, 2023
 *      Author: ntpt
 */

#ifndef INC_HARDWARE_TRANSCEIVERUART_H_
#define INC_HARDWARE_TRANSCEIVERUART_H_

#include "main.h"
#include "stm32f1xx_hal_uart.h"

#define MAXIMUM_DATA_SIZE    (256u)
#define RECEIVE_BUFFER_SIZE  MAXIMUM_DATA_SIZE
#define TRANSMIT_BUFFER_SIZE MAXIMUM_DATA_SIZE

/**
 * NOTE:
 * This module can send and receive UART info in packet. Each packet is a continous stream of UART
 * byte data between after the last IDLE event up until the next IDLE event.
 * A IDLE event is encountered when a idle UART character (RX line is high for 1 frame time) is
 * received. For more info reference the RM0008 document from STMicroelectronics. Mentions of idle
 * char should be in the USART chapter.
 */

struct TransceiverUART_temp
{
    UART_HandleTypeDef *handle;

    // start and end (one past the end) are indices into the buffer, limiting the range of memory
    // that currenty store unprocessed data, if a received packet does not fit into existing space
    // within the buffer, that packet is considered lost
    uint8_t rxBuffer[RECEIVE_BUFFER_SIZE];
    uint16_t rxDataSize;
    uint8_t rxBufferLock;

    uint8_t txBuffer[TRANSMIT_BUFFER_SIZE];
    uint16_t txDataSize;
    uint8_t txBufferLock;

    void (*rxCb)(struct TransceiverUART_temp *txcv);
    void (*txCb)(struct TransceiverUART_temp *txcv);
};

typedef void (*ReceiveCallback)(struct TransceiverUART_temp *txcv);
typedef void (*SendCallback)(struct TransceiverUART_temp *txcv);

typedef struct TransceiverUART_temp TransceiverUART;

void Hardware_TransceiverUART_Init(TransceiverUART *txcv, UART_HandleTypeDef *handle,
                                   ReceiveCallback rxCb, SendCallback txCb);
// keep trying until success, no other packet can be received or sent until complete
// callback will be call when completed
void Hardware_TransceiverUART_Send(TransceiverUART *txcv);
void Hardware_TransceiverUART_Receive(TransceiverUART *txcv);

// lock so that data in the buffer is not overwritten
void Hardware_TransceiverUART_TxBufferLock(TransceiverUART *txcv);
void Hardware_TransceiverUART_TxBufferUnlock(TransceiverUART *txcv);
uint8_t Hardware_TransceiverUART_TxBufferIsLock(TransceiverUART *txcv);
void Hardware_TransceiverUART_RxBufferLock(TransceiverUART *txcv);
void Hardware_TransceiverUART_RxBufferUnlock(TransceiverUART *txcv);
uint8_t Hardware_TransceiverUART_RxBufferIsLock(TransceiverUART *txcv);

#endif /* INC_HARDWARE_TRANSCEIVERUART_H_ */
