/*
 *  MathHelpers.h
 *  HyundaiLightRevealControlSoftware
 *
 */

#pragma once


#include "ofMain.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
class MathHelpers 
{
public:

	// ------------------------------------------------------------
	static float cosCurve( float _valueIn, float _maxValue )
	{
		float tmpVal = _valueIn / _maxValue;
		tmpVal = cosf( tmpVal * (PI*2.0f) );
		tmpVal += 1.0f;
		tmpVal /= 2.0f;
		
		return tmpVal;
	}

	// ------------------------------------------------------------
	static float sinCurve( float _valueIn, float _maxValue )
	{
		float tmpVal = _valueIn / _maxValue;
		tmpVal = sinf( tmpVal * (PI*2.0f) );
		tmpVal += 1.0f;
		tmpVal /= 2.0f;
		
		return tmpVal;
	}	
	
	// ------------------------------------------------------------	
	static float cosZeroToOne( float _angleRad )
	{
		float tmpVal = (cosf( _angleRad ) + 1.0f) * 0.5f;
		return tmpVal;
	}		
	
	// ------------------------------------------------------------	
	static float sinZeroToOne( float _angleRad )
	{
		float tmpVal = (sinf( _angleRad ) + 1.0f) * 0.5f;
		return tmpVal;
	}	
	
	// ------------------------------------------------------------	
	static float smoothstep(float edge0, float edge1, float x)
	{
		// Scale, and clamp x to 0..1 range
		x = ofClamp( (x - edge0)/(edge1 - edge0), 0, 1);
		// Evaluate polynomial
		return x*x*x*(x*(x*6 - 15) + 10);
	}

	// ------------------------------------------------------------		
	// Smoothstep in and out, for instance a bell curve would just be:
	// (_t, 0.0, 0.5, 0.5, 1.0)
	// To delay the start a little bit:
	// (_t, 0.1, 0.5, 0.5, 1.0)
	// To stay on full most of the time with a short in and out:
	// (_t, 0.0, 0.05, 0.95, 1.0)
	// Can also cross the middle point:
	// (_t, 0.0, 0.05, 0.35, 1.0)
	//
	static float smoothStepInOut( float _t, float _low0, float _high0, float _high1, float _low1 )
	{
		
		if( _t < _low0 )
		{
			return 0.0f;
		}
		else if( _t >= _low0 && _t < _high0 )
		{
			// transofrm _t into this space
			_t -= _low0;
			_t /= (_high0-_low0);
			return smoothstep( 0.0f, 1.0f, _t );
		}
		else if( _t >= _high0 && _t < _high1 )
		{
			return 1.0f;
		}
		else if( _t > _high1 && _t < _low1 )
		{
			_t -= _high1;
			_t /= (_low1-_high1);
			return smoothstep( 0.0f, 1.0f, 1.0f - _t );
		}
		else if( _t >= _low1 )
		{
			return 0.0f;
		}
		
		return 0.0f;  
	}	

	
	// ------------------------------------------------------------	
	// give a tick, a max tick and how many ticks to spend fading 
	// it will go 0..1 -> 1 -> 1..0
	//
	static float tickToAlphaInOut(  int _currTick, int _maxTick, int _fadeLength )
	{
		float tmpA = 0.0f;
		
		if( _currTick < 0 ) { tmpA = 0.0f; }
		
		else if( (_currTick >= 0) && (_currTick <= _maxTick) )
		{
			int tmpTick		= _currTick;
			int animLength	= _maxTick;
			
			if( tmpTick < _fadeLength ) { tmpA = (float)tmpTick / (float)_fadeLength; }
			else if ( tmpTick > (animLength-_fadeLength) ) { tmpA =  1.0f - ((float)(tmpTick-(animLength-_fadeLength)) / (float)_fadeLength) ; }
			else { tmpA = 1.0f; }
		}
		else if( (_currTick > _maxTick) ) { tmpA = 0.0f; }
		
		return tmpA;	
	}

	// ------------------------------------------------------------	
	// give a frac, how long of the 0..1 to spend fading,
	// it will go 0..1 -> 1 -> 1..0
	//
	static float fracToAlphaInOut(  float _currFrac, float _fadeFracLength )
	{
		float tmpA = 0.0f;
		
		if( _currFrac < 0.0f ) { tmpA = 0.0f; }
		
		else if( (_currFrac >= 0) && (_currFrac <= 1.0f) )
		{
			float tmpTick		= _currFrac;
			float animLength	= 1.0f;
			
			if( tmpTick < _fadeFracLength ) { tmpA = tmpTick / _fadeFracLength; }
			else if ( tmpTick > (animLength-_fadeFracLength) ) { tmpA =  1.0f - ((tmpTick-(animLength-_fadeFracLength)) / _fadeFracLength) ; }
			else { tmpA = 1.0f; }
		}
		else if( (_currFrac > 1.0f) ) { tmpA = 0.0f; }
		
		return tmpA;	
	}

	
	// DOES NOT WORK
	// Random point in a sphere, not terribly accurate
	static void randomPointOnUnitSphere( float& _x, float& _y, float& _z )
	{
		float phi   = ofRandom( 0.0f, PI * 2.0f );
		float theta = ofRandom( 0.0f, PI * 0.5f );  
		float phiSqrt = sqrtf(phi);
		float x = cos(phiSqrt) * cos(theta);
		float y = cos(phiSqrt) * sin(theta);
		float z = sin(phiSqrt);
		
		_x = x;
		_y = y;
		_z = z;		
	}
	
	// ------------------------------------------------------------		
	static void randomPointOnUnitSphereFromCube( float& _x, float& _y, float& _z )
	{
		_x = ofRandomf();
        _y = ofRandomf();
        _z = ofRandomf();
		
        float tmpL = sqrtf( _x * _x  + _y * _y + _z * _z);
 
		_x /= tmpL;
		_y /= tmpL;
		_z /= tmpL;		
	}	

	
	// ------------------------------------------------------------
	// take _image and fits it scaling proportionally into _screen
	static ofRectangle fitRectangleIntoRectangle( ofRectangle _image, ofRectangle _screen, bool _fitWidth = true )
	{
		ofRectangle resultRect;
		
		float ratio = _screen.width / _image.width;
		
		float contentDrawWidth  = _screen.width;
		float contentDrawHeight = _image.height * ratio;
		
		if( _fitWidth )
		{
			if( contentDrawWidth < _screen.width )
			{
				ratio = _image.height * _screen.height;
				contentDrawWidth  = _screen.width;
				contentDrawHeight = _image.height * ratio;			
			}			
		}
		else 
		{
			if( contentDrawHeight < _screen.height )
			{
				ratio = _screen.height / _image.height;
				contentDrawHeight = _screen.height;
				contentDrawWidth  = _image.width * ratio;
			}
		}

		
		resultRect.x = -( contentDrawWidth  - _screen.width ) / 2;
		resultRect.y = -( contentDrawHeight - _screen.height ) / 2;
		
		resultRect.width  = contentDrawWidth;
		resultRect.height = contentDrawHeight;
		
		return resultRect;
	}
	
		
private:
	
};
