#include "testApp.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
bool testApp::shouldRemoveParticle(Particle& _p)
{
	return _p.deleteMe;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::setup()
{
	isServer = false;
	
	fontSmall.loadFont("Fonts/DIN.otf", 8 );
	fontLarge.loadFont("Fonts/DIN.otf", 36 );
	
	ofSeedRandom();
	int uniqueID = ofRandom( 999999999 ); // yeah this is bogus I know. Todo: generate a unique computer ID.
	
	server = NULL;
	client = NULL;
	
	if( ofFile( ofToDataPath("Settings/IsServer.txt")).exists() )
	{
		server = new ServerOscManager();
		server->init( "Settings/ServerSettings.xml" );

		isServer = server->isInitialised();
	}
	else
	{
		client = new ClientOSCManager();
		client->init( uniqueID );
		
		commonTimeOsc = client->getCommonTimeOscObj();
		commonTimeOsc->setEaseOffset( true );
		
		ofAddListener( client->newDataEvent, this, &testApp::newData );
	}
	
	lastTimeAddedParticle = 0.0f;
	
	screenIndex = 0;
	
	// Read the screen index from a file
	ofxXmlSettings XML;
	bool loadedFile = XML.loadFile( "Settings/ClientSettings.xml" );
	if( loadedFile )
	{
		screenIndex = XML.getValue("Settings:ScreenIndex", 0);
	}
	

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::update()
{
	float currTime = 0.0f;
	
	// grab the current time, depending on who we are
	if( isServer )
	{
		currTime = ofGetElapsedTimef();
	}
	else
	{
		currTime = commonTimeOsc->getTimeSecs();
	}

	// If we are the server, we add some particles from time to time
	if( isServer )
	{
		float secsBetweenAddingParticles = 5.0f;
		if( (ofGetElapsedTimef() - lastTimeAddedParticle) > secsBetweenAddingParticles )
		{
			int screenAmount = 1; // how many screens we assume
		
			int startScreen = (int)ofRandom(screenAmount);
			int endScreen	= (int)ofRandom(screenAmount);
			
			ofVec2f startPos(	ofRandom(ofGetWidth()) + (startScreen * ofGetWidth()), ofRandom(ofGetHeight()) );
			ofVec2f endPos(		ofRandom(ofGetWidth()) + (endScreen * ofGetWidth())	 , ofRandom(ofGetHeight()) );
			
			float particleStartTime = currTime + 3.0f; // start the particle 3 seconds from now
			float particleLifeDuration = startPos.distance( endPos ) * 0.005f;
			
			ofFloatColor tmpColor;
			tmpColor.setHsb( ofRandom(1.0f), 0.7f, 0.5f );
			
			DataPacket tmpPacket;
			
			tmpPacket.numbersFloat.push_back( particleStartTime );
			tmpPacket.numbersFloat.push_back( particleLifeDuration );
			
			tmpPacket.numbersFloat.push_back( startPos.x );
			tmpPacket.numbersFloat.push_back( startPos.y );

			tmpPacket.numbersFloat.push_back( endPos.x );
			tmpPacket.numbersFloat.push_back( endPos.y );

			tmpPacket.numbersFloat.push_back( tmpColor.r );
			tmpPacket.numbersFloat.push_back( tmpColor.g );
			tmpPacket.numbersFloat.push_back( tmpColor.b );
			
			server->sendData(tmpPacket.numbersInt, tmpPacket.numbersFloat);
			
			newData( tmpPacket );
			
			lastTimeAddedParticle = currTime;
		}
	}
	
	// Update
	for( unsigned int i = 0; i < particles.size(); i++ )
	{
		particles.at(i).update( currTime );
	}
	
	// Remove any dead particles
	ofRemove(particles, shouldRemoveParticle );
	
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::draw()
{
	float currTime = 0.0f;
	
	// grab the current time, depending on who we are
	if( isServer ) { currTime = ofGetElapsedTimef(); } else { currTime = commonTimeOsc->getTimeSecs(); }

	
	ofSetColor(255);
	
	fontLarge.drawString( "Screen: " + ofToString(screenIndex) + "  Time: " + ofToString( currTime, 2), 7, 45 );
	
	if( isServer )
	{
		fontLarge.drawString( "Server", 7, 85 );
	}
	else
	{
		fontSmall.drawString( "Offset: " + ofToString(commonTimeOsc->offsetMillis) + " OffsetTarget: " + ofToString(commonTimeOsc->offsetMillisTarget), 7, 80 );
	}
	
	// draw things 
	ofPushMatrix();
		
		// offset depending on which screen we are
		
		for( unsigned int i = 0; i < particles.size(); i++ )
		{
			particles.at(i).draw();
		}
	
	ofPopMatrix();
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::newData( DataPacket& _packet  )
{
	ofVec2f startPos;
	ofVec2f endPos;

	ofFloatColor color;
	
	float particleStartTime = _packet.numbersFloat.at(0);
	float animationLength = _packet.numbersFloat.at(1);
	
	startPos.x = _packet.numbersFloat.at(2);
	startPos.y = _packet.numbersFloat.at(3);
	
	endPos.x = _packet.numbersFloat.at(4);
	endPos.y = _packet.numbersFloat.at(5);
	
	color.r = _packet.numbersFloat.at(6);
	color.g = _packet.numbersFloat.at(7);
	color.b = _packet.numbersFloat.at(8);
	
	particles.push_back( Particle(startPos, endPos, particleStartTime, animationLength, color) );
}


// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::keyPressed(int key)
{
	if( key == 's' )
	{
		// If we haven't initialised the server, delete the client so that we can bind the ports we want
		if( server == NULL )
		{
			delete client;
			commonTimeOsc = NULL;
			server = new ServerOscManager();
		}
		
		if( !server->isInitialised() )
		{
			server->init( "Settings/ServerSettings.xml" );
		}
		
		isServer = server->isInitialised();
	}
	else if( key >= 48 && key <= 57 ) // change screen index with keys 0..9
	{
		screenIndex = key - 48;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::gotMessage(ofMessage msg)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::keyReleased(int key)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::mouseMoved(int x, int y )
{
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::mouseDragged(int x, int y, int button)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::mousePressed(int x, int y, int button)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::mouseReleased(int x, int y, int button)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::windowResized(int w, int h)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::dragEvent(ofDragInfo dragInfo)
{
}