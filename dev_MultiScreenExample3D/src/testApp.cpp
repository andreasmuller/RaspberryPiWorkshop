#include "testApp.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
bool testApp::shouldRemoveParticle(Particle& _p)
{
	return _p.deleteMe;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::setup()
{
	isServer = false;
	
	fontSmall.loadFont("Fonts/DIN.otf", 8 );
	fontLarge.loadFont("Fonts/DIN.otf", 36 );
	
	ofSeedRandom();
	int uniqueID = ofRandom( 999999999 ); // yeah this is bogus I know. Todo: generate a unique computer ID.
	
	server = NULL;
	client = NULL;
	
	if( ofFile( ofToDataPath("Settings/IsServer.txt")).exists() )
	{
		server = new ServerOscManager();
		server->init( "Settings/ServerSettings.xml" );

		isServer = server->isInitialised();
	}
	else
	{
		client = new ClientOSCManager();
		client->init( uniqueID );
		
		commonTimeOsc = client->getCommonTimeOscObj();
		commonTimeOsc->setEaseOffset( true );
		
		ofAddListener( client->newDataEvent, this, &testApp::newData );
	}
	
	lastTimeAddedParticle = 0.0f;
	
	screenIndex = 0;
	
	// Read the screen index from a file
	ofxXmlSettings XML;
	bool loadedFile = XML.loadFile( "Settings/ClientSettings.xml" );
	if( loadedFile )
	{
		screenIndex = XML.getValue("Settings:ScreenIndex", 0);
	}
	
	cameraFov = 21.0f; // we want a really narrow Fov to avoid distortion
	tiledCameraView.setFov( cameraFov );
	
	int maxScreenAmount = 6;
	int singleScreenWidth  = 1280;
	int singleScreenHeight = 720;
	int bezelBorderInPixels = 30;
	
	// Set up a camera that covers all the screens, then before rendering we can set the view for each camera and render the scene
	tiledCameraView.init( singleScreenWidth, singleScreenHeight, maxScreenAmount, 1, bezelBorderInPixels );
	
	gridMesh = createGridMesh( 1000, 1000, 10, 10 );

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::update()
{
	float currTime = 0.0f;
	
	// grab the current time, depending on who we are
	if( isServer )
	{
		currTime = ofGetElapsedTimef();
	}
	else
	{
		currTime = commonTimeOsc->getTimeSecs();
	}

	// If we are the server, we add some particles from time to time
	if( isServer )
	{
		
	}
	
	// Update

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::draw()
{
	float currTime = 0.0f;
	
	// grab the current time, depending on who we are
	if( isServer ) { currTime = ofGetElapsedTimef(); } else { currTime = commonTimeOsc->getTimeSecs(); }

	ofSetColor(255);
	
	fontLarge.drawString( "Screen: " + ofToString(screenIndex) + "  Time: " + ofToString( currTime, 2), 7, 45 );
	
	if( isServer )
	{
		fontLarge.drawString( "Server", 7, 85 );
	}
	else
	{
		fontSmall.drawString( "Offset: " + ofToString(commonTimeOsc->offsetMillis) + " OffsetTarget: " + ofToString(commonTimeOsc->offsetMillisTarget), 7, 80 );
	}
	
	// draw things
	ofPushView();
	
		setCamera( currTime );
	
		ofPushMatrix();
			ofTranslate( 0.0f, 0.0f, -fmod( currTime*250.0f, 100.0f) );
			//ofRotate(90, 0, 0, -1);
			//ofDrawGridPlane( 1000.0f, 10.0f );
			gridMesh->draw();
		ofPopMatrix();

		//ofDrawGrid(500, 10.0f, false, false, true, false );
	
	ofPopView();
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::setCamera( float _time )
{
	ofSetupScreenPerspective( ofGetWidth(), ofGetHeight(), ofGetOrientation(), false, cameraFov, 0.01f, 2048.0f );
	
	// Create a wandering motion for the camera
	float timeScale = 3.0f;
	
	ofVec3f time( _time, _time + 12.0f, _time + 25.23f );
	float lookAheadTime = 0.15f;
	
	ofVec3f origPos(0,30,-600);
	
	ofVec3f pos = origPos;
	pos.x += ofSignedNoise( time.x / timeScale ) * 200.0f;
	pos.y += ofSignedNoise( time.y / timeScale ) * 30.0f;
	pos.z += ofSignedNoise( time.z / timeScale ) * 40.0f;
	
	//ofVec3f center(0,pos.y,0);
	//center.x += ofSignedNoise( (_time + 123.45f ) / timeScale ) * 40.0f;
	//center.y += ofSignedNoise( (_time + 25.17f ) / timeScale ) * 40.0f;
	
	ofVec3f center = origPos + ofVec3f(0,0,100);
	center.x += ofSignedNoise( (time.x + lookAheadTime) / timeScale ) * 200.0f;
	center.y += ofSignedNoise( (time.y + lookAheadTime) / timeScale ) * 30.0f;
	center.z += ofSignedNoise( (time.z + lookAheadTime) / timeScale ) * 40.0f;
	
	ofVec3f up(0,1,0);
	
	ofMatrix4x4 lookAt;
	lookAt.makeLookAtViewMatrix( pos, center, up );
	ofLoadMatrix( lookAt );
}

/*
 perlinVector.x = perlinNoise.GetHeight(tmpTime * timeScaling, tmpTime * timeScaling );
 perlinVector.y = perlinNoise.GetHeight((tmpTime - (timeScaling/10.0f)) * timeScaling, (tmpTime + (timeScaling/ 6.0f)) * timeScaling );
 perlinVector.z = perlinNoise.GetHeight((tmpTime - (timeScaling/ 2.0f)) * timeScaling, (tmpTime + (timeScaling/10.0f)) * timeScaling );
 */

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::newData( DataPacket& _packet  )
{

}


// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::keyPressed(int key)
{
	if( key == 's' )
	{
		// If we haven't initialised the server, delete the client so that we can bind the ports we want
		if( server == NULL )
		{
			delete client;
			commonTimeOsc = NULL;
			server = new ServerOscManager();
		}
		
		if( !server->isInitialised() )
		{
			server->init( "Settings/ServerSettings.xml" );
		}
		
		isServer = server->isInitialised();
	}
	else if( key >= 48 && key <= 57 ) // change screen index with keys 0..9
	{
		screenIndex = key - 48;
	}
}


// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
ofMesh* testApp::createGridMesh( float _sizeX, float _sizeZ, int _resX, int _resZ )
{
	ofMesh* tmpMesh = new ofMesh();
	tmpMesh->setMode( OF_PRIMITIVE_LINES );
	
	for( int i = 0; i <= _resX; i++ )
	{
		float tmp = (float)i * (_sizeX / (float)_resX);
		tmpMesh->addVertex( ofVec3f(tmp, 0.0f , 0.0f)  + -ofVec3f((_sizeX/2.0f),0,(_sizeZ/2.0f)) );
		tmpMesh->addVertex( ofVec3f(tmp, 0.0f, _sizeZ) + -ofVec3f((_sizeX/2.0f),0,(_sizeZ/2.0f)) );
	}
	
	for( int i = 0; i <= _resZ; i++ )
	{
		float tmp = (float)i * (_sizeZ / (float)_resZ);
		tmpMesh->addVertex( ofVec3f(   0.0f, 0.0f, tmp) + -ofVec3f((_sizeX/2.0f),0,(_sizeZ/2.0f)) );
		tmpMesh->addVertex( ofVec3f( _sizeX, 0.0f, tmp) + -ofVec3f((_sizeX/2.0f),0,(_sizeZ/2.0f)) );
	}
	
	return tmpMesh;
}


// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::gotMessage(ofMessage msg)
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
void testApp::dragEvent(ofDragInfo dragInfo)
{
}