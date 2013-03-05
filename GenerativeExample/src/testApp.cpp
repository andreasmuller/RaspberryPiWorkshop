#include "testApp.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::setup()
{
	ofBackground( 0, 0, 0 );

	ofHideCursor();
	
	fontSmall.loadFont("Fonts/DIN.otf", 8 );
	fontLarge.loadFont("Fonts/DIN.otf", 36 );

	// Initialize particles
	int particleAmount = 400;
	for( int i = 0; i < particleAmount; i++ )
	{
		Particle* tmpParticle = new Particle();
		tmpParticle->pos.set( ofRandomWidth(), ofRandomHeight() );
		
		float tmpAngle = ofRandom( PI * 2.0f );
		float magnitude = 20.0f; // pixels per second
		tmpParticle->vel.set( cosf(tmpAngle) * magnitude, sinf(tmpAngle) * magnitude );
		
		tmpParticle->spacePartitioningIndexX = 0;
		tmpParticle->spacePartitioningIndexY = 0;
		
		tmpParticle->myID = i;
		
		particles.push_back( tmpParticle );
	}
	
	// Initialize storage we will use to optimize particle-to-particle distance checks
	spacePartitioningResX = 20;
	spacePartitioningResY = 20;
	
	spacePartitioningGridWidth = ofGetWidth() / (float)spacePartitioningResX;
	spacePartitioningGridHeight= ofGetHeight() / (float)spacePartitioningResY;
	
	for( int y = 0; y < spacePartitioningResY; y++ )
	{
		spacePartitioningGrid.push_back( vector< vector< Particle* > >() );
		for( int x = 0; x < spacePartitioningResX; x++ )
		{
			spacePartitioningGrid.at(y).push_back( vector< Particle* >() );
		}
	}
	
	debugDraw = false;
	
	lastUpdateTime = ofGetElapsedTimef();
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::update()
{
	float currTime = ofGetElapsedTimef();
	float timeDelta = currTime - lastUpdateTime;
	lastUpdateTime = currTime;
	
	float screenWidth = ofGetWidth();
	float screenHeight = ofGetHeight();
	
	// update particle positions
	for( unsigned int i = 0; i < particles.size(); i++ )
	{
		Particle* tmpParticle = particles.at(i);
		tmpParticle->pos += tmpParticle->vel * timeDelta;
		
		tmpParticle->pos.x = fmodf( tmpParticle->pos.x, screenWidth );
		if( tmpParticle->pos.x < 0.0f ) tmpParticle->pos.x += screenWidth;
		tmpParticle->pos.y = fmodf( tmpParticle->pos.y, screenHeight );
		if( tmpParticle->pos.y < 0.0f ) tmpParticle->pos.y += screenHeight;
	}
	
	// hackety hack.
	particles.at(0)->pos.set( ofGetMouseX(), ofGetMouseY() );
	
	// clear the space partitioning lists
	for( int y = 0; y < spacePartitioningResY; y++ )
	{
		for( int x = 0; x < spacePartitioningResX; x++ )
		{
			spacePartitioningGrid.at(y).at(x).clear();
		}
	}
	
	// add particles into the space partitioning grid
	for( unsigned int i = 0; i < particles.size(); i++ )
	{
		Particle* tmpParticle = particles.at(i);
		
		int tmpIndexX = tmpParticle->pos.x / spacePartitioningGridWidth;
		int tmpIndexY = tmpParticle->pos.y / spacePartitioningGridHeight;

		if( tmpIndexX < 0 )  tmpIndexX = 0;
		if( tmpIndexX >= spacePartitioningResX ) tmpIndexX = spacePartitioningResX-1;
		
		if( tmpIndexY < 0 )  tmpIndexY = 0;
		if( tmpIndexY >= spacePartitioningResY ) tmpIndexY = spacePartitioningResY-1;
		
		tmpParticle->spacePartitioningIndexX = tmpIndexX;
		tmpParticle->spacePartitioningIndexY = tmpIndexY;
		
		spacePartitioningGrid.at(tmpIndexY).at(tmpIndexX).push_back( tmpParticle );
	}

	// Now we update the line mesh, to do this we check each particle against every other particle, if they are
	// within a certain distance we draw a line between them. As this quickly becoems a pretty insane amount
	// of checks, we use our space partitioning scheme to optimize it all a little bit.
	
	lineMesh.clear();
	lineMesh.setMode( OF_PRIMITIVE_LINES );
	
	ofFloatColor scratchColor;
	scratchColor.set( 1.0f, 1.0f, 1.0f );
	
	float lineConnectionMaxDistance =  ofMap( cosf( currTime / 10.0f ) , -1.0f, 1.0f, 10.0f, 100.0f); //   ofGetMouseY() / 10.0f;
	float lineConnectionMaxDistanceSquared = lineConnectionMaxDistance * lineConnectionMaxDistance;

	// how many slots do we need to check on each side?
	int spacePartitioningIndexDistanceX = ceil(lineConnectionMaxDistance / spacePartitioningGridWidth);
	int spacePartitioningIndexDistanceY = ceil(lineConnectionMaxDistance / spacePartitioningGridHeight);
			
	for( unsigned int particleIndex = 0; particleIndex < particles.size(); particleIndex++ )
	{
		Particle* tmpParticle = particles.at(particleIndex);
		
		// the particle knows where it is in the space partitioning grid, figure out which indices to loop between based
		// on how many slots the maximum line distance  can cover, then do a bounds check.
		int startIndexX = tmpParticle->spacePartitioningIndexX - spacePartitioningIndexDistanceX;
		if( startIndexX < 0 ) { startIndexX = 0; } if( startIndexX >= spacePartitioningResX ) { startIndexX = spacePartitioningResX-1;}

		int endIndexX   = tmpParticle->spacePartitioningIndexX + spacePartitioningIndexDistanceX;
		if( endIndexX < 0 ) { endIndexX = 0; } if( endIndexX >= spacePartitioningResX ) { endIndexX = spacePartitioningResX-1;}
		
		int startIndexY = tmpParticle->spacePartitioningIndexY - spacePartitioningIndexDistanceY;
		if( startIndexY < 0 ) { startIndexY = 0; } if( startIndexY >= spacePartitioningResY ) { startIndexY = spacePartitioningResY-1;}
		
		int endIndexY   = tmpParticle->spacePartitioningIndexY + spacePartitioningIndexDistanceY;
		if( endIndexY < 0 ) { endIndexY = 0; } if( endIndexY >= spacePartitioningResY ) { endIndexY = spacePartitioningResY-1;}
		
		for( int y = startIndexY; y < endIndexY; y++ )
		{
			for( int x = startIndexX; x < endIndexX; x++ )
			{	
				for( unsigned int i = 0; i < spacePartitioningGrid.at(y).at(x).size(); i++ )
				{
					Particle* tmpOtherParticle = spacePartitioningGrid.at(y).at(x).at(i);
					if( tmpParticle->myID != tmpOtherParticle->myID )
					{
						ofVec2f diff = tmpParticle->pos - tmpOtherParticle->pos;
						if( diff.lengthSquared() < lineConnectionMaxDistanceSquared )
						{
							scratchColor.a =  1.0f - (diff.length() / lineConnectionMaxDistance);
							
							lineMesh.addVertex( tmpParticle->pos );
							lineMesh.addColor( scratchColor );

							lineMesh.addVertex( tmpOtherParticle->pos );
							lineMesh.addColor( scratchColor );
							
							lineMesh.addIndex( lineMesh.getNumVertices() - 2 );
							lineMesh.addIndex( lineMesh.getNumVertices() - 1 );
						}
					}
				}
			}
		}
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::draw()
{
	ofEnableAlphaBlending();
	
	if( debugDraw)
	{
		ofSetColor( 140, 140, 140 );
		for( int y = 0; y < spacePartitioningResY; y++ ) { ofLine( 0.0f, y * spacePartitioningGridHeight, ofGetWidth(), y * spacePartitioningGridHeight ); }
		for( int x = 0; x < spacePartitioningResY; x++ ) { ofLine( x * spacePartitioningGridWidth, 0.0f, x * spacePartitioningGridWidth, ofGetHeight() ); }
		
		ofSetColor( 100, 100, 100 );
		for( int y = 0; y < spacePartitioningResY; y++ )
		{
			for( int x = 0; x < spacePartitioningResX; x++ )
			{
				if( spacePartitioningGrid.at(y).at(x).size() > 0 )
				{
					ofRect( x * spacePartitioningGridWidth, y * spacePartitioningGridHeight, spacePartitioningGridWidth, spacePartitioningGridHeight);
				}
			}
		}
		
		ofSetColor( 255, 255, 255 );
		for( unsigned int i = 0; i < particles.size(); i++ )
		{
			ofCircle( particles.at(i)->pos, 3.0f );
		}
	}
	
	lineMesh.draw();
	
	ofDisableAlphaBlending();
		
	ofSetColor( 128, 128, 128 );
	fontSmall.drawString( "fps: " + ofToString( ofGetFrameRate(), 1), 5, ofGetHeight() - 8 );
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void testApp::keyPressed(int key)
{
	if( key == 'd' )
	{
		debugDraw = !debugDraw;
	}
}
