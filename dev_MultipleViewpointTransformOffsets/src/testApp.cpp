#include "testApp.h"


// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::setup()
{
	
	fontSmall.loadFont("Fonts/DIN.otf", 8 );
	fontLarge.loadFont("Fonts/DIN.otf", 36 );
	
	ofSeedRandom();
	int uniqueID = ofRandom( 999999999 ); // yeah this is bogus I know. Todo: generate a unique computer ID.
	
	int screenAmount = 5;
	for( int i = 0; i < screenAmount; i++ )
	{
		float scale = 0.25f;
		ofFbo* tmpScreenSurface = new ofFbo();
		tmpScreenSurface->allocate( 1280 * scale, 720 * scale, GL_RGB );
		screens.push_back( tmpScreenSurface );
	}

	// we set up a camera that covers all the screens, then before rendering we can set the view for each camera and render the scene
	tileCameraView.init( screens.at(0)->getWidth(), screens.at(0)->getHeight(), screenAmount, 1, 30 );
	
	tiledCameraFov = 11.0f; // we want a really narrow Fov to avoid distortion
	tileCameraView.setFov( tiledCameraFov );
	
	// set up a debug Fbo that we render into with the same settings to confim everything looks right
	debugFbo.allocate( tileCameraView.getFullWidth(), tileCameraView.getFullHeight(), GL_RGB  );
	
	// if we seed the random number generator with the same number, we will generate the same positions, etc on all nodes.
	ofSeedRandom( 1234 );
	
	int sphereAmount = 80;
	for( int i = 0; i < sphereAmount; i++ )
	{
		float tmpFrac = (float)i / (sphereAmount-1);
		float tmpAngleRad = ofDegToRad(360.0f * tmpFrac);
		
		SphereProperties tmpSphere;
		
		//tmpSphere.pos.set( ofVec3f( ofRandom(-200, 200), ofRandom(-100, 100), ofRandom(-200, 200) ) );
		//tmpSphere.color.set( ofRandomuf(), ofRandomuf(), ofRandomuf() );
		//tmpSphere.radius = ofRandom( 3.0, 8.0 );
		
		//tmpSphere.pos.set( ofVec3f( tmpFrac * 2000.0, 0.0, -50.0 ) );
		//tmpSphere.color.set( tmpFrac, 1.0-tmpFrac, 1.0 );
		//tmpSphere.radius = 8.0;

		tmpSphere.pos.set( ofVec3f( sinf(tmpAngleRad) * 1000, 0.0, cosf(tmpAngleRad) * 1000 ) );
		tmpSphere.color.set( tmpFrac, 1.0-tmpFrac, 1.0 );
		tmpSphere.radius = ofMap( tmpFrac, 0.0f, 1.0f, 2.0f, 18.0f);
		
		if( i == 0 ) { tmpSphere.color.set(1.0,0.0,0.0); } else if( i == (sphereAmount-1) ) { tmpSphere.color.set(0.0,0.0,1.0); }

		spheres.push_back( tmpSphere );
	}
	
	// Here we can either divide a circle (360) by the amount of screens to get our Fov, but then if we have a low number
	// of screens our Fov will be quite wide, which will give us perspective distortion.
	//cameraViewCircleSlice = 360.0f / screens.size();
	
	// One solution is to base it all one us having a high number, say 18 screens, the server can keep track of the highest
	// screen number that connects and create content that takes that into account
	cameraViewCircleSlice = 360.0f / 18.0f;
	
	doCircularCameraViews = false;
	
	doAnimate = true;
	
	prevTime = ofGetElapsedTimef();
	angle = 0.0f;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::update()
{
	float currTime = ofGetElapsedTimef();
	float timeDelta = prevTime - currTime;
	prevTime = currTime;
	if( doAnimate )
	{
		angle += timeDelta * 40.0f;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::draw()
{
	
	// draw the scene into each FBO with an offset
	for( unsigned int i = 0; i < screens.size(); i++ )
	{		
		ofFbo* tmpScreen = screens.at(i);
		
		tmpScreen->begin();
		
			ofClear(0,0,0);
			
			ofPushView();
			
				ofMatrix4x4 lookAtMatrix;
				if( doCircularCameraViews )
				{
					setPerpectiveTransformForScreen( i, tmpScreen );
					lookAtMatrix = getCameraTransformForScreen( i );
				}
				else
				{
					tileCameraView.setPerspectiveTransformForTile( i );
					lookAtMatrix.makeLookAtViewMatrix( ofVec3f(0,0,0), ofVec3f(0,0,-1), ofVec3f(0,1,0) );
				}
					
				ofPushMatrix();
		
					ofLoadIdentityMatrix();
					ofMultMatrix( lookAtMatrix );
					ofScale( 1, -1, 1 );
				
					draw3DScene();
		
				ofPopMatrix();
			
			ofPopView();
		
		tmpScreen->end();
	}
	
	if( !doCircularCameraViews )
	{
		// debug draw into a big Fbo
		debugFbo.begin();
			ofClear(0,0,0);
			ofPushView();
				ofSetupScreenPerspective( tileCameraView.getFullWidth(), tileCameraView.getFullHeight(), ofGetOrientation(), false, tiledCameraFov );
				ofMatrix4x4 lookAtMatrix;
				lookAtMatrix.makeLookAtViewMatrix( ofVec3f(0,0,0), ofVec3f(0,0,-1), ofVec3f(0,1,0) );
				ofPushMatrix();
					ofLoadIdentityMatrix();
					ofMultMatrix( lookAtMatrix );
					ofScale( 1, -1, 1 );
					draw3DScene();
				ofPopMatrix();
			ofPopView();
		debugFbo.end();
	}
	
	// draw the screen surfaces one next to another
	ofSetColor( 255 );
	ofVec2f tmpPos(0, 0);
	for( unsigned int i = 0; i < screens.size(); i++ )
	{
		screens.at(i)->draw( tmpPos.x, tmpPos.y );
		ofLine( tmpPos.x, tmpPos.y, tmpPos.x, tmpPos.y + screens.at(i)->getHeight() );
		tmpPos.x += tileCameraView.getTileWidthNoBorder();
		if( !doCircularCameraViews )
		{
			tmpPos.x += tileCameraView.getBorderWidth();
		}
		if( (tmpPos.x + screens.at(i)->getWidth()) > ofGetWidth() )
		{
			tmpPos.x = 0.0f;
			tmpPos.y += tileCameraView.getTileHeightNoBorder();
			if( !doCircularCameraViews )
			{
				tmpPos.y += tileCameraView.getBorderHeight();
			}
		}
	}
	
	if( !doCircularCameraViews )
	{
		debugFbo.draw(0, screens.at(0)->getHeight() );
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::setPerpectiveTransformForScreen( int _index, ofFbo* _screenSurface )
{
	float cameraFovX = cameraViewCircleSlice;
	float cameraFovY = fovH2V(cameraFovX, _screenSurface->getWidth(), _screenSurface->getHeight() );
	
	ofSetupScreenPerspective( _screenSurface->getWidth(), _screenSurface->getHeight(), ofGetOrientation(), false, cameraFovY );
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
ofMatrix4x4	testApp::getCameraTransformForScreen( int _index )
{
	float screenIndexAngleRad = ofDegToRad( cameraViewCircleSlice * _index );
	
	ofVec3f pos( 0,0,0);
	ofVec3f center(0,0,0);
	ofVec3f up(0,1,0);
		
	center.set(cosf(screenIndexAngleRad),0,sinf(screenIndexAngleRad) );
	
	ofMatrix4x4 lookAt;
	lookAt.makeLookAtViewMatrix( pos, center, up );
	
	return lookAt;
}


// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::draw3DScene()
{
	ofPushMatrix();
	
		ofRotateY( angle );
			
		ofDrawAxis( 30.0 );
		
		for( unsigned int i = 0; i < spheres.size(); i++ )
		{
			spheres.at(i).draw();
		}
	ofPopMatrix();
	
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
float testApp::fovV2H(float _fovV, float _width, float _height )
{
	_fovV = ofDegToRad(_fovV);
	return ofRadToDeg(2.0*atan(_width*tan(_fovV/2.0)/_height));
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
float testApp::fovH2V(float _fovH, float _width, float _height )
{
	_fovH = ofDegToRad(_fovH);
	return ofRadToDeg(2.0*atan(_height*tan(_fovH/2.0)/_width));
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::keyPressed(int key)
{
	if( key == 'c' )
	{
		doCircularCameraViews = !doCircularCameraViews;
	}
	if( key == 'a' )
	{
		doAnimate = !doAnimate;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::keyReleased(int key)
{

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::mouseMoved(int x, int y )
{

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::mouseDragged(int x, int y, int button)
{

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::mousePressed(int x, int y, int button)
{

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::mouseReleased(int x, int y, int button)
{

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::windowResized(int w, int h)
{

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::gotMessage(ofMessage msg)
{

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::dragEvent(ofDragInfo dragInfo)
{
	
}
