#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp
{
	public:

		void setup();
		void update();
		void draw();
	
		string message;
		ofTrueTypeFont font;
		
		ofFbo* frontBuffer;
		ofFbo* backBuffer;
		ofMesh screenMesh;
};
