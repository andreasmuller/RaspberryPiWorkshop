#pragma once
#include "ofMain.h"
#include "ofVbo.h"

#ifdef TARGET_OPENGLES
#define NUM_BILLBOARDS 500
#else
#define NUM_BILLBOARDS 10000
#endif


class testApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void draw();
		void exit();
	
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofVbo vbo;
		ofShader shader;
		ofImage texture;
		#ifdef TARGET_OPENGLES
		vector<ofVec3f> pos;
		ofVboMesh mesh;
		#else
		ofVec2f pos[NUM_BILLBOARDS];
		#endif
		ofVec2f vel[NUM_BILLBOARDS];
		ofVec2f home[NUM_BILLBOARDS];
		float pointSizes[NUM_BILLBOARDS];
		float rotations[NUM_BILLBOARDS];
	
	int previousInputX;
	int previousInputY;
	int inputX;
	int inputY;
	
};









