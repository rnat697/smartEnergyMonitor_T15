/*
 * timer.c
 *
 * Created: 26/08/2021 5:55:53 PM
 *  Author: Rachel N,  Shou M
 */ 
#include "timer.h"
#include "functions.h"
#include "displayDriver.h"

#define F_CPU 2000000

//This ISR function is called when timer0 reaches compare value (every 0.1ms), compare flag is automatically cleared
ISR(TIMER0_COMPA_vect) {
	
//led_toggle();    to validate for report.
	
//adc auto triggers on timer0 interrupt.

}

//Sends the characters to display every 10ms
ISR(TIMER1_COMPA_vect){  
	led_toggle();
	send_next_character_to_display();
}

void timer0_init(){
	
	TCCR0A |= (1<<WGM01);    // set up mode to clear on compare match with OCR0A
	TCCR0B |= (1<<CS01);     // Set up prescaler of 8
	TIMSK0 |= (1<<OCIE0A);   // Enable interrupt for output compare match A
	OCR0A = 24;				// Loading OCR0A with 24 to get a period of 0.1ms for adc sampling
}
void timer1_init(){
	TCCR1B |= (1<<WGM12) | (1<<CS12);  // Set to CTC mode and prescaler of 256
	OCR1A = 77;				//  Loading OCR1A with 77 to get a period of 10ms for 7 segment refresh rate
	TIMSK1 |= (1<<OCIE1A); // enable interrupt for output compare match A
}



