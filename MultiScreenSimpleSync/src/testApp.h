#pragma once

#include <set>

#include "ofMain.h"
#include "ofxOsc.h"

#include "CommonTime/CommonTimeOsc.h"

#include "Client/ClientOSCManager.h"


// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
class testApp : public ofBaseApp
{

	public:

		void				setup();
		void				update();
		void				draw();
		
		void				keyPressed(int key);
		void				keyReleased(int key);
		void				mouseMoved(int x, int y );
		void				mouseDragged(int x, int y, int button);
		void				mousePressed(int x, int y, int button);
		void				mouseReleased(int x, int y, int button);
			
		bool				isServer;

		ClientOSCManager	client;

		CommonTimeOSC*		commonTimeOsc;
	
		ofTrueTypeFont		fontSmall;
};
