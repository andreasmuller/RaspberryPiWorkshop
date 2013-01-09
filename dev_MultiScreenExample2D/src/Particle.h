//
//  Particle.h
//  MultiScreenExample2D
//
//  Created by Andreas Müller on 09/01/2013.
//
//

#pragma once

class Particle
{
	public:
	
		Particle( ofVec2f _startPoint, ofVec2f _endPoint, float _startTime, float _animationLength, ofColor _color )
		{
			startPoint		= _startPoint;
			endPoint		= _endPoint;
			
			startTime		= _startTime;
			animationLength = _animationLength;
			
			color			= _color;
			
			deleteMe 		= false;
		}
	
		void update( float _currTime )
		{
			float timeFrac = (_currTime - startTime) / animationLength;

			if( timeFrac > 1.0f )
			{
				// mark for deletion
				deleteMe = true;
			}
			
			timeFrac = ofClamp( timeFrac, 0.0f, 1.0f );
			
			currPos = startPoint.interpolated( endPoint, timeFrac );
		}
	
		void draw()
		{
			ofSetColor( color );
			ofCircle( currPos, 30 );
		}
	
		ofVec2f currPos;
	
		ofVec2f startPoint;
		ofVec2f endPoint;
	
		float startTime;
		float animationLength;
		
		ofColor color;
	
		bool	deleteMe;
};