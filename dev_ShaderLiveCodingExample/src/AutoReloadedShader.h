#pragma once

#include "ofMain.h"

class AutoReloadedShader : public ofShader
{

	public:

		bool load(string shaderName );
		bool load(string vertName, string fragName, string geomName); // override the initialisation function
	
		bool reloadShaders();
	
		void enableWatchFiles();
		void disableWatchFiles();
	
		void _update(ofEventArgs &e);
	
		void setMillisBetweenFileCheck( int _millis );
	
	private:

		bool filesChanged();

		std::time_t getLastModified( ofFile& _file );
	
		int lastTimeCheckMillis;
		int millisBetweenFileCheck;
	
		string vertexShaderFilename;
		string fragmentShaderFilename;
		string geometryShaderFilename;
	
		ofFile vertexShaderFile;
		ofFile fragmentShaderFile;
		ofFile geometryShaderFile;
	
		vector< std::time_t > fileChangedTimes;
};

