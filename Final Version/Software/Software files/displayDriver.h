/*
 * displayDriver.h
 *
 * Created: 26/08/2021 5:55:32 PM
 *  Author: Rachel N,  Shou M
 */ 
#ifndef DISPLAYDRIVER_H_
#define DISPLAYDRIVER_H_
#include <avr/io.h>

void initDisplay();
void send_next_character_to_display();
void convertDecimaltoBinary(uint8_t num);
void seperate_and_load_characters(float number, uint8_t decimal_pos);

#endif