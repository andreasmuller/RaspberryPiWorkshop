#pragma once

#include "ofMain.h"

class ofxAutoReloadedShader : public ofShader
{
	
public:
	
	// override the initialisation functions
	bool load(string shaderName );
	bool load(string vertName, string fragName, string geomName);
	
	bool reloadShaders();
	
	void enableWatchFiles();
	void disableWatchFiles();
		
	void setMillisBetweenFileCheck( int _millis );
	
	void _update(ofEventArgs &e);	
	
private:
	
	bool filesChanged();
	
	bool loadShaderNextFrame;
	
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


