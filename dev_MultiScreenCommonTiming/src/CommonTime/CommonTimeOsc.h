//
//  CommonTimeOsc.h
//  MultiScreenCommonTiming
//
//  Created by Andreas Müller on 26/12/2012.
//
//

#pragma once

#include "CommonTimeBase.h"
#include "ofxOsc.h"


// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
class CommonTimeOSC : public CommonTimeBase
{
	public:
		
		CommonTimeOSC();
		~CommonTimeOSC();
	
		void	init( ofxOscSender* _oscSender );
		
	private:
	
		void	sendPing();
	
		ofxOscSender* oscSender;

};