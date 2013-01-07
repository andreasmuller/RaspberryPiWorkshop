#pragma once

#include "ofMain.h"

//#include "cinder-lite/CinderMath.h"
//#include "cinder-lite/Color.h"
//#include "cinder-lite/Vector.h"

#include <string>
#include <vector>
#include <map>

using namespace std;

namespace msa {
	
#define MSA_HOST_SUFFIX		"-OF"
	
#if defined (TARGET_OSX)
#define MSA_TARGET_OSX
	
#elif defined (TARGET_LINUX)
#define MSA_TARGET_LINUX
	
#elif defined (TARGET_WIN32)
#define MSA_TARGET_WIN32
	
#elif defined (TARGET_IPHONE)
#define MSA_TARGET_IPHONE)
#endif
	
#if defined (TARGET_OPENGLES)
#define MSA_TARGET_OPENGLES
#endif
	
    typedef ofVec2f         Vec2f;
    typedef ofVec3f         Vec3f;
    typedef ofVec4f         Vec4f;
    typedef ofFloatColor    Color;
    
	inline string dataPath(string path, bool absolute = false)		{	return ofToDataPath(path, absolute);	}
	
	inline double getElapsedSeconds()								{	return ofGetElapsedTimef(); }
	inline long int getElapsedFrames()								{	return ofGetFrameNum(); }
	
	inline int getWindowWidth()										{	return ofGetWidth(); }
	inline int getWindowHeight()									{	return ofGetHeight(); }
	inline float getWindowAspectRatio()								{	return getWindowWidth() * 1.0f / getWindowHeight(); }
	inline Vec2f getWindowSize()									{	return ofGetWindowSize(); }
	inline Vec2f getWindowCenter()									{	return Vec2f(getWindowWidth() * 0.5f, getWindowHeight() * 0.5f ); } 
	
	inline void drawString(string s, float x, float y)				{	ofDrawBitmapString(s, x, y); }
	
    
    inline float clamp(float a, float min, float max) {
        return ofClamp(a, min, max);
    }
    
    
	class Rand {
	public:
		
		static float randFloat()									{	return ofRandomf(); }
		static float randFloat(float f)								{	return ofRandom(0, f);	}
		static float randFloat(float a, float b)					{	return ofRandom(a, b);	}
		
		//! returns a random Vec3f that represents a point on the unit circle
		static Vec3f randVec3f() {
			float phi = randFloat( (float)M_PI * 2.0f );
			float costheta = randFloat( -1.0f, 1.0f );
			
			float rho = sqrt( 1.0f - costheta * costheta ); 
			float x = rho * cos( phi );
			float y = rho * sin( phi );
			float z = costheta;
			
			return Vec3f( x, y, z );
		}
		
		//! returns a random Vec2f that represents a point on the unit circle
		static Vec2f randVec2f() {
			float theta = randFloat( (float)M_PI * 2.0f );
			return Vec2f( cos( theta ), sin( theta ) );
		}
	};
	
	
}