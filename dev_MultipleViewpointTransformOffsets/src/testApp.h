#pragma once

#include "ofMain.h"
#include "MSAInterpolator.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
class SphereProperties
{
	public:
	
		void draw()
		{
			ofSetColor( color );
			ofSphere( pos, radius );
		}
	
		ofVec3f			pos;
		ofFloatColor	color;
		float			radius;
};

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
class testApp : public ofBaseApp
{

	public:

		void		setup();
		void		update();
		void		draw();
		
		void		draw3DScene();
		
		float		fovV2H(float _fovV, float _width, float _height );
		float		fovH2V(float _fovH, float _width, float _height );
	
		void		keyPressed(int key);
		void		keyReleased(int key);
		void		mouseMoved(int x, int y );
		void		mouseDragged(int x, int y, int button);
		void		mousePressed(int x, int y, int button);
		void		mouseReleased(int x, int y, int button);
		void		windowResized(int w, int h);
		void		dragEvent(ofDragInfo dragInfo);
		void		gotMessage(ofMessage msg);
			
		ofTrueTypeFont fontSmall;
		ofTrueTypeFont fontLarge;
	
		vector< ofFbo* > screens;
	
		vector< SphereProperties > spheres;
};
