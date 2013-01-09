#include "testApp.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::setup()
{
	isServer = false;
	
	fontSmall.loadFont("Fonts/DIN.otf", 8 );
		
	ofSeedRandom();
	int uniqueID = ofRandom( 999999999 ); // Yeah this is bogus I know. Good enough for our purposes.
	
	client.init( uniqueID );
	
	commonTimeOsc = client.getCommonTimeOscObj();
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
			
	// Set a color that pulsates based on the time we get
	ofColor bgColor;
	bgColor.setHsb( ((cosf(currAnimationTimeSecs/100.0f)+1.0f)/2.0f) * 255, 180, ((cosf(currAnimationTimeSecs*1.4f)+1.0f)/2.0f) * 255 );
	ofSetColor(bgColor);
	ofRect(0,0,ofGetWidth(), ofGetHeight() );

	// Rotate a circle
	ofColor circleColor = bgColor.getInverted();
	ofSetColor(circleColor);	
	ofPushMatrix();
		ofTranslate(ofGetWidth() * 0.5f, ofGetHeight() * 0.5f );
		ofRotate( currAnimationTimeSecs * 50.0f );
		ofTranslate( ofGetHeight() * 0.45f, 0 );
		ofCircle( 0, 0, 40 );
	ofPopMatrix();
	
	ofSetColor(255);
	fontSmall.drawString( "Offset: " + ofToString(commonTimeOsc->offsetMillis) + " OffsetTarget: " + ofToString(commonTimeOsc->offsetMillisTarget), 7, 15 );
	
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::keyPressed(int key)
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
void testApp::gotMessage(ofMessage msg)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::dragEvent(ofDragInfo dragInfo)
{
}
