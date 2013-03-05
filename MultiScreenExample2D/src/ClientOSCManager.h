#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#include "CommonTimeOsc.h"

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
	
		inline ofxOscSender*	getOSCSender()
		{
			return &sender;
		}
		
		inline ofxOscReceiver* getOSCReceiver()
		{
			return &receiver;
		}
		
		inline CommonTimeOSC* getCommonTimeOscObj()
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
