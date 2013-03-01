#include "ofMain.h"
#include "testApp.h"

//========================================================================
int main( )
{
	ofSetupOpenGL(1280, 720, OF_WINDOW); // OF_WINDOW or OF_FULLSCREEN
	ofRunApp( new testApp());
}
