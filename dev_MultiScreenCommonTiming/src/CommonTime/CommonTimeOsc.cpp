//
//  CommonTimeOsc.cpp
//  MultiScreenCommonTiming
//
//  Created by Andreas Müller on 26/12/2012.
//
//

#include "CommonTimeOsc.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
CommonTimeOSC::CommonTimeOSC()
{

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
CommonTimeOSC::~CommonTimeOSC()
{
}


// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void CommonTimeOSC::init( ofxOscSender* _oscSender, int _uniqueComputerID  )
{
	baseInit();
	uniqueComputerID = _uniqueComputerID;
	oscSender = _oscSender;
}



// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void CommonTimeOSC::sendPing()
{
	cout << "CommonTimeOSC::sendPing()" << endl;
	
	ofxOscMessage m;
	m.setAddress("/ping");
	m.addIntArg( ofGetElapsedTimeMillis() );
	m.addIntArg( uniqueComputerID );	
	
	oscSender->sendMessage( m );
}
