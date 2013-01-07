#include "testApp.h"


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
	
	sceneIndex = 0;
	
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
	
	sceneManager.update( currAnimationTimeSecs );
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::draw()
{
	float currAnimationTimeSecs = commonTimeOsc->getTimeSecs();
			
	
	ofSetColor(255);
	
	fontLarge.drawString( ofToString(screenIndex), 7, 40 );
	
	fontSmall.drawString( "Offset: " + ofToString(commonTimeOsc->offsetMillis) + " OffsetTarget: " + ofToString(commonTimeOsc->offsetMillisTarget), 7, 80 );
	
	//cout << commonTimeOsc->offsetMillis << "	" << commonTimeOsc->offsetMillisTarget << endl;
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
			server = new MasterServerOsc();
		}
		
		if( !server->isInitialised() )
		{
			server->init( "Settings/ServerSettings.xml" );
		}
		
		isServer = server->isInitialised();
		
	}
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
void testApp::gotMessage(ofMessage msg)
{
	//cout << "testApp::gotMessage: " << msg.message << endl;
	
	vector <string> tokens = ofSplitString( msg.message, " ", true, true );
	
	if( tokens.size() > 0 )
	{
		if( tokens.at(0) == "change_scene" )
		{
			sceneIndex = ofToInt( tokens.at(1) );
			
			cout << tokens.at(0) << ":" << tokens.at(1) << endl;
		}
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::dragEvent(ofDragInfo dragInfo)
{
	
}
