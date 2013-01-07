/**************************** InterpolatorT Template Class ****************************/
#pragma once


#include "MSACore.h"
#include "MSAInterpolationTypes.h"

namespace msa {
	
	template <typename T>
	class InterpolatorT {
	public:
		
		bool verbose;
		
		InterpolatorT();
		
		// interpolate and re-sample at t position along the spline
		// where t: 0....1 based on length of spline
		T sampleAt(float t);		
		
		void setInterpolation(InterpolationType i = kInterpolationCubic);
		int getInterpolation();
		
		void setUseLength(bool b);
		bool getUseLength();
		
		// return length upto data point i
		// leave blank (-1) to return length of entire data set
		// only valid if setUseLength is true
		// uses current interpolation settings for lenth calculation
		// returns cached value, no calculations done in this function
		const float getLength(int i=-1);
		
		
		
		// set number of subdivisions used to calculation length of segment
		void setLengthSubdivisions(int i = 100);
		int getLengthSubdivisions();

		
		/******************* stl::container wrapper functions *******************/
		void push_back(const T& newData);		
		int size();
		void reserve(int i);
		void clear();
		const T& at(int i);
		vector<T> getData();
		
	protected:
		InterpolationType _interpolationMethod;
		bool _useLength;
		int	_lengthSubdivisions;		// number of subdivisions used for length calculation
		vector<T> _data;				// vector of all data
		vector<float> _dist;			// vector of cumulative Lengths from i'th data point to beginning of spline
		
		
		// calculates length of segment prior to (leading up to) i'th point
		float calcSegmentLength(int i);		
		
		// update all Lengths in _dist array
		void updateAllLengths();
		
		// given t(0...1) find the node index directly to the left of the point
		void findPosition(float t, int &leftIndex, float &mu);		
		
		T linearInterpolate(const T& y1, const T& y2, float mu);
		
		// this function is from Paul Bourke's site
		// http://local.wasp.uwa.edu.au/~pbourke/miscellaneous/interpolation/
		T cubicInterpolate(const T& y0, const T& y1, const T& y2, const T& y3, float mu);
		
	};
	
	
	//----------------------------------------------------------------------------
    //--------------------------------------------------------------
	
    //--------------------------------------------------------------
	template <typename T>
	InterpolatorT<T>::InterpolatorT() {
		setInterpolation();
		setUseLength(false);
		setLengthSubdivisions();
		verbose = false;
	}
	
	
    //--------------------------------------------------------------
	// use catmull rom interpolation to re-sample At normT position along the spline
	// where normT: 0....1 based on length of spline
	template <typename T>
	T InterpolatorT<T>::sampleAt(float t) {
		int numItems = size();
		if(numItems == 0) {
			//				if(verbose) printf("InterpolatorT: not enough samples", t);
			return T();
		}
		
		if(t>1) t = 1;
		else if(t<0) t=0;
		int i0, i1, i2, i3;
		float mu;
		
		findPosition(t, i1, mu);
		
		// if less than 4 data points, force linear interpolation
		InterpolationType it = _interpolationMethod;
		if(numItems<4) it = kInterpolationLinear;
		
		switch(it) {
			case kInterpolationCubic:
				i0 = i1 - 1;
				i2 = i1 + 1;
				i3 = i2 + 1;
				
				if(i0 < 0) i0 = 0;
				if(i3 >= numItems) i3 = numItems-1;
				
				return cubicInterpolate(at(i0), at(i1), at(i2), at(i3), mu);
				break;
				
			case kInterpolationLinear:
				i2 = i1 + 1;
				if(i2 >= numItems) i2 = numItems-1;
				return linearInterpolate(at(i1), at(i2), mu);
				break;
		}
	}
	
    //--------------------------------------------------------------
	template <typename T>
	void InterpolatorT<T>::setInterpolation(InterpolationType i) {
		_interpolationMethod = i;
		updateAllLengths();
	}
	
    //--------------------------------------------------------------
	template <typename T>
	int InterpolatorT<T>::getInterpolation() {
		return _interpolationMethod;
	}
	
    //--------------------------------------------------------------
	template <typename T>
	void InterpolatorT<T>::setUseLength(bool b) {
		_useLength = b;
		if(_useLength) updateAllLengths();
		else _dist.clear();
	}
	
    //--------------------------------------------------------------
	template <typename T>
	bool InterpolatorT<T>::getUseLength() {
		return _useLength;
	}
	
    //--------------------------------------------------------------
	template <typename T>
	const float InterpolatorT<T>::getLength(int i) {
		if(_useLength) {
			return i < 0 ? _dist[_dist.size()-1] : _dist.at(i);
		} else {
			return 0;
		}
	}

	
    //--------------------------------------------------------------
	template <typename T>
	void InterpolatorT<T>::setLengthSubdivisions(int i) {
		_lengthSubdivisions = i;
	}

    //--------------------------------------------------------------
	template <typename T>
	int InterpolatorT<T>::getLengthSubdivisions() {
		return _lengthSubdivisions;
	}

	
    //--------------------------------------------------------------
	template <typename T>
	void InterpolatorT<T>::push_back(const T& newData) {
		_data.push_back(newData);						// add data
		
		if(getUseLength()) {
			float segmentLength;
			float totalLength;
			
			if(size() > 1) {
				//				T distT		= newData - _data.at(prevIndex);	// get offset to previous node
				//				float dist		= lengthOf(distT);					// actual Length to node
				
				segmentLength	= calcSegmentLength(size()-1);
				totalLength		= segmentLength + _dist.at(size()-2);
			} else {
				segmentLength	= 0;
				totalLength		= 0;
			}
			
			_dist.push_back(totalLength);
			
			//				if(verbose) printf("segment length = %f | total length = %f\n", segmentLength, totalLength);
		}
	}
	
    //--------------------------------------------------------------
	template <typename T>
	int InterpolatorT<T>::size() {
		return _data.size();
	}
	
    //--------------------------------------------------------------
	template <typename T>
	void InterpolatorT<T>::reserve(int i) {
		_data.reserve(i);
		_dist.reserve(i);
	}
	
    //--------------------------------------------------------------
	template <typename T>
	void InterpolatorT<T>::clear() {
		_data.clear();
		_dist.clear();		
	}
	
    //--------------------------------------------------------------
	template <typename T>
	const T& InterpolatorT<T>::at(int i) {
		return _data.at(clamp(i, 0, size()-1));
	}
	
    //--------------------------------------------------------------
	template <typename T>
	vector<T> InterpolatorT<T>::getData() {
		return _data;
	}
	
	
    //--------------------------------------------------------------
	template <typename T>
	float InterpolatorT<T>::calcSegmentLength(int i) {
        ofLogVerbose("msa::InterpolatorT<T>::calcSegmentLength(int i) isn't working anymore");
		int numItems		= size();
		
		if(numItems < 2 || i < 1 || i >= numItems) return 0;
		
		bool saveUseLength	= _useLength;
		_useLength = false;
		
		float startPerc = (i-1) * 1.0f/(numItems-1);
		float endPerc	= (i) * 1.0f/(numItems-1);
		float incPerc	= (endPerc - startPerc)/_lengthSubdivisions;
		
		T prev = sampleAt(startPerc);
		T cur;
		
		float segmentLength = 0;
		for(float f = startPerc; f <= endPerc; f+= incPerc) {
			cur = sampleAt(f);
//			segmentLength += lengthOf(cur - prev);  // TODO: this isn't compiling anymore!
			prev = cur;
		}
		
		_useLength = saveUseLength;
		
		if(verbose) printf("segment length for %i is %f\n", i, segmentLength);
		
		
		return segmentLength;
	}
	
    //--------------------------------------------------------------
	template <typename T>
	void InterpolatorT<T>::updateAllLengths() {
		_dist.clear();
		
		float curTotal = 0;
		
		for(int i=0; i<size(); i++) {
			curTotal += calcSegmentLength(i);
			_dist.push_back(curTotal);
		}
		
	}
	
	
	//--------------------------------------------------------------
    template <typename T>
	void InterpolatorT<T>::findPosition(float t, int &leftIndex, float &mu) {
		int numItems = size();
		
		switch(numItems) {
			case 0: 
				leftIndex = 0;
				mu = 0;
				break;
				
			case 1:
				leftIndex = 0;
				mu = 0;
				break;
				
			case 2:
				leftIndex = 0;
				mu = t;
				break;
				
			default:
				if(_useLength) {									// need to use 
					float totalLengthOfInterpolator = _dist.at(numItems-1);
					float tDist = totalLengthOfInterpolator * t;			// the Length we want to be from the start
					int startIndex = floor(t * (numItems - 1));			// start approximation here
					int i1 = startIndex;
					int limitLeft = 0;
					int limitRight = numItems-1;
					
					float distAt1, distAt2;
					//						do {
					for(int iterations = 0; iterations < 100; iterations ++) {	// limit iterations
						distAt1 = _dist.at(i1);
						if(distAt1 <= tDist) {							// if Length at i1 is less than desired Length (this is good)
							distAt2 = _dist.at(clamp(i1+1, 0, (int)_dist.size()-1));
							if(distAt2 > tDist) {
								leftIndex = i1;
								mu = (tDist - distAt1) / (distAt2-distAt1);
								return;
							} else {
								limitLeft = i1;
							}
						} else {
							limitRight = i1;
						}
						i1 = (limitLeft + limitRight)>>1;
					}
					//						} while(true);
					
				} else {
					float actT = t * (numItems - 1);
					leftIndex = floor(actT);
					mu = actT - leftIndex;
				}
		}
	}
	
	
    //--------------------------------------------------------------
	template <typename T>
	T InterpolatorT<T>::linearInterpolate(const T& y1, const T& y2, float mu) {
		return (y2-y1) * mu + y1;
	}
	
	
    //--------------------------------------------------------------
	// this function is from Paul Bourke's site
	// http://local.wasp.uwa.edu.au/~pbourke/miscellaneous/interpolation/
	template <typename T>
	T InterpolatorT<T>::cubicInterpolate(const T& y0, const T& y1, const T& y2, const T& y3, float mu) {
		float mu2 = mu * mu;
		T a0 = y3 - y2 - y0 + y1;
		T a1 = y0 - y1 - a0;
		T a2 = y2 - y0;
		T a3 = y1;
		
		return(a0 * mu * mu2 + a1 * mu2 + a2 * mu + a3);
	}
	
}