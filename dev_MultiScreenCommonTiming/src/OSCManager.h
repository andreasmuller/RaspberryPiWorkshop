//
//  OSCManager.h
//  MultiScreenCommonTiming
//
//  Created by Andreas Müller on 25/12/2012.
//
//

#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#include "CommonTime/CommonTimeOsc.h"


class OSCManager
{
	public:
		
		OSCManager();
		~OSCManager();
		
		void	init( int _uniqueComputerID );
	
		void	_update(ofEventArgs &e);
	
		ofxOscSender*	getOSCSender()
		{
			return &sender;
		}
		
		ofxOscReceiver* getOSCReceiver()
		{
			return &receiver;
		}
		
		CommonTimeOSC* getCommonTime()
		{
			return &commonTimeOsc;
		}
	
	private:

		int					uniqueComputerID;
	
		ofxOscSender		sender;
		ofxOscReceiver		receiver;
	
		CommonTimeOSC		commonTimeOsc;
};
