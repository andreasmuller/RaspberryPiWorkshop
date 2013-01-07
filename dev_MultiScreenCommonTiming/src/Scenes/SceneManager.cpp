//
//  SceneManager.cpp
//  MultiScreenCommonTiming
//
//  Created by Andreas Müller on 25/12/2012.
//
//

#include "SceneManager.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
SceneManager::SceneManager()
{
	sceneIndex = 0;
	currAnimationTimeSecs = 0.0f;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
SceneManager::~SceneManager()
{

}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void SceneManager::update( float _timeSecs )
{
	currAnimationTimeSecs = _timeSecs;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void SceneManager::draw()
{
	if( sceneIndex == 0 )
	{
		ofColor backgroundColor( 82, 132, 200 );
		ofBackground( backgroundColor );
		
		ofSetColor( 82, 179, 200 );
		float tmpRadius = ofGetHeight() * 0.45f;
		ofPushMatrix();
			ofTranslate( ofGetWidth()/2, ofGetHeight()/2 );
			ofRotate( currAnimationTimeSecs * 20.0f );
			ofTranslate( tmpRadius, 0 );
			ofCircle( 0,0, 40 );
		ofPopMatrix();
	}
	else if ( sceneIndex == 1 )
	{
		int col = ((sinf( currAnimationTimeSecs ) + 1.0f) * 0.5f) * 255;
		ofBackground( col, col, col );
	}
	else
	{
	}
	
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void SceneManager::changeScene( int _sceneIndex )
{
	sceneIndex = _sceneIndex;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
int SceneManager::getSceneIndex()
{
	return sceneIndex;
}