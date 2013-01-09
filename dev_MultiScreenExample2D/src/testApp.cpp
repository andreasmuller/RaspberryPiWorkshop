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
	
	client = new ClientOSCManager();
	client->init( uniqueID );
	
	commonTimeOsc = client->getCommonTimeOscObj();
	commonTimeOsc->setEaseOffset( true );
	
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
	float currAnimationTimeSecs = 0.0f;
	
	if( isServer )
	{
		currAnimationTimeSecs = ofGetElapsedTimef();
	}
	else
	{
		currAnimationTimeSecs = commonTimeOsc->getTimeSecs();
	}

   //ofRemove(bugs, shouldRemoveBug);
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::draw()
{
	float currAnimationTimeSecs = commonTimeOsc->getTimeSecs();

	
	ofSetColor(255);
	
	fontLarge.drawString( "Screen: " + ofToString(screenIndex), 7, 45 );
	
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
		
	ofPopMatrix();
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::addNewParticle( int _seed )
{
	// Actually, we are going to need the starttime 
	
	ofSeedRandom( _seed );
	
	ofVec2f startPos;
	ofVec2f endPos;
	
	float animationLength = 0.0f;
	
	ofSeedRandom();
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
