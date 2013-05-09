#include "ofMain.h"

//#define USE_DEVELOP_APP

#ifdef USE_DEVELOP_APP
	#include "developApp.h"
#warning "!!!!!!! YOU ARE USING THE developApp WHEN YOU PROBABLY WANT THE testApp !!!!!!!!"
#else
	#include "testApp.h"
#endif

#include "ofProgrammableGLRenderer.h"

//========================================================================
int main( ){
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofProgrammableGLRenderer()));
	ofSetupOpenGL(1280,720, OF_WINDOW);

	#ifdef USE_DEVELOP_APP
		ofRunApp( new developApp());
	#else
		ofRunApp( new testApp());
	#endif
}
