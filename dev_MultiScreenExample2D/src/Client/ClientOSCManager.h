//
//  ClientOSCManager.h
//  MultiScreenCommonTiming
//
//  Created by Andreas Müller on 25/12/2012.
//
//

#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#include "CommonTime/CommonTimeOsc.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
class DataPacket
{
	public:
		
		vector<int> numbersInt;
		vector<float> numbersFloat;
};

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
class ClientOSCManager
{
	public:
		
		ClientOSCManager();
		~ClientOSCManager();
		
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
		
		CommonTimeOSC* getCommonTimeOscObj()
		{
			return &commonTimeOsc;
		}
	
		ofEvent<DataPacket>  newDataEvent;
	
	private:

		int					uniqueComputerID;

		CommonTimeOSC		commonTimeOsc;
	
		bool				haveSetupSender;
		ofxOscSender		sender;
	
		ofxOscReceiver		receiver;

};
