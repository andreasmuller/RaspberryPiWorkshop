/*
 *  ofxOpenNI2Recorder.cpp
 *
 *  Created by jason van cleave on 3/5/13.
 *
 */


/*
 So the openni::Recorder class although easy to use by itself cannot be reinitialized
 stopping and starting a recording will currently just append to the filepath
 set at setup()
 
 TODO: change recorder to a pointer to allow for multiple file sessions 
 */

#include "ofxOpenNI2Recorder.h"

ofxOpenNI2Recorder::ofxOpenNI2Recorder()
{
	isReady = false;
	isRecording = false;
	didRecord = false;
}
ofxOpenNI2Recorder::~ofxOpenNI2Recorder()
{
	close();
}
void ofxOpenNI2Recorder::setup(ofxOpenNI2Grabber* grabber_)
{
	grabber = grabber_;
	if (grabber == NULL) 
	{
		ofLogError() << "Grabber is NULL, setup failing";
	}
	Status status = STATUS_OK;
	
	string recordingFileName = ofGetTimestampString();
	recordingFileName+="_";
	if (grabber->depthSource.isOn) 
	{
		recordingFileName+="depth_";
	}
	if (grabber->rgbSource.isOn) 
	{
		recordingFileName+="color_";
	}
	recordingFileName+= ofToString(grabber->settings.width);
	recordingFileName+="w_";
	recordingFileName+= ofToString(grabber->settings.height);
	recordingFileName+="h_";
	recordingFileName+= ofToString(grabber->settings.fps);
	recordingFileName+="fps.oni";
	
	recordingFilePath = ofToDataPath(recordingFileName, true);
	
	status = recorder.create(recordingFilePath.c_str());
	if (status != STATUS_OK) 
	{
		ofLogError() << "Recorder creation FAIL: " << OpenNI::getExtendedError();
		return;
	}else 
	{
		ofLogVerbose() << "Recorder creation PASS, file will be " << recordingFilePath;
	}
	
	if (grabber->depthSource.isOn) 
	{
		recorder.attach(grabber->depthSource.videoStream, false);
	}
	if (grabber->rgbSource.isOn) 
	{
		recorder.attach(grabber->rgbSource.videoStream, false);
	}
	isReady = true;
	
}
void ofxOpenNI2Recorder::startRecording()
{
	if (isRecording) 
	{
		ofLogError() << "Already recording";
		return;
	}
	if (!isReady) 
	{
		ofLogError() << "Not creating recording, Did you call setup?";
	}
	Status status = recorder.start();
	
	if (status == STATUS_OK) 
	{
		isRecording = true;
		didRecord = true;
	}else 
	{
		ofLogError() << "Start Recording FAIL: " << OpenNI::getExtendedError();
	}
	
}

void ofxOpenNI2Recorder::stopRecording()
{
	if (!isReady) 
	{
		ofLogError() << "Not stopping recording, Did you call setup?";
	}
	if (!isRecording) 
	{
		ofLogError() << "Not recording so not stopping";
		return;
	}
	
	recorder.stop();
	isRecording = false;
	
}

void ofxOpenNI2Recorder::close()
{
	if (!didRecord) {
		ofLogVerbose() << "we never recorded anything so deleting the file";
		ofFile file(recordingFilePath);
		if (file.exists()) 
		{
			int HundredK = 100*1000;
			if (file.getSize()<HundredK) {
				file.remove(); //yolo
			}
		}
	}
	ofLogVerbose() << "ofxOpenNI2Recorder::close";
	if (isRecording) 
	{
		stopRecording();
	}
	recorder.destroy();
	isReady = false;
}