/*
 * Uebung6aA2.c
 *
 * Created: 17.12.2024 13:38:38
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

int main(void)
{
	char message[] = "ATMEGA gestartet";
	char message2[20];

	//ADCSRA config
	//MUX referenzspannung auf 5 V
	ADMUX|= (1<<REFS0);
	ADMUX&= ~(1<<REFS1);

	//auf port PC1 setzen
	ADMUX&=0xF0;
	ADMUX|= 1;
	
	//ADC enable
	ADCSRA |= (1<<ADEN);
	
	ADCSRA |= (1<<ADSC);
	
	//ADCSRA |= (1<<ADATE);
	//Prescaler einstellen
	ADCSRA |= (1<<ADPS2)| (1<<ADPS1)|(1<<ADPS0);
	/*ADCSRA &= ~(1<<ADPS1);
	ADCSRA &= ~(1<<ADPS0);*/
	
	//ADCSRB config
	
	//ADCSRB enable
	
	//enable USART
	UCSR0B |= (1<<RXCIE0);
	sei();
	
	USART_Init();
	for(int f=0;message[f]!='\0';f++)
	USART_Transmit(message[f]);
	USART_Transmit('\n');

	


	while(1){
		ADCSRA |= (1<<ADSC);
		while(ADCSRA&(1<<ADSC)){
			//USART_Transmit(':');
		}
		/*analog_value=ADC;
		analog_value = analog_value * 4888;
		analog_value = analog_value/1000;
		sprintf(message2,"C:%ld   \r",analog_value);
		//itoa(analog_value, message2,10);
		for(int f=0;message2[f]!='\0';f++)
			USART_Transmit(message2[f]);
		USART_Transmit('\r');
		USART_Transmit('\n');
*/
	//ref spannung wechseln auf 1,1 v
	ADMUX|= (1<<REFS0);
	ADMUX|= (1<<REFS1);
	_delay_ms(5);
	//wechseln analoger Port
	ADMUX&=0xF0;
	ADMUX|= 8;

	//messen temperatur
	ADCSRA |= (1<<ADSC);
	while(ADCSRA&(1<<ADSC)){
		//USART_Transmit(':');
	}
	analog_value=ADC;
	analog_value = analog_value * 1074;
	analog_value = analog_value/1000;
	analog_value = analog_value-288;
	sprintf(message2,"C:%ld   \r",analog_value);
	for(int f=0;message2[f]!='\0';f++)
	USART_Transmit(message2[f]);
	USART_Transmit('T');
	USART_Transmit('\r');
	USART_Transmit('\n');

	//ausgabe Temperatur // wechsel zurück auf ref spannung 5 v und 
	ADMUX|= (1<<REFS0);
	ADMUX&= ~(1<<REFS1);
	_delay_ms(5);
	//wechsell auf port 1
	ADMUX&=0xF0;
	ADMUX|= 1;
_delay_ms(200);
		
    }
}