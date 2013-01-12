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
	
		// ---------------------------------------------------------------
		Particle( ofVec2f _startPoint, ofVec2f _endPoint,
				  float _startTime, float _animationLength,
				  ofFloatColor _color )
		{
			startPoint		= _startPoint;
			endPoint		= _endPoint;
			
			startTime		= _startTime;
			animationLength = _animationLength;
			
			color			= _color;
			
			animationFraction = 0.0f;
			
			cout << "New Particle " << startPoint << " " << endPoint << " " << startTime << " " << animationLength << endl;
			
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
			
			//timeFrac = ofClamp( timeFrac, 0.0f, 1.0f );
			
			currPos = startPoint.interpolated( endPoint, animationFraction );
		}
	
		// ---------------------------------------------------------------
		void draw()
		{
			if( animationFraction >= 0.0f && animationFraction <= 1.0f )
			{
				ofSetColor( color );
				ofCircle( currPos, 30 );
				
				ofCircle( startPoint, 3 );
				ofCircle( endPoint, 3 );				
				ofLine( startPoint, endPoint );
			}
		}
	
		ofVec2f currPos;
	
		ofVec2f startPoint;
		ofVec2f endPoint;
		
		float animationFraction;
	
		float startTime;
		float animationLength;
		
		ofFloatColor color;
	
		bool	deleteMe;
};