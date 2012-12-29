//
//  OSCManager.cpp
//  MultiScreenCommonTiming
//
//  Created by Andreas Müller on 25/12/2012.
//
//

#include "OSCManager.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
OSCManager::OSCManager()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
OSCManager::~OSCManager()
{
	ofRemoveListener(ofEvents().update, this, &OSCManager::_update);	
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void OSCManager::init( int _uniqueComputerID )
{
	uniqueComputerID = _uniqueComputerID;
	
	sender.setup( "192.168.3.2", 7777 );
	receiver.setup( 7778 );
	
	commonTimeOsc.init( &sender, uniqueComputerID );
	
	ofAddListener(ofEvents().update, this, &OSCManager::_update );
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void OSCManager::_update(ofEventArgs &e)
{
	// check for waiting messages
	while( receiver.hasWaitingMessages() )
	{
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		
		if( m.getAddress() == "/pong" )
		{
			//cout << "Received Pong, ID: " << m.getArgAsInt32(0) << "  serverTime: " << m.getArgAsInt32(1) << "  origTimeStamp: " << m.getArgAsInt32(2) << endl;
			
			// pong messages come in as: /pong, receiving computer ID (int), server timestamp (int), timestamp when remote computer sent (int)
			
			// if the message is for us
			if( m.getArgAsInt32(0) == uniqueComputerID )
			{
				int serverTime = m.getArgAsInt32(1);
				int originalTimeStamp = m.getArgAsInt32(2);
				
				commonTimeOsc.newReading( serverTime, commonTimeOsc.getInternalTimeMillis() - originalTimeStamp );
			}
		}
	}
	
}