#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED
#include <avr/io.h>
#include <stdlib.h>
#include "stdbool.h"


typedef struct TimerType
{
	bool start;
	uint32_t passedTime;
	uint32_t duration;
	void (*callback)();
	uint32_t startingTime;
} TimerType;



void setTimer(uint8_t timer_index, uint32_t time, void (*cb)());

void startTimer(uint8_t timer_index, uint32_t currentTime);

void cancelTimer(uint8_t timer_index);

void timerCheck(uint32_t currentTime);

#endif /* TIMER_H_INCLUDED */