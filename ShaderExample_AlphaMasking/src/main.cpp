#include "ofMain.h"
#include "testApp.h"
#include "ofProgrammableGLRenderer.h"


//========================================================================
int main( ){

    ofProgrammableGLRenderer * renderer = new ofProgrammableGLRenderer();
	ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(renderer));                      // switch to GLES2 renderer.

	ofSetupOpenGL(1280, 720, OF_WINDOW);// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}
