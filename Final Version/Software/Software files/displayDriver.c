/*
 * displayDriver.c
 *
 * Created: 26/08/2021 5:55:20 PM
 *  Author: Rachel N,  Shou M
 */ 
#include "displayDriver.h"
#include "timer.h"

#define SH_DS1 PORTC |= (1<<PC4)         // Sets SH_DS pin to 1
#define SH_DS0 PORTC &= ~(1<<PC4)        // Sets SH_DS pin to 0
#define SH_CPTOGGLE PORTC ^= (1<<PC3)    //SH_DS pin C4    data being transferred.
#define SH_STTOGGLE PORTC ^= (1<<PC5)    //SH_ST pin C5    loads all of it, on and off at the very end
#define SH_CPOFF PORTC &= ~(1<<PC3);	 // set SH_CP to logic 0
#define SH_STOFF PORTC &= ~(1<<PC5);	 // set SH_ST to logic 0

//all LED segment values for PortC
#define LED0   0b00111111
#define LED1   0b00000110
#define LED2   0b01011011
#define LED3   0b01001111
#define LED4   0b01100110
#define LED5   0b01101101
#define LED6   0b01111101
#define LED7   0b00000111
#define LED8   0b01111111
#define LED9   0b01101111
#define LEDP   0b01110011
#define LEDV   0b00111110
#define LEDA   0b01110111

// Enable and disable I/O pins for digit DS1,DS2,DS3 and DS4
#define DS1ON PORTD &= ~(1<<PD4)
#define DS2ON PORTD &= ~(1<<PD5)
#define DS3ON PORTD &= ~(1<<PD6)
#define DS4ON PORTD &= ~(1<<PD7)
#define DISPOFF PORTD|= (1<<PD4),PORTD|= (1<<PD5),PORTD|= (1<<PD6),PORTD|= (1<<PD7)
#define voltageDp 1
#define currentTempDp 4
#define powerDp 0
#define STARTUP 0


static volatile uint8_t disp_position = 0;
static volatile uint8_t binaryArray[8];
static volatile	uint8_t disp_characters[4];

void initDisplay(){
	DDRC |= (1<<PC3); // set SH_CP as output
	DDRC |= (1<<PC4); // set SH_DS as output
	DDRC |= (1<<PC5); // set SH_ST as output
	
	DDRD |= (1<<PD4); // Set DS1 as output
	DDRD |= (1<<PD5); // Set DS2 as output
	DDRD |= (1<<PD6); // Set DS3 as output
	DDRD |= (1<<PD7); // Set DS4 as output
	
}

void send_next_character_to_display(){
	// Get the binary segment values of the character that we want to display based on the display position
	convertDecimaltoBinary(disp_characters[disp_position]);
	SH_CPOFF; // set SH_CP to logic 0
	SH_STOFF; // set SH_ST to logic 0
	
	// Sending segment bit pattern to shift registers
	for(uint8_t i=0; i<8; i++){
	
		//Set the SH_DS pin to be either “0” or “1” as per the bit
		if(binaryArray[i] == 1){
			SH_DS1;
			}else{
			SH_DS0;
		}
		
		// SH_CP toggles to shift in bit to shift register
		SH_CPTOGGLE;
		SH_CPTOGGLE;
	}
	
	// Disable Digits
	DISPOFF;
	
	// Latch the output (l set the 8-bit pattern from the shift register to drive the segments)
	SH_STTOGGLE;
	SH_STTOGGLE;
	
	// Enable the corresponding digit position (i.e. set Ds1, Ds2, Ds3 and Ds4)
	if (disp_position ==0){
		DS1ON;
		
		}else if(disp_position == 1){
		DS2ON;
		
		}else if(disp_position == 2){
		DS3ON;
		
		}else if(disp_position == 3){
		DS4ON;
		
	}
	disp_position++; // increment display position for next function call
	
	// Reset disp_position after 3
	if(disp_position >3){
		disp_position = 0;
	}
}

void convertDecimaltoBinary(uint8_t num){
	uint8_t newNum = num;
	uint8_t position = 1;
	
	// Reset binary array
	for(uint8_t j =0; j<8;j++){
		binaryArray[j] = 0;
	}
	
	// calculate binary values and store them in an array (MSB on the left most element and LSB on right most)
	while (newNum > 0) {
		binaryArray[8-position] = newNum % 2;   // calculates the bit and loads it into the array in reverse order
		newNum = newNum / 2; // next number for iteration
		position++;
	}
}

void seperate_and_load_characters(float number, uint8_t decimal_pos){
	uint8_t seg_pattern[10] ={LED0,LED1,LED2,LED3,LED4,LED5,LED6,LED7,LED8,LED9};
	uint8_t thousands,hundreds,tens,ones,maxPos;
	uint8_t add_decimalPoint = 128;

	// Separate the numbers
	if (number == STARTUP){ // to show 0's on start up
		thousands = number/1000;
		hundreds = (number - thousands*1000)/100;
		tens = (number - thousands*1000 - hundreds*100)/10;
		ones = (number - thousands*1000 - hundreds*100 - tens*10);
		maxPos = 4;// to display 0 on all 4 segements
	
	}// To separate numbers of currentRMs, peak voltage or power
	else if(decimal_pos == voltageDp){ //Peak voltage number separation
		number = number*10; // Change to XYZ from XY.Z for easy separation
		
		thousands = number/100;
		hundreds = (number - thousands*100)/10;
		tens = (number - thousands*100 - hundreds*10);
		
		maxPos = 3; // to be able to display the numbers on 3 segments and the last segment has the unit
		disp_characters[3] = LEDV;  // Store the binary segment pattern for current unit A to use in DS4
		
		// Current RMS number separation
	}else if(decimal_pos == currentTempDp && number!=STARTUP ){ // Current is displaying in mA
		
		thousands = number/100;
		hundreds = (number - thousands*100)/10;
		tens = (number - thousands*100 - hundreds*10);
		maxPos = 3;
		disp_characters[3] = LEDA; // Store the binary segment pattern for current unit A to use in DS4
		
		// Power number separation
	} else if (decimal_pos == 0){
		number = number*100; // Change to XYZ from X.YZ for easy separation
		
		thousands = number/100;
		hundreds = (number - thousands*100)/10;
		tens = (number - thousands*100 - hundreds*10);
		maxPos = 3;
		disp_characters[3] = LEDP;
		
	}
	
	//Add to array
	uint8_t numberArray[4]={thousands,hundreds,tens,ones};
	
	// cycles through numbers array to find corresponding segment pattern
	for (int i=0;i<maxPos;i++){
		if(i==decimal_pos)
		{
			disp_characters[i] = seg_pattern[numberArray[i]]+add_decimalPoint; // Add corresponding segment pattern to display characters array for transmission
			// Add 128 because we want dp is on the 7th bit and we want to turn that on
			
			}else{
			//takes number from array and then finds its pattern.
			disp_characters[i]= seg_pattern[numberArray[i]];
		}
	}
	
}

