/*
 * EM_Uebung1.c
 *
 * Created: 15.10.2024 13:36:35
 *  Author: koa45702
 */ 

#define F_CPU 16000000UL
#include "stdbool.h"
#include <avr/io.h>
#include <util/delay.h>


uint8_t Set(){
	
	uint8_t copy = 0;
	
	if(PIND&(1<<PIND5)){					//Schalterabfrage Schalter 1
		copy+=1;
	} 
	if(PIND&(1<<PIND6)){					//Schalterabfrage Schalter 2
		copy+=2;
	}
	if(PIND&(1<<PIND7)){					//Schalterabfrage Schalter 3
		copy+=4;
	}
return copy;
}

 uint8_t actualize(uint8_t counter, uint8_t actualizer){
	 
	 actualizer &= ~0b00001110;//einzeln löschen besser
	 
	 if(counter & 1){
		 actualizer |= (1<<PORTD1);
	 }
	 if(counter & 2){
		  actualizer |= (1<<PORTD2);
	 }
	 if(counter & 4){
		 actualizer |= (1<<PORTD3);
	 }
	 return actualizer;
}

 
int main(void)
{
	// eingänge
	DDRD|=(1<<PORTD1)|(1<<PORTD3)|(1<<PORTD2);// setzt DDRD an der stelle dddx auf output
	
	
	
	// Pullups
	PORTB|= (1<<PORTB0)|(1<<PORTB1);
	PORTD|= (1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7);
	
	bool stop = false;
	uint8_t counter = 0;
	uint8_t PINB0_prev=0, PINB1_prev=0, PINB0_curr=0, PINB1_curr=0;
	
	while(1){
		
		if (stop){
			
		}
		else {
			if((counter%10)==0) {
			PORTD=actualize(counter/10,PORTD);
			}
			if(counter == 79){
				counter = 0;
			}
			else{
			counter++;
			}
		}
		
		//tasterabfrage A3
		
		if(PINB&(1<<PINB0)){					//Taster A3 gedrückt (High Pegel)
			PINB0_curr = 1;
		}
		 else {
			PINB0_curr = 0;
			if (PINB0_prev!=PINB0_curr){		//falls Taster nicht mehr gedrückt und vorheriger input = gedrückt
				stop = true;
			}	
		}
		
		
		
		if(PINB&(1<<PINB1)){					//Taster A4 gedrückt (High Pegel)
			PINB1_curr = 1;
		} else {
			PINB1_curr = 0;
			if (PINB1_prev!=PINB1_curr){		//falls Taster nicht mehr gedrückt und vorheriger input = gedrückt
				counter=Set()*10;
				stop = false;
			}
		}
		
		
		
		_delay_ms(100);
		PINB1_prev = PINB1_curr;
		PINB0_prev = PINB0_curr;
	}
}