#include "testApp.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::setup()
{
	isServer = false;
	
	fontSmall.loadFont("Fonts/DIN.otf", 8 );
	fontLarge.loadFont("Fonts/DIN.otf", 36 );
	
	ofSeedRandom();
	int uniqueID = ofRandom( 999999999 ); // yeah this is bogus I know. Todo: generate a unique computer ID.
	ofSeedRandom( 1234 ); // now random numbers will be the same across machines
	
	clientCanSend = false;
	
	if( ofFile( ofToDataPath("Settings/IsServer.txt")).exists() )
	{
		// If we are the server, set some defaults, try to read from file and then set up our sender and receiver.
		// We are going to want to send to the multicast address of our subnet.
		
		serverSendHost	= "192.168.1.255";
		serverSendPort		= 7778;
		serverReceivePort	= 7777;
		
		ofxXmlSettings XML;
		bool loadedFile = XML.loadFile( "Settings/ServerSettings.xml" );
		if( loadedFile )
		{
			serverSendHost = XML.getValue("Settings:ServerSendHost",		"192.168.1.255");
			serverSendPort = XML.getValue("Settings:ServerSendPost",		7778);
			serverReceivePort = XML.getValue("Settings:ServerReceivePort",	7777);
		}
		
		sender.setup( serverSendHost, serverSendPort);
		receiver.setup( serverReceivePort );
		
		// we periodically send out a "hello" message that everyone in the subnet listening to the right port will receive when they
		// get this, they also get the IP address of the server, this is more flexible than having that in a text file on each client
		serverLastSentHelloMessageMillis = ofGetElapsedTimeMillis();
		serverMilliseBetweenHelloMessages = 3 * 1000;
	}
	else
	{
		receiver.setup( 7778 );
		
		initClientCV();
	}
		
	screenIndex = 0;
	
	// Read the screen index from a file
	ofxXmlSettings XML;
	bool loadedFile = XML.loadFile( "Settings/ClientSettings.xml" );
	if( loadedFile )
	{
		screenIndex = XML.getValue("Settings:ScreenIndex", 0);
	}
	
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::initClientCV()
{
	videoWidth = 320;
	videoHeight = 240;
#ifdef _USE_LIVE_VIDEO
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(videoWidth,videoHeight);
#else
	string videoPath = "Movies/fingers.mov"; //"/home/pi/openFrameworks/examples/addons/opencvExample/bin/data/fingers.mov";
	vidPlayer.loadMovie( videoPath );
	vidPlayer.setLoopState(OF_LOOP_NORMAL);
	vidPlayer.play();
#endif
	
    colorImg.allocate(videoWidth, videoHeight);
	grayImage.allocate(videoWidth, videoHeight);
	grayBg.allocate(videoWidth, videoHeight);
	grayDiff.allocate(videoWidth, videoHeight);
	
	bLearnBakground = true;
	threshold = 80;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::update()
{
	if( isServer )
	{
		serverUpdate();
	}
	else
	{
		clientUpdate();
	}
	
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::serverUpdate()
{
	if( ( ofGetElapsedTimeMillis() - serverLastSentHelloMessageMillis) > serverMilliseBetweenHelloMessages )
	{
		ofxOscMessage m;
		m.setAddress("/hello");
		m.addIntArg( serverReceivePort ); // add the port we would like to use to receive messages as an argument
		
		sender.sendMessage( m );
		
		serverLastSentHelloMessageMillis = ofGetElapsedTimeMillis();
	}
	
	// check for waiting messages
	while( receiver.hasWaitingMessages() )
	{
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		
		if( m.getAddress() == "/newBlob" )
		{
			int tmpScreenIndex = m.getArgAsInt32(0);
			
			NodeData* tmpNodeDat = NULL;
			
			// If it's exists, we update, otherwise it's new
			if( nodeData.find(tmpScreenIndex) != nodeData.end() )
			{
				tmpNodeDat = nodeData.at( tmpScreenIndex );				
			}
			else
			{
				tmpNodeDat = new NodeData();
				nodeData.insert( screenIndexNodeDataPointerPair( tmpScreenIndex, tmpNodeDat ) );
			}
			
			if( tmpNodeDat != NULL )
			{
				int tmpFrameNum = m.getArgAsInt32(1);
				
				// if we have a higher frame number than saved here, we clear the data, otherwise we append to it
				if( tmpNodeDat->frameNum < tmpFrameNum )
				{
					tmpNodeDat->frameData.clear();
					tmpNodeDat->frameNum = tmpFrameNum;
				}
				
				ofPolyline tmpPolyline;
				for( int i = 2; i < m.getNumArgs(); i += 2 )
				{
					float tmpX = m.getArgAsFloat( i );
					float tmpY = m.getArgAsFloat( i + 1 );
					tmpPolyline.addVertex( ofVec2f(tmpX, tmpY) );
				}
				
				tmpNodeDat->frameData.push_back( tmpPolyline );
			}
		}
	}
}


// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::clientUpdate()
{
	// check for waiting messages
	while( receiver.hasWaitingMessages() )
	{
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		
		if( m.getAddress() == "/hello" )
		{
			// if we get a hello message and we haven't set up our sender, get the IP and port from the hello message
			if( !clientCanSend )
			{
				int sendPort = m.getArgAsInt32(0);
				sender.setup( m.getRemoteIp(), sendPort );
				clientCanSend = true;
			}
		}
	}
	
	// Computer vision
	bool bNewFrame = false;
	
#ifdef _USE_LIVE_VIDEO
	vidGrabber.update();
	bNewFrame = vidGrabber.isFrameNew();
#else
	vidPlayer.update();
	bNewFrame = vidPlayer.isFrameNew();
#endif
	
	if (bNewFrame)
	{
#ifdef _USE_LIVE_VIDEO
		colorImg.setFromPixels(vidGrabber.getPixels(), videoWidth,videoHeight);
#else
		colorImg.setFromPixels(vidPlayer.getPixels(), videoWidth,videoHeight);
#endif
		
        grayImage = colorImg;
		if (bLearnBakground == true)
		{
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}
		
		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);
		
		// find contours which are between the size of 0 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, (videoWidth*videoHeight)/3, 10, true);	// find holes
		
		// In the real world we would most likely pass this data off to something like the ofxCv::Tracker
		// first in order to get a consistent IDs for the blobs and other niceties.
		
		if( contourFinder.nBlobs > 0 && clientCanSend )
		{
			for (int blobIndex = 0; blobIndex < contourFinder.nBlobs; blobIndex++)
			{
				ofxOscMessage m;
				m.setAddress("/newBlob");
				m.addIntArg( screenIndex );
				m.addIntArg( ofGetFrameNum() ); 
		
				// Normally we would need to limit the amount of points we send over, if not
				// you might see some odd behaviour. ofPolyline::getResampledByCount is your friend there.
				
				ofxCvBlob* tmpBlob = &contourFinder.blobs.at(blobIndex);
				for( unsigned int i = 0; i < tmpBlob->pts.size(); i++ )
				{
					m.addFloatArg( tmpBlob->pts.at(i).x );
					m.addFloatArg( tmpBlob->pts.at(i).y );
				}
				
				sender.sendMessage( m );
			}
		}
	}
	
}



// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::draw()
{
	
	ofSetColor(255);

	if( isServer )
	{
		serverDraw();
	}
	else
	{
		clientDraw();
	}
	
	// Draw some information to the screen as well
	fontLarge.drawString( "Screen: " + ofToString(screenIndex), 7, 45 );
	
	if( isServer )
	{
		fontLarge.drawString( "Server", 7, 85 );
	}
	else
	{
	}
	
	ofSetColor( 128, 128, 128 );
	fontSmall.drawString( "fps: " + ofToString( ofGetFrameRate(), 1), 5, ofGetHeight() - 8 );
}


// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::serverDraw()
{
	
	for ( map<int, NodeData*>::iterator it = nodeData.begin(); it != nodeData.end(); ++it )
	{
		//std::cout << it->first << " => " << it->second << '\n';
		int screenIndex = it->first;
		NodeData* tmpNodeData = it->second;
		
		int tmpVideoWidth = 320;
		int tmpVideoHeight = 240;
		
		float tmpX = screenIndex * tmpVideoWidth;
		float tmpY = 0.0f;

		ofPushMatrix();
			ofTranslate( tmpX, tmpY );
			for(unsigned int i = 0; i < tmpNodeData->frameData.size(); i++ )
			{
				tmpNodeData->frameData.at(i).draw();
			}
		ofPopMatrix();
	}

}


// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::clientDraw()
{
	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetColor(ofColor::white);
	
	ofPushMatrix();
	ofTranslate(0, 0);
	colorImg.draw(0, 0);
	//drawVideoLabel("colorImg");
	ofPopMatrix();
	
	ofPushMatrix();
	ofTranslate(videoWidth, 0);
	grayImage.draw(0, 0);
	//drawVideoLabel("grayImage");
	ofPopMatrix();
	
	ofPushMatrix();
	ofTranslate(0, videoHeight);
	grayBg.draw(0, 0);
	//drawVideoLabel("grayBg");
	ofPopMatrix();
	
	ofPushMatrix();
	ofTranslate(videoWidth, videoHeight);
	grayDiff.draw(0, 0);
	//drawVideoLabel("grayDiff");
	ofPopMatrix();
	
	// then draw the contours:
	ofPushMatrix();
	ofTranslate(videoWidth*2, 0, 0);
	ofFill();
	ofSetColor(ofColor::gray);
	ofRect(0, 0, videoWidth, videoHeight);
	ofSetColor(ofColor::white);
	
	// we could draw the whole contour finder
	//contourFinder.draw(0, 0);
	
	// or, instead we can draw each blob individually from the blobs vector,
	// this is how to get access to them:
	for (int i = 0; i < contourFinder.nBlobs; i++){
		contourFinder.blobs[i].draw(0, 0);
		
		// draw over the centroid if the blob is a hole
		ofSetColor(255);
		if(contourFinder.blobs[i].hole){
			ofDrawBitmapString("hole",
							   contourFinder.blobs[i].boundingRect.getCenter().x,
							   contourFinder.blobs[i].boundingRect.getCenter().y);
		}
	}
	// finally, a report:
	ofSetColor(ofColor::white);
	stringstream reportStr;
	reportStr	<< "bg subtraction and blob detection"			<< endl
	<< "press ' ' to capture bg"					<< endl
	<< "threshold " << threshold << " (press: +/-)" << endl
	<< "num blobs found " << contourFinder.nBlobs << ", fps: " << ofGetFrameRate();
	ofDrawBitmapString(reportStr.str(), 20, videoHeight+20);
	ofPopMatrix();
}


// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::keyPressed(int key)
{
	if( key == 's' )
	{
		//isServer = !isServer;
	}
	else if( key >= 48 && key <= 57 ) // change screen index with keys 0..9
	{
		screenIndex = key - 48;
	}
	else if ( key == 't' )
	{
	}
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