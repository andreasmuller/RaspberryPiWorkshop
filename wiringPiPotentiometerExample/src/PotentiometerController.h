#pragma once

#include "ofMain.h"
#include "wiringPiSPI.h"


class PotentiometerController: public ofThread
{
public:
	PotentiometerController();
	bool setup();
	void threadedFunction();
	

	
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