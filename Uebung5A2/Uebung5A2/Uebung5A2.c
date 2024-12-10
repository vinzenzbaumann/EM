/*
* EMUebung5A2.c
*
* Created: 10.12.2024 13:36:35
*  Author: koa45702
*/

#define EVENT_TIMER1SEC 0x01
#define EVENT_TIMER50MS 0x02
#define TIME_1000000
#define BAUDRATE 9600
#define BAUD_CONST (((F_CPU/(BAUDRATE*16UL)))-1)

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>

#include "Timer.h"
#include "USART.h"

volatile uint8_t start_detected = 0, reset_detected = 0, overflow_detected = 0;
volatile uint8_t PIND2_prev=1, PINC0_prev=1, PIND2_curr=0, PINC0_curr=0;
static volatile uint8_t counter = 1, starting_value = 0;
static volatile unsigned char set = '0';
uint8_t eeVariable EEMEM = 0;
volatile const char Meldung1[] PROGMEM ="Hier ATmega? \n 1. Stoppuhr Starten \n 2. Stoppuhr Stoppen \n 3. Startzeit Einstellen \n 4. Aktuell eingestellte Startezeit ausgeben \n\0";
volatile const char Meldung2[] PROGMEM = "Startzeit ist :\0";
volatile const char Meldung3[] PROGMEM = "Startzeit eingeben\0";

ISR(TIMER1_OVF_vect){
	static uint32_t timercount = 0;
	TCNT1 = 65334;// 65536-2;
	timercount++;
	timerCheck(timercount);
}

//timer1 16 bit timer usen



uint8_t actualize(uint8_t counter, uint8_t actualizer)
{


	actualizer &= ~0b00110010;//einzeln löschen besser

	if(counter & 1)
	{
		actualizer |= (1<<PORTC5);
	}
	if(counter & 2)
	{
		actualizer |= (1<<PORTC4);
	}
	if(counter & 4)
	{
		actualizer |= (1<<PORTC1);
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

	if(counter >= 8){
		counter = 0;
	}

	if(reset_detected)
	{}

	else
	{
		if(start_detected)
		{
			counter = 7-(set-'0');
			//starting_value = counter;
			starting_value = counter;
			eeprom_write_byte(&eeVariable, starting_value);
			start_detected = 0;
		}
		
		PORTC = actualize(counter,PORTC);
		counter++;
		
	}
}



void startStoppuhr(){
	reset_detected=0;
}
void stopStoppuhr(){
	reset_detected=1;

	USART_Transmit(8 - counter + '0');
}

void setStartingTime(){
	stopStoppuhr();
	for(int f=0;pgm_read_byte(&Meldung3[f])!='\0';f++)
	USART_Transmit(pgm_read_byte(&Meldung3[f]));


	set = receive_datum();
	for(int f=0;pgm_read_byte(&Meldung2[f])!='\0';f++)
	USART_Transmit(pgm_read_byte(&Meldung2[f]));
	USART_Transmit(set);

	start_detected = 1;
}

void actualTime(){
	USART_Transmit(set);
}


int main(void)
{
	// eingänge
	//DDRD|=(1<<PORTD6)|(1<<PORTD3)|(1<<PORTD5);// setzt DDRD an der stelle dddx auf output lEDS
	
	DDRC|= (1<<PORTC1)|(1<<PORTC4)|(1<<PORTC5);// Auf Output
	
	//DDRB|=(1<<PORTB0);

	//timer config
	TCNT1 = 65334;//65536-2;
	TCCR1B &= ~((1<<CS12)|(1<<CS11)|(1<<CS10));  // Löscht alle Prescaler-Bits für Timer 1
	TCCR1B |= (1<<CS11);          // Setzt den Prescaler auf 64 für Timer 1
	TIMSK1 |= (1<<TOIE1);                   // Aktiviert den Overflow-Interrupt für Timer 1

	// Pullups
	PORTD|= (1<<PORTD2);
	PORTC|= (1<<PORTC0);
	
	//PORTD|= |(1<<PORTD4);
	//PORTD |=(1<<PORTD7);
	//Enable Interuppt
	
	UCSR0B |= (1<<RXCIE0);
	sei();
	setTimer(0,10000,Countdown);
	//setTimer(1,500,tasterAbfrage);
	startTimer(0,0);
	//startTimer(1,0);
	//EEPROM
	
	starting_value = eeprom_read_byte(&eeVariable);
	set = (7-starting_value) + '0';
	
	USART_Init();
	for(int f=0;pgm_read_byte(&Meldung1[f])!='\0';f++)
	USART_Transmit(pgm_read_byte(&Meldung1[f]));
	
	while(1)
	{
		unsigned char datum;
		datum = receive_datum();
		USART_Transmit(datum);
		
		switch (datum)
		{
			case 'a': startStoppuhr();break;
			case 'b':	stopStoppuhr();break;
			case 'c':	setStartingTime();break;
			case 'd': actualTime();break;
			default: USART_Transmit('R');break;
		}


	}
	
	
}