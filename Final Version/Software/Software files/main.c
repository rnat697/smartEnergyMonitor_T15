/*
 * UART Implementation.c
 *
 * Created: 22/08/2021 7:51:22 pm
 * Author : Shou M,Rachel N
 */ 

#define F_CPU 2000000UL
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "functions.h"
#include "adc.h"
#include "timer.h"
#include "displayDriver.h"
#include "signalAnalysis.h"
#include <util/delay.h>


char stringVoltage[] = {"Peak Voltage is: "};
char stringCurrent[]= {"RMS Current is: "};
char stringPower[]={"Power is: "};
float Vac, power;
uint16_t Irms;

//////////// MACROS ///////////
#define voltageDp 1
#define currentDp 4 
#define powerDp 0
#define DISABLEINTTIMER0	TIMSK0 &= ~(1 << OCIE0A);
#define ENABLEINTTIMER0		TIMSK0 |= (1<<OCIE0A);
#define SCROLLRATE 1000
#define BAUDRATE 9600
///////////////////////////////

int main(void){
	
	//Initialisations
	 DDRB |= (1<<PB5) ;    // Set led as output, validation for the ISR and timers.
	 DDRC = 0x00;		//sets all to input register
	 DDRD = 0x00;       // keeps register as input
	 
	 timer0_init(); // timer for ADC sampling
	 timer1_init(); // Timer for 7 Segement refresh rate
	 adc_init();	//initialize adc register
	 usart_init(BAUDRATE);
	 sei();	//turns interrupt on
	 
	
	//DISPLAY 
	initDisplay(); //initalise display
	
	 
	seperate_and_load_characters(0,4); // initial
	while(1){
		// Once all the sampling has been finished
		if(sampleNum == 80){
			
			DISABLEINTTIMER0;  // disable outputcompare
			// Convert ADC0 and ADC1 values to voltage signal values (in mV)
			convertADCtoVoltage();
			
			// calculates Peak Voltage (V), RMS current (mA) and Power (W)
			Vac = calculatePeakVoltage();
			Irms = calculateRMSCurrent();
			power = calculatePower();
			
			// transmit Voltage to terminal   //XY.Z
			transmitChars(stringVoltage);
			separateVoltage(Vac);
			
			// Transmit current RMS to terminal 	//XYZ
			transmitChars(stringCurrent);
			separateCurrent(Irms);
			
			
			// Transmit Power to terminal //X.YZ
			transmitChars(stringPower);
			separatePower(power);
			usart_transmit(0x0a); // new line \n
			usart_transmit(0x0d); // new line \r
			
			// Transmit peak Voltage, Current RMS and power to 7 segment display
			seperate_and_load_characters(Vac, voltageDp);
			_delay_ms(SCROLLRATE);
			seperate_and_load_characters(Irms, currentDp);
			_delay_ms(SCROLLRATE);
			seperate_and_load_characters(power, powerDp);
			_delay_ms(SCROLLRATE);
			
			// Start sampling again
			ENABLEINTTIMER0;	//Enable interrupt for timer0 output compare match
			// Enable ADC sampling again
			enableADC();
		}
		

	}
}
