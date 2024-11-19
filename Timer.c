#include "Timer.h"

volatile TimerType timers[5];

void setTimer(uint8_t timer_index,uint32_t time,void (*cb)())
{
	timers[timer_index].start = 0;
	timers[timer_index].duration = time;
	timers[timer_index].callback= cb; 
	timers[timer_index].passedTime = 0;
}

void startTimer(uint8_t timer_index,uint32_t currentTime)
{
	timers[timer_index].startingTime = currentTime;
	timers[timer_index].start = 1;
}

void cancelTimer(uint8_t timer_index)
{
	timers[timer_index].start = 0;
}

void timerCheck(uint32_t currentTime)
{

		for(int i = 0; i<4; i++)
		{
			if (timers [i].start)
			{
				timers[i].passedTime = currentTime - timers[i].startingTime;
				if (timers[i].passedTime >= timers[i].duration)
				{
					timers [i].callback();
					timers [i].start=0;
					startTimer(i, currentTime);
				}
			}
		}
	
}
