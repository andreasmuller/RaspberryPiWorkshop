#include "testApp.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
bool testApp::shouldRemoveObject(SyncedAnimationObject* _p)
{
	return _p->deleteMe;
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
	ofSeedRandom( 1234 );
	
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
	
	lastTimeAddedObject = 0.0f;
	
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
	
	int maxScreenAmount = 5;
	int singleScreenWidth  = 1280;
	int singleScreenHeight = 720;
	int bezelBorderInPixels = 30;
	
	// debugging only
	/*float scale = 0.19f;
	singleScreenWidth  *= scale;
	singleScreenHeight *= scale;
	bezelBorderInPixels *= scale;
	for( int i = 0; i < maxScreenAmount; i++ )
	{
		ofFbo* tmpScreenSurface = new ofFbo();
		tmpScreenSurface->allocate( singleScreenWidth, singleScreenHeight, GL_RGB );
		screens.push_back( tmpScreenSurface );
	}
	 */
	
	// Set up a camera that covers all the screens, then before rendering we can set the view for each camera and render the scene
	tiledCameraView.init( singleScreenWidth, singleScreenHeight, maxScreenAmount, 1, bezelBorderInPixels );
	viewTile = true;
	
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
		float secsBetweenAddingParticles = 0.25f;
		if( (ofGetElapsedTimef() - lastTimeAddedObject) > secsBetweenAddingParticles )
		{
			createNewObject( currTime );
			
			lastTimeAddedObject = currTime;
		}
	}
	
	// Update
	for( unsigned int i = 0; i < sceneObjects.size(); i++ )
	{
		sceneObjects[i]->update( currTime );
	}
	
	
	// Remove any dead objects
	ofRemove( sceneObjects, shouldRemoveObject );
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::draw()
{
	float currTime = 0.0f;
	
	// grab the current time, depending on who we are
	if( isServer ) { currTime = ofGetElapsedTimef(); } else { currTime = commonTimeOsc->getTimeSecs(); }
	
	ofPushView();
	
		setCamera( currTime, screenIndex );
		drawScene( currTime );
		
		// debugging
		/*for( unsigned int i = 0; i < screens.size(); i++ )
		{
			ofFbo* tmpScreen = screens.at(i);
			
			tmpScreen->begin();
				ofClear( 0.2f, 0.2f, 0.2f );
				ofPushView();
					setCamera( currTime, i );
					drawScene( currTime );
				ofPopView();
			tmpScreen->end();
		}*/
		 
	ofPopView();
	
	ofSetColor(255);
	
	//debug
	// draw the screen surfaces one next to another
	/*ofSetColor( 255 );
	ofVec2f tmpPos(0, 200);
	for( unsigned int i = 0; i < screens.size(); i++ )
	{
		screens.at(i)->draw( tmpPos.x, tmpPos.y + screens.at(i)->getHeight(), screens.at(i)->getWidth(), -screens.at(i)->getHeight() );
		ofLine( tmpPos.x, tmpPos.y, tmpPos.x, tmpPos.y + screens.at(i)->getHeight() );
		tmpPos.x += tiledCameraView.getTileWidthNoBorder();
		tmpPos.x += tiledCameraView.getBorderWidth();
		if( (tmpPos.x + screens.at(i)->getWidth()) > ofGetWidth() )
		{
			tmpPos.x = 0.0f;
			tmpPos.y += tiledCameraView.getTileHeightNoBorder();
			tmpPos.y += tiledCameraView.getBorderHeight();
		}
	}*/
	
	// Draw some information to the screen as well
	fontLarge.drawString( "Screen: " + ofToString(screenIndex) + "  Time: " + ofToString( currTime, 2), 7, 45 );
	
	if( isServer )
	{
		fontLarge.drawString( "Server", 7, 85 );
	}
	else
	{
		fontSmall.drawString( "Offset: " + ofToString(commonTimeOsc->offsetMillis) + " OffsetTarget: " + ofToString(commonTimeOsc->offsetMillisTarget), 7, 80 );
	}
	
	ofSetColor( 128, 128, 128 );
	fontSmall.drawString( "fps: " + ofToString( ofGetFrameRate(), 1), 5, ofGetHeight() - 8 );
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::drawScene( float _time )
{
	ofSetColor(255);
	ofPushMatrix();
		ofTranslate( 0.0f, 0.0f, -fmod( _time*250.0f, 100.0f) );
		gridMesh->draw();
	ofPopMatrix();
	
	for( unsigned int i = 0; i < sceneObjects.size(); i++ )
	{
		sceneObjects[i]->draw();
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::setCamera( float _time, int _screenIndex  )
{
	if( viewTile )
	{
		tiledCameraView.setPerspectiveTransformForTile( _screenIndex );
	}
	else
	{
		ofSetupScreenPerspective( ofGetWidth(), ofGetHeight(), ofGetOrientation(), false, cameraFov, 0.01f, 2048.0f );
	}
	
	// Create a wandering motion for the camera
	float timeScale = 3.0f;
	
	ofVec3f time( _time, _time + 12.0f, _time + 25.23f );
	float lookAheadTime = 0.15f;
	
	ofVec3f origPos(0,30,-600);
	
	ofVec3f pos = origPos;
	pos.x += ofSignedNoise( time.x / timeScale ) * 200.0f;
	pos.y += ofSignedNoise( time.y / timeScale ) * 30.0f;
	pos.z += ofSignedNoise( time.z / timeScale ) * 40.0f;
	
	ofVec3f center = origPos + ofVec3f(0,0,100);
	center.x += ofSignedNoise( (time.x + lookAheadTime) / timeScale ) * 200.0f;
	center.y += ofSignedNoise( (time.y + lookAheadTime) / timeScale ) * 30.0f;
	center.z += ofSignedNoise( (time.z + lookAheadTime) / timeScale ) * 40.0f;
	
	ofVec3f up(0,1,0);
	
	ofMatrix4x4 lookAt;
	lookAt.makeLookAtViewMatrix( pos, center, up );
	ofLoadMatrix( lookAt );
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::createNewObject( float _currTime )
{	
	ofVec3f startPos(	ofRandom(-700.0f, 700.0f), ofRandom(10.0f, 150.0f), ofRandom(600.0, 800.0f) );
	ofVec3f endPos(		startPos.x, startPos.y, ofRandom( -600.0f, -600.0f) );
	
	//ofVec3f startPos(	ofRandom(-40.0f, 40.0f), ofRandom(100.0f, 100.0f), ofRandom(600.0, 800.0f) );
	//ofVec3f endPos(		startPos.x, startPos.y, ofRandom( -400.0f, -400.0f) );
	
	float particleStartTime = _currTime + 3.0f; // start the particle 3 seconds from now to give it plenty of time to get there if we are using a slow TCP connection
	float particleLifeDuration = 3.0f; //startPos.distance( endPos ) * 0.05f;
	
	ofFloatColor tmpColor;
	tmpColor.setHsb( ofRandom(1.0f), 0.7f, 0.5f );
	
	DataPacket tmpPacket;
	
	int type = 0;
	
	tmpPacket.numbersInt.push_back( type );
	
	tmpPacket.numbersFloat.push_back( particleStartTime );
	tmpPacket.numbersFloat.push_back( particleLifeDuration );
	
	tmpPacket.numbersFloat.push_back( startPos.x );
	tmpPacket.numbersFloat.push_back( startPos.y );
	tmpPacket.numbersFloat.push_back( startPos.z );
	
	tmpPacket.numbersFloat.push_back( endPos.x );
	tmpPacket.numbersFloat.push_back( endPos.y );
	tmpPacket.numbersFloat.push_back( endPos.z );
	
	tmpPacket.numbersFloat.push_back( tmpColor.r );
	tmpPacket.numbersFloat.push_back( tmpColor.g );
	tmpPacket.numbersFloat.push_back( tmpColor.b );
	
	server->sendData(tmpPacket.numbersInt, tmpPacket.numbersFloat);
	
	newData( tmpPacket );
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::newData( DataPacket& _packet  )
{
	ofVec3f startPos;
	ofVec3f endPos;
	
	ofFloatColor color;
	
	int type = _packet.numbersInt.at(0);
	
	float startTime = _packet.numbersFloat.at(0);
	float animationLength = _packet.numbersFloat.at(1);
	
	startPos.x = _packet.numbersFloat.at(2);
	startPos.y = _packet.numbersFloat.at(3);
	startPos.z = _packet.numbersFloat.at(4);
	
	endPos.x = _packet.numbersFloat.at(5);
	endPos.y = _packet.numbersFloat.at(6);
	endPos.z = _packet.numbersFloat.at(7);
	
	color.r = _packet.numbersFloat.at(8);
	color.g = _packet.numbersFloat.at(9);
	color.b = _packet.numbersFloat.at(10);
	
	sceneObjects.push_back( new WanderingStreamer(startPos, endPos, startTime, animationLength, color) );
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
	else if ( key == 't' )
	{
		viewTile = !viewTile;
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