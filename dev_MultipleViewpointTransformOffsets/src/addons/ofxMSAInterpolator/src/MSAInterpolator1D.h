
/**************************** 1D InterpolatorT (of floats) ****************************/

#pragma once

#include "MSAInterpolatorT.h"

namespace msa {
	
    //--------------------------------------------------------------
	inline float lengthOf(float f) {
		return f;
	}
	
	
    //--------------------------------------------------------------
	typedef InterpolatorT<float> Interpolator1D;
}
