
#include "ofxAutoReloadedShader.h"


// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
bool ofxAutoReloadedShader::load(string shaderName )
{
	return load( shaderName + ".vert", shaderName + ".frag", shaderName + ".geom" );
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
bool ofxAutoReloadedShader::load(string vertName, string fragName, string geomName)
{
	unload();
	
	// hackety hack, clear errors or shader will fail to compile
	GLuint err = glGetError();
	
	lastTimeCheckMillis = ofGetElapsedTimeMillis();
	setMillisBetweenFileCheck( 2 * 1000 );
	enableWatchFiles();
	
	loadShaderNextFrame = false;
	
	vertexShaderFilename = vertName;
	fragmentShaderFilename = fragName;
	geometryShaderFilename = geomName;
	
	vertexShaderFile.clear();
	fragmentShaderFile.clear();
	geometryShaderFile.clear();
	
	vertexShaderFile   = ofFile( ofToDataPath( vertexShaderFilename ) );
	fragmentShaderFile = ofFile( ofToDataPath( fragmentShaderFilename ) );
	geometryShaderFile = ofFile( ofToDataPath( geometryShaderFilename ) );
	
	ofBuffer vertexShaderBuffer = ofBufferFromFile( ofToDataPath( vertexShaderFilename ) );
	ofBuffer fragmentShaderBuffer = ofBufferFromFile( ofToDataPath( fragmentShaderFilename ) );
	ofBuffer geometryShaderBuffer = ofBufferFromFile( ofToDataPath( geometryShaderFilename ) );
	
	fileChangedTimes.clear();
	fileChangedTimes.push_back( getLastModified( vertexShaderFile ) );
	fileChangedTimes.push_back( getLastModified( fragmentShaderFile ) );
	fileChangedTimes.push_back( getLastModified( geometryShaderFile ) );
	
	if( vertexShaderBuffer.size() > 0 )
	{
		setupShaderFromSource(GL_VERTEX_SHADER, vertexShaderBuffer.getText() );
	}

	if( fragmentShaderBuffer.size() > 0 )
	{
		setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderBuffer.getText());
	}

	#ifndef TARGET_OPENGLES
	if( geometryShaderBuffer.size() > 0 )
	{
		setupShaderFromSource(GL_GEOMETRY_SHADER_EXT, geometryShaderBuffer.getText());
	}
	#endif
	
	return linkProgram();
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofxAutoReloadedShader::_update(ofEventArgs &e)
{
	if( loadShaderNextFrame )
	{
		reloadShaders();
		loadShaderNextFrame = false;
	}
	
	int currTime = ofGetElapsedTimeMillis();
	
	if( ((currTime - lastTimeCheckMillis) > millisBetweenFileCheck) &&
	   !loadShaderNextFrame )
	{
		if( filesChanged() )
		{
			loadShaderNextFrame = true;
		}
		
		lastTimeCheckMillis = currTime;
	}
}


// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
bool ofxAutoReloadedShader::reloadShaders()
{
	return load( vertexShaderFilename,  fragmentShaderFilename, geometryShaderFilename );
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofxAutoReloadedShader::enableWatchFiles()
{
	ofAddListener(ofEvents().update, this, &ofxAutoReloadedShader::_update );
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofxAutoReloadedShader::disableWatchFiles()
{
	ofRemoveListener(ofEvents().update, this, &ofxAutoReloadedShader::_update );
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
bool ofxAutoReloadedShader::filesChanged()
{
	bool fileChanged = false;
	
	if( vertexShaderFile.exists() )
	{
		std::time_t vertexShaderFileLastChangeTime = getLastModified( vertexShaderFile );
		if( vertexShaderFileLastChangeTime != fileChangedTimes.at(0) )
		{
			fileChangedTimes.at(0) = vertexShaderFileLastChangeTime;
			fileChanged = true;
		}
	}
	
	if( fragmentShaderFile.exists() )
	{
		std::time_t fragmentShaderFileLastChangeTime = getLastModified( fragmentShaderFile );
		if( fragmentShaderFileLastChangeTime != fileChangedTimes.at(1) )
		{
			fileChangedTimes.at(1) = fragmentShaderFileLastChangeTime;
			fileChanged = true;
		}
	}
	
	
	if( geometryShaderFile.exists() )
	{
		std::time_t geometryShaderFileLastChangeTime = getLastModified( geometryShaderFile );
		if( geometryShaderFileLastChangeTime != fileChangedTimes.at(2) )
		{
			fileChangedTimes.at(2) = geometryShaderFileLastChangeTime;
			fileChanged = true;
		}
	}
	
	return fileChanged;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
std::time_t ofxAutoReloadedShader::getLastModified( ofFile& _file )
{
	if( _file.exists() )
	{
		Poco::File& pocoFile		= _file.getPocoFile();
		Poco::Timestamp timestamp	= pocoFile.getLastModified();
		std::time_t fileChangedTime = timestamp.epochTime();
		
		return fileChangedTime;
	}
	else
	{
		return 0;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofxAutoReloadedShader::setMillisBetweenFileCheck( int _millis )
{
	millisBetweenFileCheck = _millis;
}
