

#include "MasterServerOsc.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
MasterServerOsc::MasterServerOsc()
{
	initialised = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
MasterServerOsc::~MasterServerOsc()
{

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void MasterServerOsc::init( string _xmlSettingsPath )
{
	string _defaultServerSendHost	= "192.168.1.255";
	int _defaultServerSendPort		= 7778;
	int _defaultServerReceivePort	= 7777;
	
	ofxXmlSettings XML;
	
	bool loadedFile = false;
	if( _xmlSettingsPath != "") loadedFile = XML.loadFile( _xmlSettingsPath );
	
	if( loadedFile )
	{
		init( XML.getValue("Settings:ServerSendHost",		_defaultServerSendHost ),
			  XML.getValue("Settings:ServerSendPort",		_defaultServerSendPort ),
			  XML.getValue("Settings:ServerReceivePort",	_defaultServerReceivePort) );
	}
	else
	{
		init( _defaultServerSendHost, _defaultServerSendPort, _defaultServerReceivePort ); // init with default
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void MasterServerOsc::init(string _serverSendHost, int _serverSendPort, int _serverReceivePort )
{
	serverSendHost	= _serverSendHost;
	serverSendPort	= _serverSendPort;
	serverReceivePort = _serverReceivePort;

	sender.setup( serverSendHost, serverSendPort);
	
	receiver.setup( serverReceivePort );

	lastSentHelloMessageMillis = 0;
	milliseBetweenHelloMessages = 3 * 1000;

	ofAddListener(ofEvents().update, this, &MasterServerOsc::_update );
	
	initialised = true;
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
		m.addIntArg( serverReceivePort ); // add the port we would like to use to receive messages as an argument, seems more flexible than having a rule like remote port + 1
		// add the scene that we are in as well? Otherwise screens joining will not know what scene we are supposed to be in. 
		
		sender.sendMessage( m );
		
		lastSentHelloMessageMillis = getServerTime();
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
			m.addIntArg( remoteComputerID );		// their ID
			m.addIntArg( getServerTime() );			// my time
			m.addIntArg( remoteComputerTime );		// their time				

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
	if( !initialised ) return;
	
	string buf;
	buf = "Sending OSC messages to " + string(serverSendHost) + ":"+ ofToString(serverSendPort) + "    Listening for OSC messages on port: " + ofToString(serverReceivePort);

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
int MasterServerOsc::getServerTime()
{
	return ofGetElapsedTimeMillis();
}