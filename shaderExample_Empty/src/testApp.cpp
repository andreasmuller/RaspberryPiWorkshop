#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	//ofSetLogLevel(OF_LOG_VERBOSE); set in main.cpp for core troubleshooting
	
	bool didLoadShader = shader.load("Empty_GLES.vert", "Empty_GLES.frag", "");
	
	if (!didLoadShader) 
	{
		ofLogError() << "Load Shader FAIL";
	}
	
	fbo.allocate(ofGetWidth(), ofGetHeight());
	fbo.begin();
		ofClear(0, 0, 0, 0);
	fbo.end();
	ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void testApp::update(){
	fbo.begin();
	ofClear(0, 0, 0, 0);
		shader.begin();
			ofRect(0, 0, ofGetWidth(), ofGetHeight());
		shader.end();
	fbo.end();
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

