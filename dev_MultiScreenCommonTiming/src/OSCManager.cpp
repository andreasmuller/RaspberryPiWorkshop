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
	
	sender.setup( "localhost", 23456 );
	//receiver;
	
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
		
		//m.getAddress()
	}
	
}