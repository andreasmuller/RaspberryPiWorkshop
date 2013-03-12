#pragma once

#include "ofMain.h"
#include "ofxOpenNI2Grabber.h"

using namespace openni;

class ofxOpenNI2Recorder
{
public:
	ofxOpenNI2Recorder();
	~ofxOpenNI2Recorder();
	Recorder recorder;
	
	ofxOpenNI2Grabber* grabber;
	void setup(ofxOpenNI2Grabber* grabber_);
	void startRecording();
	void stopRecording();
	void close();
	
	bool isReady;
	bool isRecording;
	bool didRecord;
	string recordingFilePath;
};