#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	tex0Source.loadImage("brick.jpg");	
	
	contentWidth = tex0Source.getWidth();
	contentHeight = tex0Source.getHeight();
	
	tex0 = tex0Source.getTextureReference();
	tex0.setTextureWrap(GL_REPEAT, GL_REPEAT);
	
	shader.load("MotionBlur_GLES.vert", "MotionBlur_GLES.frag", "");
	fbo.allocate(contentWidth, contentHeight);
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
			shader.setUniformTexture("tex0", tex0, 1);
			shader.setUniform1f("time", ofGetElapsedTimef());
			shader.setUniform2f("resolution", contentWidth, contentHeight);
			ofRect(0, 0, contentWidth, contentHeight);
		shader.end();
	fbo.end();
}

//--------------------------------------------------------------
void testApp::draw(){
	fbo.draw(0, 0, ofGetWidth(), ofGetHeight());

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

