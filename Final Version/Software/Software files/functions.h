/*
 * functions.h
 *
 * Created: 26/08/2021 5:46:47 PM
 *  Author: Shou M, Rachel N
 */ 
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include <math.h>
#include <avr/io.h>
#include <stdio.h>

void separateVoltage(float voltage);
void separateCurrent(uint16_t current);
void separatePower(float num);

void convert_to_ascii(uint16_t voltage);

void led_toggle() ;
#endif /* FUNCTIONS_H_ */