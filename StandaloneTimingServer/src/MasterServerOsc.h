#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
class MasterServerOsc
{
	public:

		MasterServerOsc();
		~MasterServerOsc();		

		void 		init( string _xmlSettingsPath = "" );
	
		void 		init( string _serverSendHost, int _serverSendPort, int _serverReceivePort );

		void		_update(ofEventArgs &e);
		void 		draw();

		int 		getServerTime();
	
		bool		isInitialised() { return initialised; }
	
	private:

		ofxOscSender 	sender;
		ofxOscReceiver receiver;
	
		vector< string > receivedMessageSubjects;
	
#ifdef HEADLESS
#else
		ofTrueTypeFont font;
#endif
	
		string			serverSendHost;
		int				serverSendPort;
		int				serverReceivePort;
	
		int 			lastSentHelloMessageMillis;
		int 			milliseBetweenHelloMessages;
	
		bool			initialised;
};