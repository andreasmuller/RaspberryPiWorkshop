#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"
#include "ofxOpenCv.h"

//#define _USE_LIVE_VIDEO		// uncomment this to use a live camera otherwise, we'll use a movie file


class NodeData
{
	public:
	
		NodeData()
		{
			frameNum = -1;
		}
	
		int					frameNum;
		vector< ofPolyline > frameData;
};

typedef pair <int, NodeData*> screenIndexNodeDataPointerPair;
//typedef map<int, NodeData*>::iterator screenIndexNodeDataPointerIteratin;

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
class testApp : public ofBaseApp
{

	public:

		void				setup();
		void				update();
		void				draw();
	
		void				serverUpdate();
		void				serverDraw();

		void				initClientCV();
		void				clientUpdate();
		void				clientDraw();
	
		void				keyPressed(int key);
		void				keyReleased(int key);
		void				mouseMoved(int x, int y );
		void				mouseDragged(int x, int y, int button);
		void				mousePressed(int x, int y, int button);
		void				mouseReleased(int x, int y, int button);
		void				windowResized(int w, int h);
		void				dragEvent(ofDragInfo dragInfo);
		void				gotMessage(ofMessage msg);
		
		// Server stuff
		map<int, NodeData* > nodeData;
	
		int					serverLastSentHelloMessageMillis;
		int					serverMilliseBetweenHelloMessages;
	
		string				serverSendHost;
		int					serverSendPort;
		int					serverReceivePort;
	
		// Client stuff
		
		bool				clientCanSend;
	
#ifdef _USE_LIVE_VIDEO
	ofVideoGrabber			vidGrabber;
#else
	ofVideoPlayer			vidPlayer;
#endif

		ofxCvColorImage		colorImg;
		
		ofxCvGrayscaleImage grayImage;
		ofxCvGrayscaleImage grayBg;
		ofxCvGrayscaleImage grayDiff;
		
		ofxCvContourFinder 	contourFinder;
	
		int 				threshold;
		bool				bLearnBakground;
		int					videoWidth;
		int					videoHeight;
		
		// Common stuff
		bool				isServer;
		int					clientScreenIndex;	// which screen are we?
	
		ofxOscSender		sender;
		ofxOscReceiver		receiver;
		
		ofTrueTypeFont		fontSmall;
		ofTrueTypeFont		fontLarge;
	
		char				tmpStr[256];
};
