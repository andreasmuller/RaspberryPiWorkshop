/*
 *  DeviceController.cpp
 *  OpenNI2AppExample
 *
 *  Created by jason van cleave on 3/2/13.
 *  Copyright 2013 jasonvancleave.com. All rights reserved.
 *
 */

#include "DeviceController.h"

DeviceController::DeviceController()
{
	isKinect = false;
}
void DeviceController::onDeviceConnected(const DeviceInfo*)
{
	ofLogVerbose() << "onDeviceConnected";
}
void DeviceController::onDeviceDisconnected(const DeviceInfo*)
{
	ofLogVerbose() << "onDeviceDisconnected";
}

void DeviceController::onDeviceStateChanged(const DeviceInfo*, DeviceState)
{
	ofLogVerbose() << "onDeviceStateChanged";
}

void DeviceController::setup(ofxOpenNI2GrabberSettings _settings)
{
	settings = _settings;
	Array<DeviceInfo> deviceInfoList;
	OpenNI::enumerateDevices(&deviceInfoList);
	for (int i=0; i<deviceInfoList.getSize(); i++) 
	{
		ofLogVerbose() << "Device URI: "			<< deviceInfoList[i].getUri();
		ofLogVerbose() << "Device Vendor: "			<< deviceInfoList[i].getVendor();
		ofLogVerbose() << "Device Name: "			<< deviceInfoList[i].getName();
		ofLogVerbose() << "Device USB Vendor ID "	<< deviceInfoList[i].getUsbVendorId();
		ofLogVerbose() << "Device USB Product ID: "	<< deviceInfoList[i].getUsbProductId();
	}
	Status status = STATUS_OK;
	
	OpenNI::addDeviceStateChangedListener(this);
	OpenNI::addDeviceConnectedListener(this);
	OpenNI::addDeviceDisconnectedListener(this);
	if (settings.useOniFile) 
	{
		status = device.open(settings.oniFilePath.c_str());
		if (status == STATUS_OK) 
		{
			ofLogVerbose() << "Open file PASS: " << settings.oniFilePath;
		}else 
		{
			ofLogError() << "Open file " << settings.oniFilePath << " FAIL:" << OpenNI::getExtendedError();
		}

	}else 
	{
		status = device.open(ANY_DEVICE);
		if (status == STATUS_OK) 
		{
			printVideoModes();
			string deviceName = device.getDeviceInfo().getName();
			if (deviceName == "Kinect") 
			{
				isKinect = true;
				ofLogVerbose() << "DEVICE IS KINECT";
			}
			ofLogVerbose() << "Device open PASS: " << deviceName;
		}else 
		{
			ofLogError() << "Device open FAIL:" << OpenNI::getExtendedError();
			ofLogWarning() << "If you are on the RPi and using the Xtion Pro, See /addons/ofxOpenNI2Grabber/libs/openni2/udev_rules_for_xtion_pro/README.txt";
		}
	}
}
//The Kinect/freenect driver does not find anything, The Xtion Pro has options
const VideoMode* DeviceController::findMode(SensorType sensorType)
{
	const VideoMode* mode = NULL;
	const SensorInfo* sensorInfo = device.getSensorInfo(sensorType);
	const Array<VideoMode>& videoModes = sensorInfo->getSupportedVideoModes();
	for (int i=0; i<videoModes.getSize(); i++) 
	{
		const VideoMode& currentMode  = videoModes[i];
		ofLogVerbose() << i;
		switch (sensorType) 
		{
			case SENSOR_DEPTH:
			{
				if(
				   currentMode.getPixelFormat() == settings.depthPixelFormat
				   && currentMode.getResolutionX() == settings.width
				   && currentMode.getResolutionY() == settings.height
				   && currentMode.getFps() == settings.fps
				   ){
					mode = &currentMode;
					printVideoMode(*mode);
					return mode;
				}
				break;
				
			}
			case SENSOR_COLOR:
			{
				if(
				   currentMode.getPixelFormat() == settings.colorPixelFormat
				   && currentMode.getResolutionX() == settings.width
				   && currentMode.getResolutionY() == settings.height
				   && currentMode.getFps() == settings.fps
				   ){
					mode = &currentMode;
					printVideoMode(*mode);
					return mode;
				}
				break;
			}
			case SENSOR_IR:
			{
				break;
			}
		}
	}
	return mode;
}

void DeviceController::registerDepthToColor()
{
	if (device.isImageRegistrationModeSupported(IMAGE_REGISTRATION_DEPTH_TO_COLOR)) 
	{
		Status status = device.setImageRegistrationMode(IMAGE_REGISTRATION_DEPTH_TO_COLOR);
		if (status == STATUS_OK)
		{
			ofLogVerbose() << "IMAGE_REGISTRATION_DEPTH_TO_COLOR PASS";
		}else
		{
			ofLogError() << "IMAGE_REGISTRATION_DEPTH_TO_COLOR FAIL:" << OpenNI::getExtendedError();
		}
	}else 
	{
		ofLogError() << "Device does not support IMAGE_REGISTRATION_DEPTH_TO_COLOR";
	}
}

void DeviceController::printDeviceInfo()
{
	DeviceInfo deviceInfo = device.getDeviceInfo();
	ofLogVerbose() << "Device URI: "			<< deviceInfo.getUri();
	ofLogVerbose() << "Device Vendor: "			<< deviceInfo.getVendor();
	ofLogVerbose() << "Device Name: "			<< deviceInfo.getName();
	ofLogVerbose() << "Device USB Vendor ID "	<< deviceInfo.getUsbVendorId();
	ofLogVerbose() << "Device USB Product ID: "	<< deviceInfo.getUsbProductId();
}

void DeviceController::printVideoMode(VideoMode mode)
{
	PixelFormat pixelFormat = mode.getPixelFormat();
	string pixelFormatName;
	switch (pixelFormat) 
	{
		case PIXEL_FORMAT_DEPTH_1_MM:
		{
			pixelFormatName = "PIXEL_FORMAT_DEPTH_1_MM";
			break;
		}
		case PIXEL_FORMAT_DEPTH_100_UM:
		{
			pixelFormatName = "PIXEL_FORMAT_DEPTH_100_UM";
			break;
		}
		case PIXEL_FORMAT_SHIFT_9_2:
		{
			pixelFormatName = "PIXEL_FORMAT_SHIFT_9_2";
			break;
		}
		case PIXEL_FORMAT_SHIFT_9_3:
		{
			pixelFormatName = "PIXEL_FORMAT_SHIFT_9_3";
			break;
		}
		case PIXEL_FORMAT_RGB888:
		{
			pixelFormatName = "PIXEL_FORMAT_RGB888";
			break;
		}
		case PIXEL_FORMAT_YUV422:
		{
			pixelFormatName = "PIXEL_FORMAT_YUV422";
			break;
		}
		case PIXEL_FORMAT_GRAY8:
		{
			pixelFormatName = "PIXEL_FORMAT_GRAY8";
			break;
		}
		case PIXEL_FORMAT_GRAY16:
		{
			pixelFormatName = "PIXEL_FORMAT_GRAY16";
			break;
		}
		case PIXEL_FORMAT_JPEG:
		{
			pixelFormatName = "PIXEL_FORMAT_JPEG";
			break;
		}
			
	}
	ofLogVerbose() << "PixelFormat: "	<< pixelFormatName;
	ofLogVerbose() << "ResolutionX: "	<< mode.getResolutionX();
	ofLogVerbose() << "ResolutionY: "	<< mode.getResolutionY();
	ofLogVerbose() << "FPS: "			<< mode.getFps();
}

void DeviceController::printVideoModes()
{
	const SensorInfo* colorSensorInfo = device.getSensorInfo(SENSOR_COLOR);
	const Array<VideoMode>& colorVideoModes= colorSensorInfo->getSupportedVideoModes();
	ofLogVerbose() << "\n--------COLOR MODES--------\n";
	for (int i=0; i<colorVideoModes.getSize(); i++) 
	{
		ofLogVerbose() << "COLOR MODE: " << i;
		printVideoMode(colorVideoModes[i]);
	}
	
	const SensorInfo* depthSensorInfo = device.getSensorInfo(SENSOR_DEPTH);
	const Array<VideoMode>& depthVideoModes = depthSensorInfo->getSupportedVideoModes();
	ofLogVerbose() << "\n--------DEPTH MODES--------\n";
	for (int i=0; i<depthVideoModes.getSize(); i++) 
	{
		ofLogVerbose() << "DEPTH MODE: " << i;
		printVideoMode(depthVideoModes[i]);
	}
}

void DeviceController::close()
{
	ofLogVerbose() << "DeviceController close";
	OpenNI::removeDeviceStateChangedListener(this);
	OpenNI::removeDeviceConnectedListener(this);
	OpenNI::removeDeviceDisconnectedListener(this);
	device.close();
}