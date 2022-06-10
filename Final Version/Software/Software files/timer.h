/*
 * timer.h
 *
 * Created: 26/08/2021 5:56:08 PM
 *  Author: Rachel N,  Shou M
 */ 
#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
void timer0_init();
void timer1_init();
ISR(TIMER0_COMPA_vect);

#endif