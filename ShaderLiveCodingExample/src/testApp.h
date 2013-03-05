#pragma once

#include "ofMain.h"
#include "ofxAutoReloadedShader.h"

class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);

		ofxAutoReloadedShader autoShader;
		ofFbo fbo;

		ofImage image;

		ofTrueTypeFont 	font;

		char tempStr[512];
};

