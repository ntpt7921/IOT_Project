/*
 * blink_onboard_led.h
 *
 *  Created on: Apr 23, 2023
 *      Author: ntpt
 */

#ifndef INC_USERSCHEDULERTASK_BLINK_ONBOARD_LED_H_
#define INC_USERSCHEDULERTASK_BLINK_ONBOARD_LED_H_

// used as task ID, should be unique
#define BLINK_ONBOARD_LED_SCHEDTASK_ID (0u)

// is of type SchedulerTask_Callback
void blink_onboard_led(void *param);

#endif /* INC_USERSCHEDULERTASK_BLINK_ONBOARD_LED_H_ */
