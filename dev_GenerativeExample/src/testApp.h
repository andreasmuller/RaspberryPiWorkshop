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
		void				keyReleased(int key);
		void				mouseMoved(int x, int y );
		void				mouseDragged(int x, int y, int button);
		void				mousePressed(int x, int y, int button);
		void				mouseReleased(int x, int y, int button);
		void				windowResized(int w, int h);
		void				dragEvent(ofDragInfo dragInfo);
		void				gotMessage(ofMessage msg);

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
