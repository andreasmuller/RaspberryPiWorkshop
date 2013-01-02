#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){

	ofSetLogLevel(OF_LOG_VERBOSE);
	
#ifdef TARGET_OPENGLES
	autoShader.load("Shaders/LiveShader");
	cout << "Loading mobile shader" << endl;
#else
	autoShader.load("Shaders/LiveShaderDesktop");
	cout << "Loading desktop shader" << endl;	
#endif
	
	fbo.allocate(ofGetWidth(), ofGetHeight());
	fbo.begin();
		ofClear(0, 0, 0, 0);
	fbo.end();

	image.loadImage("Textures/landangui.jpg");

	font.loadFont(ofToDataPath( "Fonts/DIN.otf"), 8 );
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

	// draw to the FBO
	ofEnableAlphaBlending();	
	fbo.begin();
	ofClear(0, 0, 0, 0);
		autoShader.begin();
		autoShader.setUniform1f("time", ofGetElapsedTimef() );
			image.draw(0,0, ofGetWidth(), ofGetHeight() );
			//ofRect(0, 0, ofGetWidth(), ofGetHeight());
		autoShader.end();
	fbo.end();
	ofDisableAlphaBlending();

	// draw the FBO to screen
	fbo.draw(0, 0);

	// draw the FPS
	sprintf(tempStr, "%4.1f", ofGetFrameRate() );
	ofVec2f pos( ofGetWidth()-20, ofGetHeight()-20 );

	ofSetColor(0);
	font.drawString( tempStr, pos.x + 1, pos.y + 1 );

	ofSetColor( 255 );
	font.drawString( tempStr, pos.x, pos.y );
	//cout << tempStr << endl;
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){


}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}


//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

