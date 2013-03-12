#pragma once

/*
 *  ofxOpenNI2Grabber.h
 *
 *  Created by jason van cleave on 2/28/13.
 *
 */
#include "ofMain.h"
#include "OpenNI.h"
#include "ofxOpenNI2GrabberSettings.h"
#include "DeviceController.h"

#include "RGBSource.h"
#include "DepthSource.h"

using namespace openni;

class ofxOpenNI2Grabber: 
public ofThread
{
public:

	ofxOpenNI2Grabber();
	bool setup(ofxOpenNI2GrabberSettings _settings);
	void readFrame();
	void update();
	void draw();
	bool close();
	
	RGBSource rgbSource;
	DepthSource depthSource;
	DeviceController deviceController;
	
	vector<VideoStream*> streams;

	bool isReady;
	
	ofxOpenNI2GrabberSettings 			settings;
	
	ofPixels & getDepthPixels();
	ofShortPixels & getDepthRawPixels();
	ofPixels & getRGBPixels();
	
	ofTexture & getDepthTextureReference();
	ofTexture & getRGBTextureReference();
	
protected:
	void threadedFunction();

};