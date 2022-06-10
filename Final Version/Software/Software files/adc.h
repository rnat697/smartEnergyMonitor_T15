/*
 * adc.h
 *
 * Created: 26/08/2021 5:52:00 PM
 *  Author: Rachel N,  Shou M
 */ 
#ifndef ADC_H_
#define ADC_H_
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

extern volatile uint16_t sampleNum;
extern volatile uint16_t adc0ValueArray[40];
extern volatile uint16_t adc1ValueArray[40];

void adc_init();
ISR(ADC_vect);
uint16_t adc_read(uint8_t channel);
uint16_t adc_convert_mv(uint16_t value);
void enableADC();

#endif