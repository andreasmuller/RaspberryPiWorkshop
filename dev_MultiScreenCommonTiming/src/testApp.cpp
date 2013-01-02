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
	
	oscManager.init( uniqueID );
	commonTimeOsc = oscManager.getCommonTimeOscObj();
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

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::draw()
{
	float currAnimationTimeSecs = commonTimeOsc->getTimeSecs();
			
	if( sceneIndex == 0 )
	{
		ofColor backgroundColor( 82, 132, 200 );
		if( isServer ) { backgroundColor.set( 200, 82, 110 ); }
		ofBackground( backgroundColor );
		
		ofSetColor( 82, 179, 200 );
		float tmpRadius = ofGetHeight() * 0.45f;
		ofPushMatrix();
			ofTranslate( ofGetWidth()/2, ofGetHeight()/2 );
			ofRotate( currAnimationTimeSecs * 20.0f );
			ofTranslate( tmpRadius, 0 );
			ofCircle( 0,0, 40 );
		ofPopMatrix();
	}
	else if ( sceneIndex == 1 )
	{
		int col = ((sinf( commonTimeOsc->getTimeSecs() ) + 1.0f) * 0.5f) * 255;
		ofBackground( col, col, col );
	}
	else
	{
	}
	
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
		isServer = !isServer;
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
