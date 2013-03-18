/*
 *  PotentiometerController.cpp
 *
 *  Created by jason van cleave on 3/9/13.
 *  translation of http://learn.adafruit.com/reading-a-analog-in-and-controlling-audio-volume-with-the-raspberry-pi/connecting-the-cobbler-to-a-mcp3008
 *
 */

#include "PotentiometerController.h"

PotentiometerController::PotentiometerController()
{
	isReady = false;
	changeAmount = 0;
	lastPotValue = 0;
	potValue = 0;
	doToleranceCheck = false;
}


bool PotentiometerController::setup()
{
	int status = wiringPiSPISetup(0, 1000000);
	if (status != -1)
	{
		ofLogVerbose() << "wiringPiSetup PASS";
		
		//		GPIO values
		//		CLOCK_PIN = 18;
		//		MISO_PIN = 23;
		//		MOSI_PIN = 24;
		//		CS_PIN = 25;
		
		//wiringPI translations
		// https://projects.drogon.net/raspberry-pi/wiringpi/pins/
		CLOCK_PIN = 1;
		MISO_PIN = 4;
		MOSI_PIN = 5;
		CS_PIN = 6;
		
		/*pinMode(MOSI_PIN, OUTPUT);
		pinMode(MISO_PIN, INPUT);
		pinMode(CLOCK_PIN, OUTPUT);
		pinMode(CS_PIN, OUTPUT);*/
		
		//10k trim pot connected to adc #0
		potentiometerInput = 0;
		
		lastPotValue = 0;    
		/*
		 this keeps track of the last potentiometer value
		 to keep from being jittery we'll only change
		 volume when the pot has moved more than 5 'counts'
		 */
		tolerance = 5;	
		
		didPotChange = false;
		isReady = true;
	}else 
	{
		ofLogError() << "wiringPiSetup FAIL status: " << status;
	}
	if (isReady) 
	{
		startThread(false, true);
	}
	return isReady;
}


void PotentiometerController::threadedFunction()
{
	while (isThreadRunning()) 
	{
		didPotChange = false;
		
		potValue = readAnalogDigitalConvertor();
		changeAmount = abs(potValue - lastPotValue);
		
		if (doToleranceCheck) 
		{
			if ( changeAmount > tolerance )
			{
				didPotChange = true;
			}
		}else 
		{
			if(changeAmount!=0)
			{
				didPotChange = true;
			}
		}
		lastPotValue = potValue;
		
		ofLogVerbose() << potValue;
		sleep(10);
	}
}

int  PotentiometerController::readAnalogDigitalConvertor()
{
	uint8_t buff[3];
	int adc;
	potentiometerInput = 0;
	buff[0] = 1;
	buff[1] = (8+potentiometerInput)<<4;
	buff[2] = 0;
	wiringPiSPIDataRW(0, buff, 3);
	adc = ((buff[1]&3) << 8) + buff[2];
	return adc;
}
