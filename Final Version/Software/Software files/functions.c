/*
 * functions.c
 *
 * Created: 26/08/2021 5:46:10 PM
 *  Author: Shou M, Rachel N
 */ 
#include "functions.h"
#include "uart.h"

/////////////MACROS///////////////////
#define TOGGLE_LED PORTB ^= (1<<PB5)

////////////////////////////////////////

void separateVoltage(float voltage){ // Peak voltage is in format XY.Z
	uint8_t hundreds,tens,ones;
	uint16_t value;
	char voltNum[7] = {0};
	value = (uint16_t)(voltage*10);
	
	hundreds = value/100; //Ones ,x
	tens = ((value-(hundreds*100))/10);	//tenthss  ,y
	ones = (value%10);	//hundredths ,z
	
	// Add to character number array for transmission and changed to ascii
	voltNum[0]=hundreds+'0';
	voltNum[1]=tens+'0';
	voltNum[2]='.';
	voltNum[3]=ones+'0';
	voltNum[4]='V';
	// Transmit the character array of numbers
	transmitNum(voltNum);

}

void separateCurrent(uint16_t current){ // current RMS value is in format XYZ
	uint8_t hundreds,tens,ones;
	uint16_t value;
	char currentNum[7] = {0};
	value = current;
	
	hundreds = value/100; //Ones ,x
	tens = ((value-(hundreds*100))/10);	//tenthss  ,y
	ones = (value%10);	//hundredths ,z
	
	// Add to character number array for transmission and changed to ascii
	currentNum[0]=hundreds+'0';
	currentNum[1]=tens+'0';
	currentNum[2]=ones+'0';
	currentNum[3]='m';
	currentNum[4]='A';
	
	if (currentNum[0]=='0')
	{
		currentNum[0]=' ';
	}
	// Transmit the character array of numbers
	transmitNum(currentNum);

}

void separatePower(float num){ // Power value is in format X.YZ
	uint8_t hundreds,tens,ones;
	uint16_t value;
	
	char powerNum[7] = {0};
	
	value= (uint16_t)(num*100); //expand it to XYZ.
	
	hundreds = value/100; //Ones ,x
	tens =((value-(hundreds*100))/10);	//tenthss  ,y
	ones =(value%10);	//hundredths ,z
	
	// store in the character array for transmission
	powerNum[0]=hundreds+'0';
	powerNum[1]='.';
	powerNum[2]=tens+'0';
	powerNum[3]=ones+'0';
	powerNum[4]='W';
	
	transmitNum(powerNum);
	
}

 
void led_toggle() { // toggles LED for validation purposes
	TOGGLE_LED;
}