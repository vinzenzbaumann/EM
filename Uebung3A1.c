/*
* EMUebung3A1.c
*
* Created: 15.10.2024 13:36:35
*  Author: koa45702
*/

#define EVENT_TIMER1SEC 0x01
#define EVENT_TIMER50MS 0x02
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "Timer.h"

volatile uint8_t start_detected = 0, reset_detected = 0, overflow_detected = 0;
volatile uint8_t PIND2_prev=1, PINC0_prev=1, PIND2_curr=0, PINC0_curr=0, counter = 1;

ISR(TIMER1_OVF_vect){
	static uint16_t timercount = 0;
	TCNT1 = 65536-20000;
	timercount++;

	if ((timercount%50)==0){
		
		timerCheck(timercount);

	}
}

//timer1 16 bit timer usen
uint8_t Set()
{

	uint8_t copy = 0;

	if(PIND&(1<<PIND5)) 					//Schalterabfrage Schalter 1
	{
		copy+=1;
	}
	if(PIND&(1<<PIND6)) 					//Schalterabfrage Schalter 2
	{
		copy+=2;
	}
	if(PIND&(1<<PIND7)) 					//Schalterabfrage Schalter 3
	{
		copy+=4;
	}
	return copy;
}


uint8_t actualize(uint8_t counter, uint8_t actualizer)
{


	actualizer &= ~0b00001011;//einzeln löschen besser

	if(counter & 1)
	{
		actualizer |= (1<<PORTD1);
	}
	if(counter & 2)
	{
		actualizer |= (1<<PORTD0);
	}
	if(counter & 4)
	{
		actualizer |= (1<<PORTD3);
	}
	return actualizer;
}

void tasterAbfrage(){

	if((PIND&(1<<PIND2))){					//Taster A3 gedrückt (High Pegel) start
		PIND2_curr = 1;
	}
	else {
		PIND2_curr = 0;
		if (PIND2_prev==1 && PIND2_curr==0){		//falls Taster nicht mehr gedrückt und vorheriger input = gedrückt
			
			reset_detected=0;
			start_detected=1;
		}
	}
	PIND2_prev = PIND2_curr;


	if((PINC&(1<<PINC0))){					//Taster A4 gedrückt (High Pegel) reset
		PINC0_curr = 1;
	}
	else {
		PINC0_curr = 0;

		if (PINC0_prev!=PINC0_curr){		//falls Taster nicht mehr gedrückt und vorheriger input = gedrückt
			reset_detected=1;
		}
	}

	PINC0_prev = PINC0_curr;
	
}

void Countdown(){
 static uint8_t counter = 1;

	if(reset_detected)
	{}

	else
	{
		if(start_detected)
		{
			counter=Set(); // dip schalter
			start_detected = 0;
		}
	
			
			PORTD = actualize(counter,PORTD);
	counter++;
			
		
	}
}


int main(void)
{
	// eingänge
	DDRD|=(1<<PORTD1)|(1<<PORTD3)|(1<<PORTD0);// setzt DDRD an der stelle dddx auf output
	DDRB|=(1<<PORTB0);

	//timer config
	TCNT1 = 65536-200;                             
	TCCR1B &= ~((1<<CS12)|(1<<CS11)|(1<<CS10));  // Löscht alle Prescaler-Bits für Timer 1
	TCCR1B |= (1<<CS10);          // Setzt den Prescaler auf 64 für Timer 1
	TIMSK1 |= (1<<TOIE1);                   // Aktiviert den Overflow-Interrupt für Timer 1

	// Pullups
	PORTD|= (1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7)|(1<<PORTD2)|(1<<PORTD4);
	PORTC|= (1<<PORTC0);
	sei();
	

	
	setTimer(0,1000,Countdown);
	setTimer(1,50,tasterAbfrage);
	startTimer(0,0);
	startTimer(1,0);

	while(1)
	{

	}
}