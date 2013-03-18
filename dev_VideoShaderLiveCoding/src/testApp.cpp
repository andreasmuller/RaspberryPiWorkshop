#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofEnableAlphaBlending();

	string videoPath = "/opt/vc/src/hello_pi/hello_video/test.h264";
	
	//this will let us just grab a video without recompiling
	ofDirectory currentVideoDirectory("/home/pi/videos/current");
	if (currentVideoDirectory.exists()) 
	{
		currentVideoDirectory.listDir();
		vector<ofFile> files = currentVideoDirectory.getFiles();
		if (files.size()>0) 
		{
			videoPath = files[0].path();
		}		
	}
	
	omxPlayer.loadMovie(videoPath);

#ifdef TARGET_OPENGLES
	autoShader.load("Shaders/LiveShader");
	cout << "Loading mobile shader" << endl;
#else
	autoShader.load("Shaders/LiveShaderDesktop");
	cout << "Loading desktop shader" << endl;	
#endif


}

//--------------------------------------------------------------
void testApp::update()
{
	if(!omxPlayer.isPlaying())
	{
		return;
	}
	if(!omxPlayer.isThreaded)
	{
		omxPlayer.update();
	}
}

//--------------------------------------------------------------
void testApp::draw(){

	if(!omxPlayer.isPlaying())
	{
		return;
	}
	

	autoShader.begin();
	autoShader.setUniform1f("time", ofGetElapsedTimef() );
		omxPlayer.getTextureReference().draw(0, 0);
	autoShader.end();

	ofDisableAlphaBlending();

	


	stringstream info;
	info << "" + ofToString(ofGetFrameRate());
	ofDrawBitmapStringHighlight(info.str(), 20, 20, ofColor::black, ofColor::yellow);
}



void testApp::exit()
{
	bool DO_HARD_EXIT = true;
	if(DO_HARD_EXIT)
	{
		ofLogVerbose() << "testApp::exiting hard";
		atexit(0);
	}else 
	{
		omxPlayer.close();
	}
	
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

