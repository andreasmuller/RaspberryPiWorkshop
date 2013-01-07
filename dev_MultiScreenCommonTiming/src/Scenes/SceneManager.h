//
//  SceneManager.h
//  MultiScreenCommonTiming
//
//  Created by Andreas Müller on 25/12/2012.
//
//

#pragma once

#include "ofMain.h"


class SceneManager
{
	public:
		
		SceneManager();
		~SceneManager();
	
		void update( float _time );
		void draw();
	
		void changeScene( int _scene );
		int  getSceneIndex();
	
	private:
	
		int	sceneIndex;		// which scene are we displaying?
		float currAnimationTimeSecs;

};
