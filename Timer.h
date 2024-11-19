#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED
#include <avr/io.h>
#include <stdlib.h>
#include "stdbool.h"


typedef struct TimerType
{
	bool start;
	int passedTime;
	int duration;
	void (*callback)();
	uint16_t startingTime;
} TimerType;



void setTimer(uint8_t timer_index, int time, void (*cb)());

void startTimer(uint8_t timer_index, uint16_t currentTime);

void cancelTimer(uint8_t timer_index);

void timerCheck(uint16_t currentTime);

#endif /* TIMER_H_INCLUDED */