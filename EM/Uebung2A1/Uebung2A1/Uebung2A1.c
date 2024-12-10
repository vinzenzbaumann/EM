/*
 * EM_Uebung1.c
 *
 * Created: 15.10.2024 13:36:35
 *  Author: koa45702
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>

uint8_t start_detected = 0, reset_detected = 0, overflow_detected = 0;
volatile uint8_t counter = 0;

ISR(INT0_vect)
{
    if(!start_detected)
    {
        start_detected=1;
        reset_detected=0;
    }
}

ISR(PCINT1_vect)
{
    if(!reset_detected)
    {
        reset_detected=1;
    }
}

ISR(PCINT0_vect)
{
    counter=0;
}

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


int main(void)
{
    // eingänge
    DDRD|=(1<<PORTD1)|(1<<PORTD3)|(1<<PORTD0);// setzt DDRD an der stelle dddx auf output
    DDRB|=(1<<PORTB0);



    // Pullups
    //PORTB|= (1<<PORTB0)|(1<<PORTB1);						//falls inport A3 A4
    PORTD|= (1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7)|(1<<PORTD2)|(1<<PORTD4);
    PORTC|= (1<<PORTC0);

    EICRA &= ~((1<<ISC01)|(1<<ISC00));
    EICRA |= ((1<<ISC01)|(1<<ISC00));
    EIMSK |= (1<<INT0);

    PCICR|=(1<<PCIE1)|(1<<PCIE0);
    PCMSK1 |= (1<<PCINT8);
    PCMSK0 |= (1<<PCINT0);

    sei();



    //uint8_t PINB0_prev=0, PINB1_prev=0, PINB0_curr=0, PINB1_curr=0;

    while(1)
    {
        if(counter == 79)
        {
            PORTB^=(1<<PORTB0);
        }
        if(reset_detected)
        {

        }
        else
        {
            if(start_detected)
            {
                counter=Set()*10;
                start_detected = 0;
            }
            if((counter%10)==0)
            {
                PORTD=actualize(counter/10,PORTD);
            }
            counter++;
        }

        //tasterabfrage A3 A4
        /*
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




        		PINB1_prev = PINB1_curr;
        		PINB0_prev = PINB0_curr; */

        _delay_ms(100);
    }
}
