/*
 * read_adc_value.h
 *
 *  Created on: Apr 23, 2023
 *      Author: ntpt
 */

#ifndef INC_USERSCHEDULERTASK_READ_ADC_VALUE_H_
#define INC_USERSCHEDULERTASK_READ_ADC_VALUE_H_

#include "main.h"

#define ERROR_ADC_VALUE (uint16_t)(0xFFFF)
#define TEMP_TASK_ID    (1u)
#define HUMID_TASK_ID   (2u)

// both read will be blocking, because reading is very fast
// ADC read set to 20 conversion time (12.5 + 7.5 sample cycle)
uint16_t read_adc1_value(void);
uint16_t read_adc2_value(void);

float get_temperature_value(void);
float get_humidity_value(void);

void get_and_send_temperature(void *param);
void get_and_send_humidity(void *param);

#endif /* INC_USERSCHEDULERTASK_READ_ADC_VALUE_H_ */
