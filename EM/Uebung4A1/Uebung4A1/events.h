/*
 * events.h
 *
 * Created: 03.03.2020 09:04:14
 *  Author: mea39511
 */ 


#ifndef EVENT_H_
#define EVENT_H_

#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/common.h>


typedef uint16_t EventType;

volatile EventType events=0;

void setEvent(EventType ev) {
	unsigned char sreg_old = SREG;
	cli();
	events |= ev;
	SREG = sreg_old;
}

void clearEvent(EventType ev) {
	unsigned char sreg_old = SREG;
	cli();
	events &= ~ev;
	SREG = sreg_old;
}

int eventIsSet(EventType ev){
	return (events&ev);
}


#endif /* EVENT_H_ */