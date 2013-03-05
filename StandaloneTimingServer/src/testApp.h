#pragma once

#include "ofMain.h"
#include "MasterServerOsc.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
class testApp : public ofBaseApp {

	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);

		MasterServerOsc masterServerOsc;
};

