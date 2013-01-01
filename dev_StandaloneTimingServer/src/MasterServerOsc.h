#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#define SEND_HOST "192.168.1.255"
#define SEND_PORT 7778

#define RECEIVE_PORT 7777

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
class MasterServerOsc
{
	public:

		MasterServerOsc();
		~MasterServerOsc();		

		void 		init();

		void		_update(ofEventArgs &e);
		void 		draw();

		int 		getServerTime();

		void 		sendChangeScene( int _index );

	private:

		ofxOscSender 	sender;
		ofxOscReceiver receiver;
	
		vector< string > receivedMessageSubjects;
	
#ifdef HEADLESS
#else
		ofTrueTypeFont font;
#endif
	
		int 			lastSentHelloMessageMillis;
		int 			milliseBetweenHelloMessages;
};