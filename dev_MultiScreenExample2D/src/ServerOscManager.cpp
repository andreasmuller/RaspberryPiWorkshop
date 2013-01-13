#include "ServerOscManager.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
/*ServerOscManager::ServerOscManager()
{
	initialised = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
ServerOscManager::~ServerOscManager()
{
}*/

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ServerOscManager::init( string _xmlSettingsPath )
{
	string _serverSendHost	= "192.168.1.255";
	int _serverSendPort		= 7778;
	int _serverReceivePort	= 7777;
	
	ofxXmlSettings XML;
	bool loadedFile = XML.loadFile( _xmlSettingsPath );
	if( loadedFile )
	{
		init( XML.getValue("Settings:ServerSendHost",		"192.168.1.255"),
			  XML.getValue("Settings:ServerSendPost",		7778),
			  XML.getValue("Settings:ServerReceivePort",	7777) );
	}
	else
	{
		init( _serverSendHost, _serverSendPort, _serverReceivePort ); // init with default
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ServerOscManager::init(string _serverSendHost, int _serverSendPort, int _serverReceivePort )
{
	serverSendHost	= _serverSendHost;
	serverSendPort	= _serverSendPort;
	serverReceivePort = _serverReceivePort;

	sender.setup( serverSendHost, serverSendPort);
	
	receiver.setup( serverReceivePort );

	lastSentHelloMessageMillis = 0;
	milliseBetweenHelloMessages = 3 * 1000;

	ofAddListener(ofEvents().update, this, &ServerOscManager::_update );
	
	initialised = true;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ServerOscManager::_update(ofEventArgs &e)
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
void ServerOscManager::draw()
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
void ServerOscManager::sendData( vector<int> _numbersInt, vector<float> _numbersFloat )
{
	if( !initialised ) return;
	
	ofxOscMessage m;
	m.setAddress("/data");

	for( unsigned int i = 0; i < _numbersInt.size(); i++ )
	{
		m.addIntArg( _numbersInt.at(i) );
	}
	
	for( unsigned int i = 0; i < _numbersFloat.size(); i++ )
	{
		m.addFloatArg( _numbersFloat.at(i) );
	}
	
	sender.sendMessage( m );
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
int ServerOscManager::getServerTime()
{
	return ofGetElapsedTimeMillis();
}