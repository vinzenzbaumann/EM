/*
 * CFile2.c
 *
 * Created: 03.12.2024 14:46:26
 *  Author: koa45702
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
 
// Hysterese
#define max_queue 21
#define min_queue 15
 
#define F_CPU 16000000UL
 
#define BAUDRATE 9600
#define BAUD_CONST (((F_CPU/(BAUDRATE*16UL)))-1)
 
 
#define BUFFER_SIZE 32
#define XON 0x11
#define XOFF 0x13
 
 
volatile uint8_t readbuffer[BUFFER_SIZE] = {0x0d};
volatile uint8_t write_index = 0;
volatile uint8_t read_index = 0;
volatile uint8_t data_count = 0;
volatile uint8_t xoff_sent = 0;
volatile uint8_t xon_sent = 0;
 
void USART_Transmit(unsigned char data){
	// Flusskontrolle hier
	while(!(UCSR0A & (1<<UDRE0))) ;
	UDR0 = data;
}
 
ISR(USART_RX_vect) {
	// Flusskontrolle
	if (data_count >= max_queue) {
		// nur einmal senden
		if (xoff_sent == 0) {
			USART_Transmit(XOFF);
			xoff_sent = 1;
		}
		xon_sent = 0;
	}
	//received data
	if (UCSR0A & (1<<RXC0)) {
		// read into circular buffer
		uint8_t datum = UDR0;
		readbuffer [write_index] = datum;
		data_count++;
		write_index = (write_index + 1) % BUFFER_SIZE; // overflow
	}
}
 
 
void USART_Init(){
	
	UBRR0H = (BAUD_CONST >> 8);
	UBRR0L = BAUD_CONST;
	
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
	
}
 
 
unsigned char USART_Receive(){
	if (data_count <= min_queue) {
		if (xon_sent == 0) {
			USART_Transmit(XON);
			xon_sent = 1;
		}
	}
	
	uint8_t data;
	if (data_count != 0) {
		data = readbuffer[read_index];
		read_index = (read_index + 1) % BUFFER_SIZE; // overflow 
		data_count--;
	}
	else {
		data = 0;
	}
	return data;
}
 
unsigned char receive_datum () {
	unsigned char datum = USART_Receive();
	while (datum == 0) {
		datum = USART_Receive();
	}
	return datum;
}