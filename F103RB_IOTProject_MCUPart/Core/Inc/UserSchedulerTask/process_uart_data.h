/*
 * read_uart_data.h
 *
 *  Created on: Apr 24, 2023
 *      Author: ntpt
 */

#ifndef INC_USERSCHEDULERTASK_PROCESS_UART_DATA_H_
#define INC_USERSCHEDULERTASK_PROCESS_UART_DATA_H_

#include "main.h"
#include "globals.h"

#define READ_UART_DATA_SCHEDTASK_ID (3u)
#define SEND_UART_DATA_SCHEDTASK_ID (4u)

void read_uart_data_in_buffer(void *param);
void send_uart_data_in_buffer(void *param);

#endif /* INC_USERSCHEDULERTASK_PROCESS_UART_DATA_H_ */
