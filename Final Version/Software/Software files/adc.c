/*
 * adc.c
 *
 * Created: 26/08/2021 5:51:48 PM
 *  Author: Rachel N,  Shou M
 */ 

#include "adc.h"
#include "timer.h"

volatile uint16_t adc0ValueArray[40];
volatile uint16_t adc1ValueArray[40];
volatile uint16_t sampleNum = 0;
static volatile uint8_t channel = 0;
static volatile uint8_t nextposADC0=0;
static volatile uint8_t nextposADC1=0;



////////////////////MACROS////////////
#define CHANNEL0 0 // ADC0
#define CHANNEL1 1 // ADC1
#define DISABLEADC ADCSRA &= ~(1<<ADEN)|(1<<ADATE);
#define DISABLEADC_INTERRUPT ADCSRA &= ~(1<<ADIE);
#define MAXSAMPLES 80
#define RESET 0

///////////////
ISR(ADC_vect){
	// Alternate between the two ADC channels and get 40 samples each
	if (sampleNum<MAXSAMPLES){
		if(channel == CHANNEL0){ // When current conversion is using ADC0
			
			adc0ValueArray[nextposADC0] = ADC;  //reads adc
			channel = CHANNEL1;//stores channel for if condition.
			ADMUX &= 0xF0;  // clear channel selection
			ADMUX |= CHANNEL1;  //switch to channel 1
			nextposADC0++;
			
			}else{// When current conversion is using ADC1
			
			adc1ValueArray[nextposADC1] = ADC;   //reads adc
			
			channel = CHANNEL0;
			ADMUX &= 0xF0;  // clear channel selection
			ADMUX |= CHANNEL0;  //switch to channel 0
			nextposADC1++;
		}
		
		sampleNum++;
		
		}else{ // Once all samples has been taken
		nextposADC0 =0; //index to store sample adc0
		nextposADC1 =0; // index to store sample adc1
		DISABLEADC_INTERRUPT;
		DISABLEADC;
	}
}




void adc_init(){
	ADMUX |= (1<<REFS0); 
	ADMUX &= ~(1<<MUX0);     //AVCC set as reference, ADC0 selected and results are right adjusted
	ADCSRA |= (1<<ADEN) | (1<<ADPS1)| (1<<ADPS0) | (1<<ADATE);    //Enable ADC, prescaler to 8(i.e. 250kHz ) and enable auto trigger
	ADCSRB |= (1<<ADTS1)| (1<<ADTS0);    // auto trigger mode on timer/counter0 compare match A
	ADCSRA |= (1<<ADIE); // enable interrupt
	
	/*Start first conversion of ADC (to maintain 13 cycles in the following conversions)*/
	ADCSRA |= (1<<ADSC); // Starts ADC conversion		
	while((ADCSRA & (1<< ADIF))){  //First adc conversion is twice as long
		;
	}
	// Set next conversion to use ADC1
	ADMUX |=(1<<MUX0);    //select adc1
	channel = CHANNEL1;   // channel 1

}
void enableADC(){ // To start sampling again
	ADMUX |= (1<<REFS0);
	ADCSRA |= (1<<ADEN) | (1<<ADATE); //renable ADC and enable auto trigger
	ADCSRA |= (1<<ADIE); // enable interrupt
	sampleNum = RESET; 
	
}
