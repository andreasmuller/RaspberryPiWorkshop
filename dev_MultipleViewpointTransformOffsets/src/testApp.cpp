#include "testApp.h"


// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::setup()
{
	
	fontSmall.loadFont("Fonts/DIN.otf", 8 );
	fontLarge.loadFont("Fonts/DIN.otf", 36 );
	
	ofSeedRandom();
	int uniqueID = ofRandom( 999999999 ); // yeah this is bogus I know. Todo: generate a unique computer ID.
	
	//vector< ofFbo* > screens;
	int screenAmount = 6;
	for( int i = 0; i < screenAmount; i++ )
	{
		float scale = 0.25f;
		ofFbo* tmpScreenSurface = new ofFbo();
		tmpScreenSurface->allocate( 1280 * scale, 720 * scale, GL_RGB );
		screens.push_back( tmpScreenSurface );
	}

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
	
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::update()
{

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::draw()
{
	// draw the scene into each FBO with an offset
	for( unsigned int i = 0; i < screens.size(); i++ )
	{
		float screenIndexFrac = (float)i / (screens.size()-1);
		float screenIndexAngleRad = ofDegToRad( 360.0f * screenIndexFrac );
		
		ofFbo* tmpScreen = screens.at(i);
		
		tmpScreen->begin();
		
			ofClear(0,0,0);
			
			ofPushView();
		
				float cameraFovX = 360.0f / screens.size();
				float cameraFovY = fovH2V(cameraFovX, tmpScreen->getWidth(), tmpScreen->getHeight() );
		
				ofSetupScreenPerspective( tmpScreen->getWidth(), tmpScreen->getHeight(), ofGetOrientation(), false, cameraFovY );
			
				// set the relevant camera transform for each screen
				float tmpX = tmpScreen->getWidth() * i;
				ofVec3f pos( tmpX,0,-300);
				ofVec3f center(tmpX,0,0);
				ofVec3f up(0,1,0);
		
				pos.set(0,0,0);
				center.set(cosf(screenIndexAngleRad),0,sinf(screenIndexAngleRad) );
		
				ofMatrix4x4 cameraMatrix;
				cameraMatrix.makeLookAtViewMatrix( pos, center, up );
					
				ofPushMatrix();
					ofLoadIdentityMatrix();
					ofMultMatrix( cameraMatrix );
					ofScale( 1, -1, 1 );
				
					//ofTranslate( -tmpScreen->getWidth() * i, 0, 0 );
		
					draw3DScene();
				ofPopMatrix();
			
			ofPopView();
		
		tmpScreen->end();
	}
	
	// draw the screen surfaces one next to another
	ofSetColor( 255 );
	ofVec2f tmpPos(0, 0);
	for( unsigned int i = 0; i < screens.size(); i++ )
	{
		screens.at(i)->draw( tmpPos.x, tmpPos.y );
		ofLine( tmpPos.x, tmpPos.y, tmpPos.x, tmpPos.y + screens.at(i)->getHeight() );
		tmpPos.x += screens.at(i)->getWidth();
		if( (tmpPos.x + screens.at(i)->getWidth()) > ofGetWidth() )
		{
			tmpPos.x = 0.0f;
			tmpPos.y += screens.at(i)->getHeight();
		}
	}
}

/*
 
 float viewW = ofGetViewportWidth();
 float viewH = ofGetViewportHeight();
 
 float eyeX = viewW / 2;
 float eyeY = viewH / 2;
 float halfFov = PI * fov / 360;
 float theTan = tanf(halfFov);
 float dist = eyeY / theTan;
 float aspect = (float) viewW / viewH;
 
 if(nearDist == 0) nearDist = dist / 10.0f;
 if(farDist == 0) farDist = dist * 10.0f;
 
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 
 ofMatrix4x4 persp;
 persp.makePerspectiveMatrix(fov, aspect, nearDist, farDist);
 loadMatrix( persp );
 //gluPerspective(fov, aspect, nearDist, farDist);
 
 
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 
 ofMatrix4x4 lookAt;
 lookAt.makeLookAtViewMatrix( ofVec3f(eyeX, eyeY, dist),  ofVec3f(eyeX, eyeY, 0),  ofVec3f(0, 1, 0) );
 loadMatrix( lookAt );
 */

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::draw3DScene()
{
	ofPushMatrix();

		ofRotateY( ofGetElapsedTimef() * 40.0f );
	
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
