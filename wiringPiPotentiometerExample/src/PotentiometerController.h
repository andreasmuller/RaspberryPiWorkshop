#pragma once

#include "ofMain.h"
#include "wiringPi.h"


class PotentiometerController: public ofThread
{
public:
	PotentiometerController();
	bool setup();
	void threadedFunction();
	
	int CLOCK_PIN;
	int MISO_PIN;
	int MOSI_PIN;
	int CS_PIN;
	
	int readAnalogDigitalConvertor();
	int potentiometerInput;
	
	
	int lastPotValue;
	int potValue;
	int changeAmount;
	bool didPotChange;
	
	bool doToleranceCheck;
	int tolerance;
	
	bool isReady;
	
};