#include "testApp.h"

ofGstVideoUtils videoUtils;
ofTexture videoTexture;
int camWidth;
int camHeight;
ofImage videoImage;
//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	camWidth 		= 320;	
	camHeight 		= 240;

	string pipeline = "v4l2src name=video_source device=/dev/video0 ! video/x-raw-rgb,width="+ofToString(camWidth)+",height="+ofToString(camHeight)+",framerate=60/1";
	videoTexture.allocate(camWidth, camHeight, GL_RGB);
	bool didStart = videoUtils.setPipeline(pipeline, 24, false, camWidth, camHeight);
	videoUtils.play();	
}


//--------------------------------------------------------------
void testApp::update(){
	ofBackground(100,100,100);
	videoUtils.update();
	if (videoUtils.isFrameNew()) 
	{
		videoTexture.loadData(videoUtils.getPixels(), camWidth, camHeight, GL_RGB);
		//videoImage = videoUtils.getPixelsRef();
	}

}

//--------------------------------------------------------------
void testApp::draw(){
	videoTexture.draw(20, 20, camWidth, camHeight);
	ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), ofGetWidth()/2, ofGetHeight()/2, ofColor::black, ofColor::yellow);
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

