#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#include "main.h"

#define TIMER_TICK_DURATION_MS 1 // should be set to be equal to scheduler tick duration

typedef enum
{
	TIMER_FLAG_RESET = RESET, TIMER_FLAG_SET = SET,
} SoftwareTimer_FlagState;


typedef struct
{
	uint32_t timer_counter;
	SoftwareTimer_FlagState timer_flag;
} SoftwareTimer;

void SoftwareTimer_SetDurationTick(volatile SoftwareTimer *tm, uint32_t ms);
void SoftwareTimer_TickUpdate(volatile SoftwareTimer *tm);
uint8_t SoftwareTimer_IsSet(volatile SoftwareTimer *tm);

#endif /* INC_SOFTWARE_TIMER_H_ */
