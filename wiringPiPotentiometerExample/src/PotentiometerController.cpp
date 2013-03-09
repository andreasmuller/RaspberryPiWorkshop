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
	int status = wiringPiSetup();
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
		
		pinMode(MOSI_PIN, OUTPUT);
		pinMode(MISO_PIN, INPUT);
		pinMode(CLOCK_PIN, OUTPUT);
		pinMode(CS_PIN, OUTPUT);
		
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
		changeAmount = potValue - lastPotValue;
		
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
		sleep(500);
	}
}

int  PotentiometerController::readAnalogDigitalConvertor()
{
	
	digitalWrite(CS_PIN, true);
	digitalWrite(CLOCK_PIN, false);  // start clock low
	digitalWrite(CS_PIN, false);     // bring CS low
	
	int commandout = potentiometerInput;
	// start bit + single-ended bit
	
	commandout |= 0x18;  
	commandout <<= 3;
	
	// we only need to send 5 bits here
	for (int i=0; i<5; i++) 
	{
		if (commandout & 0x80)
		{
			digitalWrite(MOSI_PIN, true);
		} 
		else
		{
			digitalWrite(MOSI_PIN, false);
		}
		commandout <<= 1;
		
		digitalWrite(CLOCK_PIN, true);
		digitalWrite(CLOCK_PIN, false);
	}
	
	
	int analogDigitalConvertorValue = 0;
	
	//read in one empty bit, one null bit and 10 ADC bits
	for (int i=0; i<10; i++) 
	{
		digitalWrite(CLOCK_PIN, true);
		digitalWrite(CLOCK_PIN, false);
		analogDigitalConvertorValue <<= 1;
		if (digitalRead(MISO_PIN))
		{
			analogDigitalConvertorValue |= 0x1;
		}
	}
	
	digitalWrite(CS_PIN, true);
	
	// first bit is 'null' so drop it
	analogDigitalConvertorValue >>= 1;   
	
	return analogDigitalConvertorValue;
}
