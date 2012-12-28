#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#define SEND_HOST "192.168.2.103"
#define SEND_PORT 7778

#define RECEIVE_PORT 7777

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
class testApp : public ofBaseApp {

	public:

		void setup();
		void update();
		void draw();

		int getServerTime();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofxOscSender sender;
		ofxOscReceiver receiver;
	
		vector< string > receivedMessageSubjects;
	
#ifdef HEADLESS
#else
		ofTrueTypeFont font;
#endif
	




};

