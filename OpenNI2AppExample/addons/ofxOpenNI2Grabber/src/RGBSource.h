#pragma once

#include "ofMain.h"
#include "OpenNI.h"
#include "DeviceController.h"
using namespace openni;

class RGBSource : public VideoStream::NewFrameListener
{
public:
	RGBSource();
	int width;
	int height;
	VideoStream videoStream;
	
	ofTexture texture;
	ofPixels pixels[2];
	ofPixels* backPixels;
	ofPixels* currentPixels;
	
	VideoMode videoMode;
	
	VideoFrameRef videoFrameRef;
	bool setup(DeviceController& deviceController);
	void allocateBuffers();
	
	void close();
	void update();
	void draw();
	
	bool doDoubleBuffering;

	void onNewFrame(VideoStream&);
	
	bool isOn;
};

