#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"


class testApp : public ofBaseApp {
	
public:
	
	void setup();
	void draw();

	void mousePressed(int x, int y, int button);
	void keyPressed(int key);
};

#endif

