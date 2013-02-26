#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	videoWidth = 320;
	videoHeight = 240;
#ifdef _USE_LIVE_VIDEO
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(videoWidth,videoHeight);
#else
	vidPlayer.loadMovie("/home/pi/openFrameworks/examples/addons/opencvExample/bin/data/fingers.mov");
	vidPlayer.setLoopState(OF_LOOP_NORMAL);
	vidPlayer.play();
#endif
	
    colorImg.allocate(videoWidth, videoHeight);
	grayImage.allocate(videoWidth, videoHeight);
	grayBg.allocate(videoWidth, videoHeight);
	grayDiff.allocate(videoWidth, videoHeight);
	
	bLearnBakground = true;
	threshold = 80;
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(100,100,100);
	
    bool bNewFrame = false;
	
	#ifdef _USE_LIVE_VIDEO
		vidGrabber.update();
		bNewFrame = vidGrabber.isFrameNew();
	#else
		vidPlayer.update();
		bNewFrame = vidPlayer.isFrameNew();
	#endif
		
	if (bNewFrame){
			
	#ifdef _USE_LIVE_VIDEO
			colorImg.setFromPixels(vidGrabber.getPixels(), videoWidth,videoHeight);
	#else
			colorImg.setFromPixels(vidPlayer.getPixels(), videoWidth,videoHeight);
	#endif
		
        grayImage = colorImg;
		if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}
		
		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);
		
		// find contours which are between the size of 0 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, (videoWidth*videoHeight)/3, 10, true);	// find holes
	}
}

void testApp::drawVideoLabel(string label)
{
	ofColor boxBackgroundColor(0, 0, 0, 128);
	ofColor boxForegroundColor = ofColor::yellow;
	ofDrawBitmapStringHighlight(label, 0, videoHeight-20, boxBackgroundColor, boxForegroundColor);
}

//--------------------------------------------------------------
void testApp::draw(){
	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetColor(ofColor::white);

	ofPushMatrix();
		ofTranslate(0, 0);
		colorImg.draw(0, 0);
		drawVideoLabel("colorImg");
	ofPopMatrix();
	
	ofPushMatrix();
		ofTranslate(videoWidth, 0);
		grayImage.draw(0, 0);
		drawVideoLabel("grayImage");
	ofPopMatrix();
	
	ofPushMatrix();
		ofTranslate(0, videoHeight);
		grayBg.draw(0, 0);
		drawVideoLabel("grayBg");
	ofPopMatrix();
	
	ofPushMatrix();
		ofTranslate(videoWidth, videoHeight);
		grayDiff.draw(0, 0);
		drawVideoLabel("grayDiff");
	ofPopMatrix();
	
	// then draw the contours:
	ofPushMatrix();
		ofTranslate(videoWidth*2, 0, 0);
		ofFill();
		ofSetColor(ofColor::gray);
		ofRect(0, 0, videoWidth, videoHeight);
		ofSetColor(ofColor::white);
	
		// we could draw the whole contour finder
		//contourFinder.draw(0, 0);
	
		// or, instead we can draw each blob individually from the blobs vector,
		// this is how to get access to them:
		for (int i = 0; i < contourFinder.nBlobs; i++){
			contourFinder.blobs[i].draw(0, 0);
		
			// draw over the centroid if the blob is a hole
			ofSetColor(255);
			if(contourFinder.blobs[i].hole){
				ofDrawBitmapString("hole",
							   contourFinder.blobs[i].boundingRect.getCenter().x,
							   contourFinder.blobs[i].boundingRect.getCenter().y);
			}
		}
		// finally, a report:
		ofSetColor(ofColor::white);
		stringstream reportStr;
		reportStr	<< "bg subtraction and blob detection"			<< endl
					<< "press ' ' to capture bg"					<< endl
					<< "threshold " << threshold << " (press: +/-)" << endl
					<< "num blobs found " << contourFinder.nBlobs << ", fps: " << ofGetFrameRate();
		ofDrawBitmapString(reportStr.str(), 20, videoHeight+20);
	ofPopMatrix();
	

}
void testApp::exit()
{
ofLogVerbose() << "program was locking up the RPi - trying this on exit()";
#ifdef _USE_LIVE_VIDEO
	vidGrabber.close();
#else
	vidPlayer.close();
#endif
	ofSleepMillis(50);
	
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

