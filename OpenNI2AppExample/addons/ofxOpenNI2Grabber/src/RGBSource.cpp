/*
 *  RGBSource.cpp
 *  OpenNI2AppExample
 *
 *  Created by jason van cleave on 3/2/13.
 *  Copyright 2013 jasonvancleave.com. All rights reserved.
 *
 */

#include "RGBSource.h"

RGBSource::RGBSource()
{
	width = 0;
	height = 0;
	backPixels = NULL;
	currentPixels = NULL;
	doDoubleBuffering = true;
#ifdef TARGET_OPENGLES
	doDoubleBuffering = false;
#endif
	isOn = false;
}
bool RGBSource::setup(DeviceController& deviceController)
{
	
	Status status = STATUS_OK;
	
	status = videoStream.create(deviceController.device, SENSOR_COLOR);
	if (status == STATUS_OK)
	{
		ofLogVerbose() << "Find RGBSource videoStream PASS";
		status = videoStream.start();
		if (status == STATUS_OK)
		{
			ofLogVerbose() << "RGBSource videoStream start PASS";
		}else 
		{
			
			ofLogError() << "RGBSource videoStream start FAIL: " << OpenNI::getExtendedError();
			videoStream.destroy();
		}
	}else
	{
		ofLogError() << "Find RGBSource videoStream FAIL: "<< OpenNI::getExtendedError();
		videoStream.destroy();
	}
	if (videoStream.isValid())
	{
		if(!deviceController.settings.useOniFile && !deviceController.isKinect)
		{
			const VideoMode* requestedMode = deviceController.findMode(SENSOR_COLOR); 
			if (requestedMode != NULL) 
			{
				videoStream.setVideoMode(*requestedMode);
			}
		}
		allocateBuffers();
		status = videoStream.addNewFrameListener(this);
		if (status == STATUS_OK)
		{
			ofLogVerbose() << "RGBSource videoStream addNewFrameListener PASS";
		}else
		{
			ofLogVerbose() << "RGBSource videoStream addNewFrameListener FAIL" << OpenNI::getExtendedError() ;
		}
		
		isOn = true;
	}else 
	{
		ofLogError() << "RGBSource is INVALID";
	}
	return isOn;
}

void RGBSource::update()
{
	texture.loadData(*currentPixels);
}
void RGBSource::draw()
{
	texture.loadData(*currentPixels);
	texture.draw(0, 0);
}
void RGBSource::allocateBuffers()
{
	videoMode = videoStream.getVideoMode();
	width = videoMode.getResolutionX();
	height = videoMode.getResolutionY();
	
	pixels[0].allocate(width, height, OF_IMAGE_COLOR);
	pixels[1].allocate(width, height, OF_IMAGE_COLOR);
	currentPixels = &pixels[0];
	backPixels = &pixels[1];
	
	texture.allocate(width, height, GL_RGB);
}
void RGBSource::onNewFrame(VideoStream& stream)
{
	//ofLogVerbose() << "RGBSource::onNewFrame";
	stream.readFrame(&videoFrameRef);
	if (doDoubleBuffering) 
	{
		backPixels->setFromPixels((unsigned char *)videoFrameRef.getData(), width, height, OF_IMAGE_COLOR);
		swap(backPixels, currentPixels);
	}else 
	{
		currentPixels->setFromPixels((unsigned char *)videoFrameRef.getData(), width, height, OF_IMAGE_COLOR);
	}

	
}
void RGBSource::close()
{
	ofLogVerbose() << "RGBSource close";
	videoStream.stop();
	videoStream.removeNewFrameListener(this);
	videoStream.destroy();
}