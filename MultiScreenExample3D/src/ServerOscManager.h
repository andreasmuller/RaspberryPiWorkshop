#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
class ServerOscManager
{
	public:

		ServerOscManager();
		~ServerOscManager();

		void 			init( string _xmlSettingsPath );
	
		void 			init( string _serverSendHost,
							  int _serverSendPort,
							  int _serverReceivePort );

		void 			draw();
		void 			sendData( vector<int> _numbersInt, vector<float> _numbersFloat );
		bool			isInitialised() { return initialised; }

		void			_update(ofEventArgs &e);
		
	private:

		int 			getServerTime();
	
		ofxOscSender 	sender;
		ofxOscReceiver 	receiver;
	
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