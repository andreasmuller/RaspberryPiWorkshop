#include "testApp.h"

//Often I have to run 
// $sudo amixer cset numid=3 1
//


//--------------------------------------------------------------
void testApp::setup(){
	
	isReady = potController.setup();
	bool didLoadSound = soundPlayer.loadSound("sounds/1085.mp3");
	soundPlayer.setLoop(true);
	soundPlayer.play(); 
	ofLogVerbose() << "didLoadSound: " << didLoadSound;
}


//--------------------------------------------------------------
void testApp::update(){

	ofSoundUpdate();
}

//--------------------------------------------------------------
void testApp::draw(){
	if (!isReady) {
		return;
	}
	stringstream info;
	info << "lastPotValue: " << potController.lastPotValue << "\n";
	info << "potValue: " << potController.potValue<< "\n";
	info << "changeAmount: " << potController.changeAmount<< "\n";
	info << "fps: " << ofToString(ofGetFrameRate())<< "\n";
	
	int colorValue = potController.potValue;
	ofBackgroundGradient(ofColor::darkOliveGreen, ofColor(colorValue, colorValue, colorValue),  OF_GRADIENT_CIRCULAR);
	ofDrawBitmapStringHighlight(info.str(), 100, 100, ofColor::black, ofColor::yellow);
	
	float speed = ofMap((float)potController.potValue, 0.0, 255.0, 0.1, 2.0, false);
	soundPlayer.setSpeed(speed);
}

void testApp::exit()
{
	if (isReady)
	{
		isReady = false;
		potController.waitForThread(true);
		ofLogVerbose() << "finished waiting on EXIT";
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

