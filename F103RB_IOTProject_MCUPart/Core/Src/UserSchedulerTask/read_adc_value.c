/*
 * read_adc_value.c
 *
 *  Created on: Apr 23, 2023
 *      Author: ntpt
 */

#include "UserSchedulerTask/read_adc_value.h"
#include "adc.h"
#include "globals.h"
#include "usart.h"
#include "stm32f1xx_hal_adc.h"
#include "stm32f1xx_hal_def.h"
#include <math.h>
#include <stdio.h>

uint16_t read_adc1_value(void)
{
	HAL_ADC_Start(&hadc1);
	if (HAL_ADC_PollForConversion(&hadc1, 1) == HAL_OK)
		return HAL_ADC_GetValue(&hadc1);
	else
		return ERROR_ADC_VALUE ;  // consider a error
}

uint16_t read_adc2_value(void)
{
	HAL_ADC_Start(&hadc2);
	if (HAL_ADC_PollForConversion(&hadc2, 1) == HAL_OK)
		return HAL_ADC_GetValue(&hadc2);
	else
		return ERROR_ADC_VALUE ;  // consider a error
}

float get_temperature_value(void)
{
	static const float low_limit = 0.0;
	static const float high_limit = 50.0;
	static const float range = high_limit - low_limit;

	uint16_t adc_value = read_adc1_value();
	if (adc_value == ERROR_ADC_VALUE)
		return NAN;  // not a number

	// just interpolate the adc value to temperature
	return low_limit + adc_value * range / 0xFFFF;
}

float get_humidity_value(void)
{
	static const float low_limit = 0.0;
	static const float high_limit = 100.0;
	static const float range = high_limit - low_limit;

	uint16_t adc_value = read_adc2_value();
	if (adc_value == ERROR_ADC_VALUE)
		return NAN;  // not a number

	// just interpolate the adc value to temperature
	return low_limit + adc_value * range / 0xFFFF;
}

void get_and_send_temperature(void *param)
{
	uint16_t temp_value = read_adc1_value();
	txcv_uart2.txDataSize = sprintf((char *) txcv_uart2.txBuffer, "!0:T:%u", temp_value);
	Hardware_TransceiverUART_Send(&txcv_uart2);
	return;
}

void get_and_send_humidity(void *param)
{
	uint16_t humid_value = read_adc2_value();
	txcv_uart2.txDataSize = sprintf((char *) txcv_uart2.txBuffer, "!0:H:%u", humid_value);
	Hardware_TransceiverUART_Send(&txcv_uart2);
	return;
}
