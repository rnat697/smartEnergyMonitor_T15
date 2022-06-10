/*
 * signalAnalysis.h
 *
 * Created: 26/08/2021 5:54:16 PM
 *  Author: Rachel N,  Shou M
 */ 
 #include <avr/io.h>
 #include <stdio.h>
 #include <math.h>
 #include <stdint.h>

#ifndef SIGNALANALYSIS_H_
#define SIGNALANALYSIS_H_
float calculatePeakVoltage();
void convertADCtoVoltage();
uint32_t calculateRMSCurrent();
float calculatePower();

#endif