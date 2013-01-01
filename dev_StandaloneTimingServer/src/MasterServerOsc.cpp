

#include "MasterServerOsc.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
MasterServerOsc::MasterServerOsc()
{

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
MasterServerOsc::~MasterServerOsc()
{

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void MasterServerOsc::init()
{

	sender.setup(SEND_HOST, SEND_PORT);
	
	receiver.setup( RECEIVE_PORT );

	lastSentHelloMessageMillis = 0;
	milliseBetweenHelloMessages = 3 * 1000;


	ofAddListener(ofEvents().update, this, &MasterServerOsc::_update );
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void MasterServerOsc::_update(ofEventArgs &e)
{

	// periodically send out a hello message on the multicast address, this way anyone joining us 
	// can get the address to the server if they are on the same network and listening to the right port
	if( (getServerTime() - lastSentHelloMessageMillis) > milliseBetweenHelloMessages )
	{
		ofxOscMessage m;
		m.setAddress("/hello");
		m.addIntArg( RECEIVE_PORT ); // add the port we would like to use to receive messages as an argument, seems more flexible than having a rule like remote port + 1
		sender.sendMessage( m );
	}

	// check for waiting messages
	while( receiver.hasWaitingMessages() )
	{
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
	
		if( m.getAddress() == "/ping" )
		{
			string tmpStr = "Ping, remote time: " + ofToString(m.getArgAsInt32(1)) +
			" computer: " + ofToString(m.getArgAsInt32(0)) +
			"  IP: " + m.getRemoteIp() +
			" Port: " + ofToString(m.getRemotePort());
			
			receivedMessageSubjects.push_back( tmpStr );

			// we need to send a "pong" message back, either we send this over the multicasting address, 
			// or we create a sender for each new address and port that send us a message, I'm going to 
			// try the multicasting route first.

			// their message comes in as /ping, their ID (int), their timestamp (int)

			int remoteComputerID 	= m.getArgAsInt32(0);			
			int remoteComputerTime 	= m.getArgAsInt32(1);

			ofxOscMessage m;
			m.setAddress("/pong");
			m.addIntArg( remoteComputerID );			// their ID			
			m.addIntArg( getServerTime() ); 	// my time
			m.addIntArg( remoteComputerTime );			// their time				

			sender.sendMessage( m );
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
void MasterServerOsc::draw()
{
	string buf;
	buf = "Sending OSC messages to " + string(SEND_HOST) + ":" + ofToString(SEND_PORT) + "    Listening for OSC messages on port: " + ofToString(RECEIVE_PORT);

#ifdef HEADLESS

#ifdef TARGET_LINUX
	std::system( "clear" );
#endif

	cout << "*********************************************************************************" << endl;
	cout << buf << endl;
	for( unsigned int i = 0; i < receivedMessageSubjects.size(); i++ )
	{
		 cout << "    " << receivedMessageSubjects.at(i) << endl;
	}
	cout << "*********************************************************************************" << endl;
#else

	ofDrawBitmapString(buf, 10, 20);
	
	
	for( unsigned int i = 0; i < receivedMessageSubjects.size(); i++ )
	{
		ofDrawBitmapString(receivedMessageSubjects.at(i), 10, 60 + (i * 20) );
	}
	
#endif
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void MasterServerOsc::sendChangeScene( int _index )
{
	ofxOscMessage m;
	m.setAddress("/scene_index");
	m.addIntArg( _index );		
	
	sender.sendMessage( m );
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
int MasterServerOsc::getServerTime()
{
	return ofGetElapsedTimeMillis();
}