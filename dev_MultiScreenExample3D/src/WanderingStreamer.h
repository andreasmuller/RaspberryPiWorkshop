//
//  WanderingStreamer.h
//  MultiScreenExample3D
//
//  Created by Andreas Muller on 02/02/2013.
//
//

#pragma once

#include "SyncedAnimationObject.h"

class WanderingStreamer : public SyncedAnimationObject
{
	public:
	
		// ---------------------------------------------------------------
		WanderingStreamer( ofVec3f _startPoint, ofVec3f _endPoint,
						   float _startTime, float _animationLength,
						   ofFloatColor _color )
		{
			startPoint		= _startPoint;
			endPoint		= _endPoint;
			
			startTime		= _startTime;
			animationLength = _animationLength;
			
			color			= _color;
			
			animationFraction = -1.0f;
			
			//cout << "New WanderingStreamer " << startPoint << " " << endPoint << " " << startTime << " " << animationLength << endl;
			
			deleteMe 		= false;
		}
		
		// ---------------------------------------------------------------
		void update( float _currTime )
		{
			animationFraction = (_currTime - startTime) / animationLength;
			
			if( animationFraction > 1.0f )
			{
				// mark for deletion
				deleteMe = true;
			}
			
			currPos = startPoint.interpolated( endPoint, animationFraction );
			
			float noiseTime = ofGetElapsedTimef() * 0.5f;
			float noiseMagnitude = 50.0f;
			float noiseLiquidness = 1000.0f;
			
			ofVec3f noise( ofSignedNoise( currPos.x/noiseLiquidness, currPos.y/noiseLiquidness, currPos.z/noiseLiquidness, noiseTime ),
						   ofSignedNoise( currPos.z/noiseLiquidness, currPos.x/noiseLiquidness, currPos.y/noiseLiquidness, noiseTime ),
						   ofSignedNoise( currPos.y/noiseLiquidness, currPos.z/noiseLiquidness, currPos.x/noiseLiquidness, noiseTime ) );
			
			currPos += noise * noiseMagnitude;
			 
			positionHistory.push_front( currPos );
			
			// the length un world units of the 'tail' is obviously time step dependent, but let's overlook that for now.
			int maxHistoryLength = 20;
			if( positionHistory.size() > maxHistoryLength ) { positionHistory.pop_back(); }
			
		}
		
		// ---------------------------------------------------------------
		void draw()
		{
			if( animationFraction >= 0.0f && animationFraction <= 1.0f )
			{
				ofSetColor( color );
				
				ofSphere( positionHistory[0], 2 );
				
				// bail if we don't have enough points
				if( positionHistory.size() < 2 ) return;
				
				ofMatrix4x4 scratchLookAtMatrix;
				ofVec3f up(0,1,0);
				mesh.clear();
				for( unsigned int i = 0; i < positionHistory.size()-1; i++ )
				{
					mesh.addVertex( positionHistory[i] );
					mesh.addColor(color);
				}
				
				mesh.setMode( OF_PRIMITIVE_LINE_STRIP );
				mesh.draw();
			}
		}
		
		ofVec3f startPoint;
		ofVec3f endPoint;

		ofVec3f currPos;
	
		float animationFraction;
		
		float startTime;
		float animationLength;
		
		ofFloatColor color;
	
		ofMesh mesh;
		
		deque<ofVec3f> positionHistory;
	
	private:
};