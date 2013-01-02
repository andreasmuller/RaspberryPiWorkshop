#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	


#ifdef TARGET_OPENGLES
	autoShader.load("LiveShader");
	cout << "Loading mobile shader" << endl;
#else
	autoShader.load("LiveShaderDesktop");
	cout << "Loading desktop shader" << endl;	
#endif
	
	fbo.allocate(ofGetWidth(), ofGetHeight());
	fbo.begin();
		ofClear(0, 0, 0, 0);
	fbo.end();
}

//--------------------------------------------------------------
void testApp::update(){
	ofEnableAlphaBlending();	
	fbo.begin();
	ofClear(0, 0, 0, 0);
		autoShader.begin();
		autoShader.setUniform1f("time", ofGetElapsedTimef() );
			ofRect(0, 0, ofGetWidth(), ofGetHeight());
		autoShader.end();
	fbo.end();
	ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void testApp::draw(){
	fbo.draw(0, 0);

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

