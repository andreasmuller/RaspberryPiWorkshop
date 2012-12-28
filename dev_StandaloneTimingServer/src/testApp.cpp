
#include "testApp.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::setup()
{
	ofBackground(40, 100, 40);

//	sender.setup(SEND_HOST, SEND_PORT);
	
	receiver.setup( RECEIVE_PORT );
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::update()
{
	// check for waiting messages
	while( receiver.hasWaitingMessages() )
	{
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
	
		if( m.getAddress() == "/ping" )
		{
			string tmpStr = "Ping, remote time: " + ofToString(m.getArgAsInt32(0)) +
			" computer: " + ofToString(m.getArgAsInt32(1)) +
			"  IP: " + m.getRemoteIp() +
			" Port: " + ofToString(m.getRemotePort());
			
			receivedMessageSubjects.push_back( tmpStr );
		}
		else
		{
			receivedMessageSubjects.push_back( ofGetTimestampString() + " " + m.getAddress() );
		}
	}
	
	int maxSavedSubjects = 20;
	if( receivedMessageSubjects.size() > maxSavedSubjects )
	{
		receivedMessageSubjects.erase( receivedMessageSubjects.begin(),receivedMessageSubjects.begin()+1 );
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::draw()
{
	// display instructions
#ifdef HEADLESS

#else
	string buf;
	buf = "Sending OSC messages to " + string(SEND_HOST) + ":" + ofToString(SEND_PORT) + "    Listening for OSC messages on port: " + ofToString(RECEIVE_PORT);
	ofDrawBitmapString(buf, 10, 20);
	
	
	for( unsigned int i = 0; i < receivedMessageSubjects.size(); i++ )
	{
		ofDrawBitmapString(receivedMessageSubjects.at(i), 10, 60 + (i * 20) );
	}
	
#endif

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::keyPressed(int key)
{
	if(key == 'a')
	{

	}
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::keyReleased(int key){

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::mouseMoved(int x, int y){
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::mouseDragged(int x, int y, int button){

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::mousePressed(int x, int y, int button){

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::mouseReleased(int x, int y, int button){


}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::windowResized(int w, int h){

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::gotMessage(ofMessage msg){

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::dragEvent(ofDragInfo dragInfo){

}

