#pragma once

#include "ofMain.h"
#include "OpenNI.h"
#include "ofxOpenNI2GrabberSettings.h"

using namespace openni;

class DeviceController:
public OpenNI::DeviceStateChangedListener,
public OpenNI::DeviceDisconnectedListener,
public OpenNI::DeviceConnectedListener
{
public:
	DeviceController();
	void setup(ofxOpenNI2GrabberSettings _settings);
	Device device;
	ofxOpenNI2GrabberSettings settings;
	bool isKinect;
	void close();
	void printDeviceInfo();
	void printVideoMode(VideoMode mode);
	void printVideoModes();
	void registerDepthToColor();
	const VideoMode* findMode(SensorType sensorType);

	void onDeviceConnected(const DeviceInfo*);
	void onDeviceDisconnected(const DeviceInfo*);
	void onDeviceStateChanged(const DeviceInfo*, DeviceState);
};