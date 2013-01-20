#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

#include "CommonTimeOsc.h"

#include "ClientOSCManager.h"
#include "ServerOscManager.h"

#include "TiledCameraView.h"

#include "Particle.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
class testApp : public ofBaseApp
{

	public:

		void				setup();
		void				update();
		void				draw();
		
		void				setCamera( float _time );
	
		void				newData( DataPacket& _packet );

		ofMesh*				createGridMesh( float _sizeX, float _sizeZ, int _resX, int _resZ );
	
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
	
		float				cameraFov;
		TiledCameraView		tiledCameraView;
	
		ofMesh*				gridMesh;
		
		float				lastTimeAddedParticle;
	
		ofTrueTypeFont		fontSmall;
		ofTrueTypeFont		fontLarge;
};
