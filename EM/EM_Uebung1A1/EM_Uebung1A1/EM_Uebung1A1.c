/*
 * EM_Uebung1.c
 *
 * Created: 15.10.2024 13:36:35
 *  Author: koa45702
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

 uint8_t actualize(uint8_t counter, uint8_t actualizer){
	 
	 switch (counter) {
		 
		 case 0:
		 printf("Fall 0: Counter ist 0.\n");
		 actualizer^= (1<<PORTD1);
		 break;
		 
		 case 1:
		 printf("Fall 1: Counter ist 1.\n");
		 actualizer ^= (1<<PORTD1);
		 actualizer ^= (1<<PORTD2);
		 break;
		 
		 case 2:
		 printf("Fall 2: Counter ist 2.\n");
		 actualizer ^= (1<<PORTD1);
		 break;
		 
		 case 3:
		 printf("Fall 3: Counter ist 3.\n");
		 actualizer ^= (1<<PORTD1);
		 actualizer ^= (1<<PORTD2);
		 actualizer ^= (1<<PORTD3);
		 break;
		 
		 case 4:
		 printf("Fall 4: Counter ist 4.\n");
		 actualizer ^= (1<<PORTD1);
		 break;
		 
		 case 5:
		 printf("Fall 5: Counter ist 5.\n");
		 actualizer ^= (1<<PORTD1);
		 actualizer ^= (1<<PORTD2);
		 break;
		 
		 case 6:
		 printf("Fall 6: Counter ist 6.\n");
		 actualizer ^= (1<<PORTD1);
		 break;
		 
		 case 7:
		 printf("Fall 7: Counter ist 7.\n");
		 actualizer = ~((1<<PORTD1)|(1<<PORTD2)|(1<<PORTD3));
		 break;
		 
		 default:
		 //printf("Ungültiger Wert! Der Counter muss zwischen 0 und 7 liegen.\n");
		 break;
	 }
	 return actualizer;
}

 
int main(void)
{
	DDRD|=(1<<PORTD1)|(1<<PORTD3)|(1<<PORTD2);// setzt DDRD an der stelle dddx auf output
	
	uint8_t counter = 7;
	
	
    while(1)
    {
		PORTD=actualize(counter, PORTD);
		_delay_ms(1000);
		
		if(counter == 0){
			counter = 7;
		}
		else{
		counter--;
		}
		
		
		
		
		
    }
}