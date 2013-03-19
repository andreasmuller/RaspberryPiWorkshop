#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	//ofSetLogLevel(OF_LOG_VERBOSE); set in main.cpp for core debugging
	cameraWidth		= 320;	
	cameraHeight	= 240;
	cameraFPS		= 60;
	
	videoTexture.allocate(cameraWidth, cameraHeight, GL_RGB);
	
	//optimized pipeline for the PS3Eye
	stringstream pipeline;
	pipeline << "v4l2src name=video_source device=/dev/video0 ! video/x-raw-rgb,";
	pipeline << "width="		<< cameraWidth	<<	",";
	pipeline << "height="		<< cameraHeight	<<	",";
	pipeline << "framerate="	<< cameraFPS	<<	"/1";
	
	bool didStart = videoUtils.setPipeline(pipeline.str(), 24, false, cameraWidth, cameraHeight);
	if(didStart)
	{
		ofLogVerbose() << "set pipeline SUCCESS";
	}else 
	{
		ofLogError() << "set pipeline FAIL, pipeline: " << pipeline.str();
	}

	videoUtils.play();	
}


//--------------------------------------------------------------
void testApp::update(){
	
	videoUtils.update();
	if (videoUtils.isFrameNew()) 
	{
		videoTexture.loadData(videoUtils.getPixels(), cameraWidth, cameraHeight, GL_RGB);
	}

}

//--------------------------------------------------------------
void testApp::draw(){
	videoTexture.draw(0, 0, ofGetWidth(), ofGetHeight());
	videoTexture.draw(20, 20, cameraWidth, cameraHeight);
	stringstream info;
	info << "cameraWidth: "		<< cameraWidth			<< "\n";
	info << "cameraHeight: "	<< cameraHeight			<< "\n";
	info << "cameraFPS: "		<< cameraFPS			<< "\n";
	info << "app fps: "			<<	ofGetFrameRate()	<< "\n";
	ofEnableAlphaBlending();
		ofDrawBitmapStringHighlight(info.str(), 20, cameraHeight+40, ofColor(0, 0, 0, 200), ofColor::yellow);
	ofDisableAlphaBlending();
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

