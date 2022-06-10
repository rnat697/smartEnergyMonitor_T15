/*
 * uart.c
 *
 * Created: 22/08/2021 7:52:13 pm
 *  Author: Shou M, Rachel N
 */ 

#define F_CPU 2000000UL

 #include "adc.h"
 #include "uart.h"
 #include <avr/io.h>
 #include <stdio.h>
 #include <string.h>
 #include <math.h>
 
 void usart_init(uint16_t baudRate){
	UCSR0A |= (1<< UDRE0); // Initalises data register empty flag
	UCSR0B |= (1 << TXEN0); // Initialise transmitter
	UCSR0C &= !(1 << USBS0); // Initalises one stop bit
	UCSR0C |= (3 << UCSZ00); // Initalises 8 bit character size
	// uart is in asynchronous mode UMSEL0[1..0] = 00 in register UCSR0C
	
	UBRR0 = (F_CPU / ((uint32_t)baudRate*16))-1; // Initalises baud rate register
 }

 void usart_transmit(uint8_t  byte){
	 
	 while (((UCSR0A & (1 << UDRE0))== 0))
	 {       //UDRE0 bit is checked to see if it is 0
		;
	 }
	 UDR0 = byte;   
 }

 void transmitChars(char string[]){
	 // Goes through all the characters in the string array and transmits them using UART
 	for(int j =0; j<strlen(string);j++){
	 	usart_transmit(string[j]);
 	}
 }
 
 void transmitNum(char string[]){
	  // Goes through all the characters in the string array and transmits them using UART
	  for(int j =0; j<strlen(string);j++){
		  usart_transmit(string[j]);
	  }
	  
	  // Transmit new line
	   usart_transmit(0x0d);
	   usart_transmit(0x0a);
	  
}