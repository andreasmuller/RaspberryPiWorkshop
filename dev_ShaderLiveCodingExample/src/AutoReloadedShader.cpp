
#include "AutoReloadedShader.h"


// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
bool AutoReloadedShader::load(string shaderName )
{
	load( shaderName + ".vert", shaderName+ ".frag", "" );
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
bool AutoReloadedShader::load(string vertName, string fragName, string geomName)
{
	lastTimeCheckMillis = 0;
	setMillisBetweenFileCheck( 2 * 1000 );
	enableWatchFiles();
	
	vertexShaderFilename = vertName;
	fragmentShaderFilename = fragName;
	geometryShaderFilename = geomName;

	vertexShaderFile   = ofFile( ofToDataPath( vertexShaderFilename ) );
	fragmentShaderFile = ofFile( ofToDataPath( fragmentShaderFilename ) );
	geometryShaderFile = ofFile( ofToDataPath( geometryShaderFilename ) );
	
	//cout << vertexShaderFile.exists() << endl;
	
	fileChangedTimes.push_back( getLastModified( vertexShaderFile ) );
	fileChangedTimes.push_back( getLastModified( fragmentShaderFile ) );
	fileChangedTimes.push_back( getLastModified( geometryShaderFile ) );
	
	ofShader::load( vertexShaderFilename, fragmentShaderFilename, geometryShaderFilename );
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void AutoReloadedShader::_update(ofEventArgs &e)
{
	int currTime = ofGetElapsedTimeMillis();
	
	if( (currTime - lastTimeCheckMillis) > millisBetweenFileCheck )
	{
		if( filesChanged() )
		{
			ofShader::load( vertexShaderFilename, fragmentShaderFilename, geometryShaderFilename );
		}
		
		lastTimeCheckMillis = currTime;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void AutoReloadedShader::enableWatchFiles()
{
	ofAddListener(ofEvents().update, this, &AutoReloadedShader::_update );
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void AutoReloadedShader::disableWatchFiles()
{
	ofRemoveListener(ofEvents().update, this, &AutoReloadedShader::_update );
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
bool AutoReloadedShader::filesChanged()
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
std::time_t AutoReloadedShader::getLastModified( ofFile& _file )
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
bool AutoReloadedShader::reloadShaders()
{
	cout << "Reload Shader" << endl;
	return load( vertexShaderFilename,  fragmentShaderFilename, geometryShaderFilename );
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void AutoReloadedShader::setMillisBetweenFileCheck( int _millis )
{
	millisBetweenFileCheck = _millis;
}
