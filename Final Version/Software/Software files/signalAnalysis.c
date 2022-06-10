/*
 * signalAnalysis.c
 *
 * Created: 26/08/2021 5:53:27 PM
 *  Author: Rachel N,  Shou M,	Francis C
 */ 
#include "signalAnalysis.h"
#include "adc.h"

 //////////////////////////////MACROS/////////////////////////
 #define OFFSET 2100
 #define VOLTAGEGAIN 1
 #define CURRENTGAIN 2.06
 #define SHUNT_RESISTOR 0.5
 #define SCALEPOWER 1000000
 #define POWERITERATIONS 79
 #define VOLTAGEDIVIDER 21.74 // (RA+RB)/RB = 58.7k/2.7k
 #define MILLI_TO_VOLTS 1000
 #define SAMPLE_NUMBER 40
 ///////////////////////////////////////////////////
 
static volatile int16_t peakVoltage=0; // for range -32768 to +32767
static volatile uint32_t rmsCurrent=0;
static volatile float power=0;
static volatile uint16_t voltageArray[40];
static volatile uint16_t currentArray[40];
 
 
void convertADCtoVoltage(){
	for(uint8_t i=0; i<40; i++){
		voltageArray[i] = adc_convert_mv(adc0ValueArray[i]);  //voltage array
		currentArray[i] = adc_convert_mv(adc1ValueArray[i]);
	}
}

uint16_t adc_convert_mv(uint16_t value){
	double voltage = 0;
	double oneAdcCount = 4.8828125; //Voltage step = Vref/2^N = 5/2^10 =4.88mV
	voltage =  value * oneAdcCount;
	return (uint16_t)voltage;
}


float calculatePeakVoltage(){
	// Calculating Vac using voltage signals from ADC0 in mV
	int16_t voltage;
	for(uint8_t i=0; i<40; i++){
		
		voltage = ((((float)voltageArray[i]-OFFSET)/VOLTAGEGAIN)*VOLTAGEDIVIDER);
		
		// Find the peak voltage from the array after calculating Vac
		if (peakVoltage<voltage)
		{
			peakVoltage = voltage;
		}
	}
	// Return the voltage value in V not mV
	return ((float)peakVoltage/MILLI_TO_VOLTS);
}

uint32_t calculateRMSCurrent(){
	float currentSample;
	//rms current is the mean squared^2 added together squarerooted and divided by the number of samples taken
	
	for(uint8_t i=0; i<40; i++){
		// Calculating Iac from ADC0 current signals
		currentSample = (float)currentArray[i]-OFFSET;
		currentSample = (currentSample/CURRENTGAIN)/SHUNT_RESISTOR;
		
		if (i==0){ // squares first sample and defines it as return variable.
			rmsCurrent= pow(currentSample,2);
		}
		
		else{
			rmsCurrent = rmsCurrent + (int32_t) pow(currentSample,2);  // adds the next adc sample to the rmscurrent.
		}
	}
	
	rmsCurrent = (int32_t) sqrt(rmsCurrent/SAMPLE_NUMBER);    //sqrt number then divide by sample.
	
	return rmsCurrent;
}

float calculatePower(){
	float voltage;
	float voltage_hat;
	float current;
	float current_hat;
	float powerSample;
	
	
	for (int i=0;i<40;i++)   // first iteration
	{	// calculating Vac and Iac
		voltage = (float)voltageArray[i]-OFFSET;	// voltage from ac source,
		voltage = (voltage/VOLTAGEGAIN)*VOLTAGEDIVIDER;		//divide by the opamp gain and voltage divider across 58k/2.7k
		
		current = (float)currentArray[i]-OFFSET;		//current from ac source
		current = ((current/CURRENTGAIN)/SHUNT_RESISTOR);	//adc can only measure voltage so to get current, voltage/resistor.
		
		powerSample = (float)(current*voltage)/SCALEPOWER;  // from micro to Watts.
		
		if (i==0)   //resets power and starts chain of x * x+1 /2 which is the average across two power values.
		{
			power = (float)(voltage*current)/SCALEPOWER;   //where if x is 0 average power is just same value /2 = samevalue
		}
		
		else{
			//handles average power without linear.
			power=power+powerSample;
		}

		if (i<39)  //goes odd,handles linear power.
		{
			//linearly approx of power between 0,1
			voltage_hat = (float)(voltageArray[i] + voltageArray[i+1])/2  -  OFFSET;	//gets data point between two voltages,ac source
			voltage_hat = ((voltage_hat/VOLTAGEGAIN)*VOLTAGEDIVIDER);	
			
			current_hat = (float)(currentArray[i] + currentArray[i+1])/2 - OFFSET;	//datapoint between two current.
			current_hat = (current_hat/CURRENTGAIN)/SHUNT_RESISTOR;
			
			powerSample = (float)(voltage*current_hat + voltage_hat*current)/SCALEPOWER;
			power= (float)power+powerSample;
		}

	}
	
	power= power/POWERITERATIONS ;  //sample number + linear approximation number
	return power;
}