#include "ofMain.h"
#include "testApp.h"

#ifdef HEADLESS
	#include "Utils/ofAppNoWindow.h"
#else
	#include "ofAppGlutWindow.h"
#endif


//========================================================================
int main(){

#ifdef HEADLESS
	ofAppNoWindow window;
#else
	ofAppGlutWindow window;
#endif
	
	ofSetupOpenGL(&window, 800, 600, OF_WINDOW);
	
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new testApp());

}
