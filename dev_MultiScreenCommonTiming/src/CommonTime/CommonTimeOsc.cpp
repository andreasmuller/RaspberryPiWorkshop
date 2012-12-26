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
void CommonTimeOSC::init( ofxOscSender* _oscSender )
{
	baseInit();
	oscSender = _oscSender;
}



// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void CommonTimeOSC::sendPing()
{
	ofxOscMessage m;
	m.setAddress("/ping");
	m.addIntArg( ofGetElapsedTimeMillis() );
	
	oscSender->sendMessage( m );
}
