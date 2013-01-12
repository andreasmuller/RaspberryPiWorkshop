#pragma once

#include <set>

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

#include "CommonTime/CommonTimeOsc.h"

#include "Client/ClientOSCManager.h"
#include "Server/ServerOscManager.h"

#include "Particle.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
class testApp : public ofBaseApp
{

	public:

		void				setup();
		void				update();
		void				draw();
		
		void				newData( DataPacket& _packet );
	
		void				keyPressed(int key);
		void				keyReleased(int key);
		void				mouseMoved(int x, int y );
		void				mouseDragged(int x, int y, int button);
		void				mousePressed(int x, int y, int button);
		void				mouseReleased(int x, int y, int button);
		void				windowResized(int w, int h);
		void				dragEvent(ofDragInfo dragInfo);
		void				gotMessage(ofMessage msg);
			
		static bool 		shouldRemoveParticle(Particle &b);
	
		bool				isServer;

		CommonTimeOSC*		commonTimeOsc;	
	
		ClientOSCManager*	client;
		ServerOscManager*	server;
		
		int					screenIndex;	// which screen are we?
	
		vector<Particle>	particles;
		
		float				lastTimeAddedParticle;
	
		ofTrueTypeFont		fontSmall;
		ofTrueTypeFont		fontLarge;
};
