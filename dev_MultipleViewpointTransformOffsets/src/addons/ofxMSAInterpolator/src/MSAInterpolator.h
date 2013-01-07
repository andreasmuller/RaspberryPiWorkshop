/**************************** InterpolatorT Classes ****************************
Usage:
	msa::InterpolatorT<float>		myInterpolator1;		// create spline of floats
	msa::InterpolatorT<myDataType>	myInterpolator2;		// create spline of custom data types (more info below)
 
// OR use preset classes:
 
	msa::Interpolator1D				myInterpolator1D;		// create spline of floats (1D)
	msa::Interpolator2D				myInterpolator2D;		// create spline of Vec2f (2D)
	msa::Interpolator3D				myInterpolator3D;		// create spline of Vec3f (3D)
 

// splines wrap basic functionality of stl::vector:
	myInterpolator.size();				// return number of data elements
	myInterpolator.reserve(int count);	// if you know how many elements up front it will improved performance when adding (you can still add more than this number of elements)
	myInterpolator.at(int i);				// return data at i'th index
	myInterpolator.clear();				// remove all elements
	myInterpolator.push_back(data1);		// add some data to the spline
	myInterpolator.push_back(data2);

	myInterpolator.sampleAt(float t);		// (e.g. t:0.34 =>) samples along 34% of the whole spline using the current interpolation method and options

	setInterpolation(i);			// set interpolation type, see MSAInterpolationTypes.h (currently cubic catmull rom and linear)
	int getInterpolation();			// get interpolation type
 
	setUseLength(bool b);			// whether to use Length or not. using Length is slightly slower than not using (depending on number of data points)
	bool getUseLength();			// if useLength is true, sampleAt(0.57) means sample at 57% along the physical length of the spline (using the interpolated spline for Length calculation)
									// if useLength is false, the %t refers to % along the data points. If data points are evenly spaced its no problem, but if they are randomly spaced, the interpolation will not be uniform
 
 
	myInterpolator.drawRaw(int dotSize, int lineWidth);					// draws raw data with dotSize and lineWidth (make either zero to not draw dots or lines)
	myInterpolator.drawSmooth(int numSteps, int dotSize, int lineWidth);	// draws smoothed data in  (make either zero to not draw dots or lines)
 
Using custom data type:
	msa::InterpolatorT<myDataType>	myInterpolator2;		// create spline of custom data types (more info below)
	myDataType has to be a scalar or class with the overloaded operators:
		+  (myDataType&)
		-  (myDataType&)
		== (myDataType&)
		=  (myDataType&)
		*  (float)
	
	and also define the function lengthOf(myDataType&) to return a scalar float value depicting the 'magnitude' of the data type (used in calculating Length)

 
*************************************************************************/

/***************
 DEPENDENCIES:
 - MSACore
 ***************/ 

#pragma once

#include "MSAInterpolationTypes.h"
#include "MSAInterpolatorT.h"
#include "MSAInterpolator1D.h"
#include "MSAInterpolator2D.h"
#include "MSAInterpolator3D.h"


