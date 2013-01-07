#pragma once

#include <set>

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

#include "CommonTime/CommonTimeOsc.h"

#include "Client/ClientOSCManager.h"
#include "Server/MasterServerOsc.h"

#include "Scenes/SceneManager.h"


// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
class testApp : public ofBaseApp
{

	public:

		void				setup();
		void				update();
		void				draw();
		
		void				keyPressed(int key);
		void				keyReleased(int key);
		void				mouseMoved(int x, int y );
		void				mouseDragged(int x, int y, int button);
		void				mousePressed(int x, int y, int button);
		void				mouseReleased(int x, int y, int button);
		void				windowResized(int w, int h);
		void				dragEvent(ofDragInfo dragInfo);
		void				gotMessage(ofMessage msg);
			
	
		bool				isServer;

		ClientOSCManager*	client;
		MasterServerOsc*	server;
	
		SceneManager		sceneManager;
	
		int					screenIndex;	// which screen are we?

		CommonTimeOSC*		commonTimeOsc;
	
		ofTrueTypeFont		fontSmall;
		ofTrueTypeFont		fontLarge;
};

/*

 This topic is a bit old but I have a solution that works quite well for me.
 Basically you wait for the first incoming message to be received and then you can get the address from the message using the built in function getRemoteIp().
 
 In your header file declare a boolean as follows,
 
 Code:
 view plaincopy to clipboardprint?
 bool connectedToHost;
 
 in your setup() function initialize your boolean,
 
 Code:
 view plaincopy to clipboardprint?
 connectedToHost = false;
 
 then in update(), when your reading your Osc message do something like this,
 

 while( receiver.hasWaitingMessages() )
 {
	 // get the next message
	 ofxOscMessage m;
	 receiver.getNextMessage( &m );
	 
	 // get host ip on first time through only (you can change this method if you want to handle multiple hosts)
	 if(!connectedToHost){
		 string hostAddress = m.getRemoteIp();
		 sender.setup(hostAddress, 9000);         // make sure 9000 is actually your host port
		 connectedToOscHost = true;
	 }
	 
	 // add the rest of your Osc awesomeness here...
 
 }
 
 Note: this could easily be extended to handling multiple Osc hosts since each will give a unique ip address so you can send a message back to the host you want based on your call to m.getRemoteIp();
 
 Hope this helps someone.
 
 Cheers!
*/