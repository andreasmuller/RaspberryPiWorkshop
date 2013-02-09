#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

#include "CommonTimeOsc.h"

#include "ClientOSCManager.h"
#include "ServerOscManager.h"

#include "TiledCameraView.h"

#include "SyncedAnimationObject.h"
#include "WanderingStreamer.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
class testApp : public ofBaseApp
{

	public:

		void				setup();
		void				update();
		void				draw();
	
		void				drawScene( float _time );
		
		void				setCamera( float _time, int _screenIndex );
	
		void				createNewObject( float _currTime );
	
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
			
		static bool 		shouldRemoveObject(SyncedAnimationObject *b);
	
		bool				isServer;

		CommonTimeOSC*		commonTimeOsc;	
	
		ClientOSCManager*	client;
		ServerOscManager*	server;
		
		//vector<SyncedAnimationObject>	sceneObjects;
		vector<SyncedAnimationObject*>	sceneObjects;
	
		int					screenIndex;	// which screen are we?
	
		float				cameraFov;
		TiledCameraView		tiledCameraView;
		bool				viewTile;
	
		vector< ofFbo* >	screens; // debugging only
	
		ofMesh*				gridMesh;
		
		float				lastTimeAddedObject;
	
		ofTrueTypeFont		fontSmall;
		ofTrueTypeFont		fontLarge;
	
		char				tmpStr[256];
};
