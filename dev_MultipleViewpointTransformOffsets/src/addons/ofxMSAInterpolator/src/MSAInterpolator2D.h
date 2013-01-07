
/**************************** 2D InterpolatorT (of Vec2) ****************************/

#pragma once

#include "MSAInterpolatorT.h"

namespace msa {
	
	typedef InterpolatorT<Vec2f> Interpolator2D;

	
    //--------------------------------------------------------------
	inline float lengthOf(const Vec2f& v) {
		return v.length();
	}

	
    //--------------------------------------------------------------
	// OpenGL ES compatibility added by Rob Seward
	// http://www.openframeworks.cc/forum/viewtopic.php?f=25&t=3767&p=19865
	inline void drawInterpolatorRaw(Interpolator2D &spline, int dotSize = 20, int lineWidth = 4){
		int numItems = spline.size();
		
		if(lineWidth) {
			glLineWidth(lineWidth);
			GLfloat vertex[numItems * 2];
			for(int i=0; i<numItems; i++) {
				vertex[i*2]		= spline.at(i).x;
				vertex[(i*2)+1] = spline.at(i).y;
			}
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, 0, vertex);
			glDrawArrays(GL_LINE_STRIP, 0, numItems);
		}
		
		if(dotSize) {
			glPointSize(dotSize);
			GLfloat vertex[numItems * 2];
			for(int i=0; i<numItems; i++) {
				vertex[i*2]		= spline.at(i).x;
				vertex[(i*2)+1] = spline.at(i).y;
			}
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, 0, vertex);
			glDrawArrays(GL_POINTS, 0, numItems);
		}		
	}
	
	
    //--------------------------------------------------------------
	inline void drawInterpolatorSmooth(Interpolator2D &spline, int numSteps, int dotSize = 8, int lineWidth = 2) {
		float spacing = 1.0/numSteps;
		if(lineWidth) {
			glLineWidth(lineWidth);
			
			GLfloat vertex[numSteps * 2];
			int i=0;
			for(float f=0; f<1; f+= spacing) {
				Vec2f v			= spline.sampleAt(f);
				vertex[i*2]		= v.x;
				vertex[(i*2)+1] = v.y;
				i++;
			}
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, 0, vertex);
			glDrawArrays(GL_LINE_STRIP, 0, numSteps);
		}
		
		if(dotSize) {
			glPointSize(dotSize);
			GLfloat vertex[numSteps * 2];
			int i=0;
			for(float f=0; f<1; f+= spacing) {
				Vec2f v			= spline.sampleAt(f);
				vertex[i*2]		= v.x;
				vertex[(i*2)+1] = v.y;
				i++;
			}
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, 0, vertex);
			glDrawArrays(GL_POINTS, 0, numSteps);
		}
	}
}
