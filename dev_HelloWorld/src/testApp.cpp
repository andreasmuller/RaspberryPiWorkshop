#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate( 60 );
	
	font.loadFont( "DIN.otf", 64 );
	
	frontBuffer = new ofFbo();
	backBuffer = new ofFbo();
	
	frontBuffer->allocate(ofGetWidth(), ofGetHeight(), GL_RGB );
	backBuffer->allocate(ofGetWidth(), ofGetHeight(), GL_RGB );
	
	frontBuffer->begin();
		ofClear(0);
	frontBuffer->end();
	
	backBuffer->bind();
		ofClear(0);
	backBuffer->unbind();
	
	message = "Hello World!";
}

//--------------------------------------------------------------
void testApp::update()
{
	screenMesh.clear();
	
	int resX = 30;
	int resY = 30;
	float stepX = ofGetWidth() /  (resX-1);
	float stepY = ofGetHeight() / (resY-1);
	
	float noiseLiquidness = 175;					// larger values make the movement more uniform, smaller more varied, changes how it all flows
	float noiseMagnitude = 5;						// how far do we displace the noise?
	float noiseTime = ofGetElapsedTimef() / 3.0f;	// the noise uses time as one dimension, we slow time down a little bit for a slower flow
	
	// add points making up a distorted mesh
	for( int y = 0; y < resY; y++ )
	{
		for( int x = 0; x < resX; x++ )
		{
			float tmpX = x * stepX;
			float tmpY = y * stepY;
			
			float noiseX = ofSignedNoise(tmpX/noiseLiquidness, tmpY/noiseLiquidness, noiseTime ) * noiseMagnitude;
			float noiseY = ofSignedNoise(tmpY/noiseLiquidness, tmpX/noiseLiquidness, noiseTime ) * noiseMagnitude;
			
			screenMesh.addVertex( ofVec2f(tmpX + noiseX, tmpY + noiseY ) );
			screenMesh.addTexCoord( ofVec2f(tmpX, tmpY) );
		}
	}
	
	// add triangles, we could do this once in the setup if we wanted and make sure not to clear the vertices.
	for( int y = 0; y < resY-1; y++ )
	{
		for( int x = 0; x < resX-1; x++ )
		{
			int tmpIndex = (y*resX) + x;
			screenMesh.addTriangle( tmpIndex, tmpIndex+1, tmpIndex+resY );
			screenMesh.addTriangle( tmpIndex+1, tmpIndex+resY+1, tmpIndex+resY );
		}
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofVec2f messageDrawPos( (frontBuffer->getWidth()/2.0f) - (font.stringWidth(message)/2.0f), (frontBuffer->getHeight()/2.0f) + (font.stringHeight(message)/2.0f) );
	
	// draw into front buffer, whatever is drawn in there will add colour into the distortion
	ofEnableAlphaBlending();
		frontBuffer->begin();
			// darken a little bit
			ofSetColor(0, 0, 0, 4); // changing the alpha here controls how long colour stays on screen once it is in the flow
			ofRect(0,0,frontBuffer->getWidth(),frontBuffer->getHeight() );
			ofSetColor( ofColor::fromHsb( ((sinf(ofGetElapsedTimef()/6.0f)+1.0f)/2.0f) * 255, 255, 200 ) ); // get ourselves a colour based on rotating the hue
			font.drawString(message, messageDrawPos.x, messageDrawPos.y );
		frontBuffer->end();
	ofDisableAlphaBlending();
	
	// draw the front bugger into the backbuffer using the distorted mesh we prepared earlier
	ofSetColor(255);
	backBuffer->begin();
		frontBuffer->getTextureReference().bind();
			screenMesh.draw();
		frontBuffer->getTextureReference().unbind();
	backBuffer->end();
	
	// swap front and back buffer
	ofFbo* tmp = backBuffer;
	backBuffer = frontBuffer;
	frontBuffer = tmp;
	
	// draw the front buffer to screen and also the text
	frontBuffer->draw(0,0);
	ofSetColor(255);
	font.drawString(message, messageDrawPos.x, messageDrawPos.y );
}
