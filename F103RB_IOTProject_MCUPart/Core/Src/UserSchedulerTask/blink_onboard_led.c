/*
 * blink_onboard_led.c
 *
 *  Created on: Apr 23, 2023
 *      Author: ntpt
 */

#include "UserSchedulerTask/blink_onboard_led.h"
#include "main.h"

// is of type SchedulerTask_Callback
void blink_onboard_led(void *param)
{
	HAL_GPIO_TogglePin(ONBOARD_LED_GPIO_Port, ONBOARD_LED_Pin);
}
