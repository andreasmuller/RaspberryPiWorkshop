#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
class Particle
{
	public:
	
		ofVec2f pos;
		ofVec2f vel;
	
		int myID;
		
		int spacePartitioningIndexX;
		int spacePartitioningIndexY;
};

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
class testApp : public ofBaseApp
{

	public:

		void				setup();
		void				update();
		void				draw();
		
		void				keyPressed(int key);

		vector< Particle* > particles;
	
		vector< vector< vector< Particle* > > > spacePartitioningGrid;
		int					spacePartitioningResX;
		int					spacePartitioningResY;
		float				spacePartitioningGridWidth;
		float				spacePartitioningGridHeight;
	
		float				lastUpdateTime;
	
		ofMesh				lineMesh;
	
		bool				debugDraw;
	
		ofTrueTypeFont		fontSmall;
		ofTrueTypeFont		fontLarge;
	
		char				tmpStr[256];
};
