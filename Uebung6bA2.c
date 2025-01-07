/*
 * Uebung6bA2.c
 *
 * Created: 07.01.2025 13:22:38
 *  Author: koa45702
 */ 

#define BAUDRATE 9600
#define BAUD_CONST (((F_CPU/(BAUDRATE*16UL)))-1)
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#include "USART.h"
volatile int32_t analog_value = 0;
volatile uint16_t edge = 0;
volatile uint16_t f_edge = 0;
volatile uint16_t r_edge = 0;
volatile uint16_t curr_edge =0;
volatile uint16_t prev_r_edge =0;
volatile uint16_t time = 0;
volatile uint8_t duty = 5;


ISR(TIMER1_CAPT_vect){
	curr_edge = ICR1;
	if(edge == 0){
		r_edge =curr_edge;
		if(prev_r_edge != 0){
			time = r_edge -prev_r_edge;
		}
		prev_r_edge = curr_edge;
		TCCR1B &= ~(1<<ICES1);
		edge = 1;
	}
	else{
		f_edge = curr_edge;
		if(time != 0){
			duty = ((f_edge -r_edge)*100)/time;
		}
		TCCR1B |= (1<<ICES1);
		edge = 0;
	}
}

int main(void)
{
	char message[] = "ATMEGA gestartet";
	char message2[20];
	//inputcapture
	TCCR1B |= (1<< ICES1) | (1<< CS11) | (1 << CS10);
	TIMSK1 |= (1<<ICIE1);
	
	//Timer0 config
	TCCR0A |=(1<<COM0B1) |(1<<COM0B0) | (1<< WGM00) |(1<<WGM01);//
	TCCR0B |=(1<< WGM02) |(1<< CS01) | (1<< CS00);//
	OCR0A=250;
	
	//LED config
	DDRD|=( 1 << PD5 );
	

	//Setze Top Wert
	
	//ADCSRA config
	//MUX referenzspannung auf 5 V
	ADMUX|= (1<<REFS0);
	ADMUX&= ~(1<<REFS1);

	//auf port PC1 setzen
	ADMUX&=0xF0;
	ADMUX|= 1;
	
	//ADC enable
	ADCSRA |= (1<<ADEN);
	
	//Prescaler einstellen
	ADCSRA |= (1<<ADPS2)| (1<<ADPS1)|(1<<ADPS0);
	
	//enable USART
	USART_Init();
	for(int f=0;message[f]!='\0';f++)
	USART_Transmit(message[f]);
	USART_Transmit('\n');
	
	sei();
	_delay_ms(500);
	while(1){

		ADCSRA |= (1<<ADSC);
		
		while(ADCSRA&(1<<ADSC)){
			//USART_Transmit(':');
		}
		
		analog_value	=	ADC;
		OCR0B = (analog_value >> 2 );
		 
		/*sprintf(message2,"C:%ld   \r",(analog_value>>2));
		//itoa(analog_value, message2,10);
		for(int f=0;message2[f]!='\0';f++)
		USART_Transmit(message2[f]);
		USART_Transmit('\r');
		USART_Transmit('\n');*/
		
		sprintf(message2,"DUTY: %d   \r",duty);
		//itoa(analog_value, message2,10);
		for(int f=0;message2[f]!='\0';f++)
		USART_Transmit(message2[f]);
		USART_Transmit('\r');
		USART_Transmit('\n');
		
    }
}