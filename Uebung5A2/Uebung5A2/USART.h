/*
 * USART.h
 *
 * Created: 03.12.2024 14:47:46
 *  Author: koa45702
 */ 


#ifndef USART_H_
#define USART_H_






void USART_Transmit(unsigned char data);
void USART_Init();
unsigned char USART_Receive();
unsigned char receive_datum ();



#endif /* USART_H_ */