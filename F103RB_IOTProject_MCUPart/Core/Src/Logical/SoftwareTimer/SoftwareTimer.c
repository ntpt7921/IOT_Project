#include <Logical/SoftwareTimer/SoftwareTimer.h>

// since timer will be update every tick duration, it can only register
// event with the smallest time scale of tick duration (in this case TIMER_TICK_DURATION_MS)

void SoftwareTimer_SetDurationTick(volatile SoftwareTimer *tm, uint32_t ms)
{
	tm->timer_counter = (ms / TIMER_TICK_DURATION_MS);
	tm->timer_flag = TIMER_FLAG_RESET;
}

void SoftwareTimer_TickUpdate(volatile SoftwareTimer *tm)
{
	if (tm->timer_counter > 0)
	{
		tm->timer_counter--;
	}

	if (tm->timer_counter == 0)
	{
		tm->timer_flag = TIMER_FLAG_SET;
	}
}

uint8_t SoftwareTimer_IsSet(volatile SoftwareTimer *tm)
{
	return (tm->timer_flag == TIMER_FLAG_SET);
}
