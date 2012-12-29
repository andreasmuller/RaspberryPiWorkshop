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
	
	ofColor backgroundColor( 82, 132, 200 );
	if( isServer ) { backgroundColor.set( 200, 82, 110 ); }
	ofSetBackgroundColor( backgroundColor );
	
	ofSetColor(255);
	fontSmall.drawString( "Offset: " + ofToString(commonTimeOsc->offsetMillis) + " OffsetTarget: " + ofToString(commonTimeOsc->offsetMillisTarget), 40, 80 );
	
	ofSetColor( 82, 179, 200 );
	float tmpRadius = ofGetHeight() * 0.45f;
	ofPushMatrix();
		ofTranslate( ofGetWidth()/2, ofGetHeight()/2 );
		ofRotate( currAnimationTimeSecs * 20.0f );
		ofTranslate( tmpRadius, 0 );
		ofCircle( 0,0, 40 );
	ofPopMatrix();
	
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
void testApp::gotMessage(ofMessage msg){

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
