/*
 * functions.h
 *
 * Created: 14/08/2021 5:48:01 pm
 *  Author: Shou M, Rachel N
 */ 
 #include <avr/io.h>
 #include <stdio.h>

#ifndef UART_H_
#define UART_H_ 

void usart_init(uint16_t baudRate);
void usart_transmit(uint8_t byte);
void transmitChars(char string[]);
void transmitNum(char string[]);

#endif /* UART_H_ */