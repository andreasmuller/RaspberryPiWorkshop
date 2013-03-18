#pragma once

#include "ofMain.h"


class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		
		
	ofImage tex0Source;
	ofImage tex1Source;
	
	ofTexture tex0;
	ofTexture tex1;
	
	float drawWidth;
	float drawHeight;
	
	ofShader shader;
	ofFbo fbo;

};

