#pragma once

#include "ofMain.h"
#include "OpenNI.h"

using namespace openni;

class ofxOpenNI2GrabberSettings
{
public:
	
	int		width;					// camera width
	int		height;					// camera height
	int		fps;					// camera fps
	bool	doDepth;				// use Depth camera
	bool	doRawDepth;				// capture raw Depth info
	bool	doColor;				// use RGB camera
	bool	doPointCloud;			// option for point cloud
	bool	doPointCloudColor;		// color point cloud
	PixelFormat depthPixelFormat;	//PIXEL_FORMAT_DEPTH_1_MM, PIXEL_FORMAT_DEPTH_100_UM, PIXEL_FORMAT_SHIFT_9_2, PIXEL_FORMAT_SHIFT_9_3
	PixelFormat colorPixelFormat;	//PIXEL_FORMAT_RGB888, PIXEL_FORMAT_YUV422, PIXEL_FORMAT_GRAY8, PIXEL_FORMAT_GRAY16, PIXEL_FORMAT_JPEG
	bool	doRegisterDepthToColor;
	bool	useOniFile;
	string oniFilePath; 
	
	ofxOpenNI2GrabberSettings()
	{
		width = 640;
		height = 480;
		fps = 30;
		doDepth = true;
		doRawDepth = true;
		doColor = true;
		depthPixelFormat = PIXEL_FORMAT_DEPTH_1_MM;
		colorPixelFormat = PIXEL_FORMAT_RGB888;
		doRegisterDepthToColor = true;
		doPointCloud = true;			// option for point cloud
		doPointCloudColor = true;		// color point cloud
		useOniFile = false;
		oniFilePath = "UNDEFINED";
	}
};