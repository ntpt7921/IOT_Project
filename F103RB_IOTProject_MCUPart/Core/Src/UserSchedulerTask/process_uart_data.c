/*
 * process_uart_data.c
 *
 *  Created on: Apr 24, 2023
 *      Author: ntpt
 */

#include "UserSchedulerTask/process_uart_data.h"
#include "UserSchedulerTask/blink_onboard_led.h"
#include "globals.h"
#include "main.h"
#include "usart.h"
#include "Hardware/TransceiverUART.h"
#include "Logical/Scheduler/Scheduler.h"
#include "Logical/FiniteStateMachine/FiniteStateMachine.h"
#include "Logical/FiniteStateMachine/FiniteStateMachine_State.h"
#include <stdio.h>

static uint8_t *curr_char;
static uint16_t char_left;
static char type; // can be P ('pump') or L ('light')
static char value; // can be '0' or '1'

static FiniteStateMachine_State parser_get_next_state(FiniteStateMachine_State current_state)
{
	char curr_data = *(char*) curr_char;
	FiniteStateMachine_State next_state = current_state;

	switch (current_state)
	{
		case START_STATE:
			if (curr_data == '!')
				next_state = CHAR_0_STATE;
			break;
		case CHAR_0_STATE:
			next_state = DELIMIT_0_STATE;
			break;
		case DELIMIT_0_STATE:
			if (curr_data == ':')
				next_state = CHAR_TYPE_STATE;
			else
				next_state = START_STATE;
			break;
		case CHAR_TYPE_STATE:
			if (curr_data == 'P' || curr_data == 'L')
			{
				type = curr_data;
				next_state = DELIMIT_1_STATE;
			}
			else
				next_state = START_STATE;
			break;
		case DELIMIT_1_STATE:
			if (curr_data == ':')
				next_state = CHAR_VALUE_STATE;
			else
				next_state = START_STATE;
			break;
		case CHAR_VALUE_STATE:
			if (curr_data == '0' || curr_data == '1')
			{
				value = curr_data;
				next_state = END_STATE;
			}
			else
				next_state = START_STATE;
			break;
		case END_STATE:
			next_state = START_STATE;
			break;
		default:
			break;
	}

	return next_state;
}

static void parser_do_in_state(FiniteStateMachine_State current_state)
{
	switch (current_state)
	{
		case START_STATE:
			break;
		case CHAR_0_STATE:
			break;
		case DELIMIT_0_STATE:
			break;
		case CHAR_TYPE_STATE:
			break;
		case DELIMIT_1_STATE:
			break;
		case CHAR_VALUE_STATE:
			break;
		case END_STATE:
			if (type == 'P')
			{
				txcv_uart2.txDataSize = sprintf((char*) txcv_uart2.txBuffer, "!0:P:%c#", value);
				Hardware_TransceiverUART_Send(&txcv_uart2);
			}
			else if (type == 'L')
			{
				txcv_uart2.txDataSize = sprintf((char*) txcv_uart2.txBuffer, "!0:L:%c#", value);
				Hardware_TransceiverUART_Send(&txcv_uart2);
				if (value == '0')
					Scheduler_Delete(BLINK_ONBOARD_LED_SCHEDTASK_ID);
				else if (value == '1')
					Scheduler_Add(blink_onboard_led, NULL, 0, MS_TO_SCHEDTICK(500), 0,
								  BLINK_ONBOARD_LED_SCHEDTASK_ID);
			}
			break;
		default:
			break;
	}
}

FiniteStateMachine parser =
{ .current_state = START_STATE, .get_next_state = parser_get_next_state, .do_in_state =
		parser_do_in_state, .set_next_state = NULL, };

void read_uart_data_in_buffer(void *param)
{
	// if there is data in the buffer, process it
	if (Hardware_TransceiverUART_RxBufferIsLock(&txcv_uart2) && txcv_uart2.rxDataSize != 0)
	{
		curr_char = txcv_uart2.rxBuffer;
		char_left = txcv_uart2.rxDataSize;
		while (char_left)
		{
			FiniteStateMachine_Run(&parser);
			curr_char++;
			char_left--;
		}

		Hardware_TransceiverUART_RxBufferUnlock(&txcv_uart2);
		Hardware_TransceiverUART_Receive(&txcv_uart2);
		txcv_uart2.rxDataSize = 0;
	}
}

void send_uart_data_in_buffer(void *param)
{
	// this task is invoked after some uart data had been successfully sent
	// since we don't do anything special, this do nothing
	// this function is not used
	return;
}
