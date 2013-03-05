//
//  SyncedAnimationObject.h
//  MultiScreenExample3D
//
//  Created by Andreas Muller on 02/02/2013.
//
//

#pragma once

#include "ofMain.h"

class SyncedAnimationObject
{
	public:
	
		virtual void update( float _time ) {}
		virtual void draw() { }
	
		bool	deleteMe;
};