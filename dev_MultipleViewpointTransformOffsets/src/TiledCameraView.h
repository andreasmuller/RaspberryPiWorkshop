
#pragma once

#include "ofMain.h"

/*
 	Todo:
 		- confirm it works properly for tiles in the Y direction as wel
 		- add an option to add a gap in the tiles, to account for screen bezels
 
 */

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
class TiledCameraView
{
	public:
	
		// --------------------------------------------------------------------
		TiledCameraView()
		{
			numTilesX = 0;
			numTilesY = 0;
			setFov( 60.0f );
			setNearAndFarZ( 0.1f, 1024.0f );
		}
	
		// --------------------------------------------------------------------
		void init( int _tileWidth, int _tileHeight, int _screenAmountX, int _screenAmountY = 1 )
		{
			numTilesX = _screenAmountX;
			numTilesY = _screenAmountY;
			
			tileWidth 	= _tileWidth;
			tileHeight 	= _tileHeight;
			
			fullWidth	= tileWidth  * _screenAmountX;
			fullHeight	= tileHeight * _screenAmountY;
		}
	
		// --------------------------------------------------------------------
		// Thanks ofxTileSaver (by Matthias Dörfelt & Jeremy Rotsztain) for this snippet
		//
		void setPerspectiveTransformForTile( int _tileIndexX, int _tileIndexY = 0 )
		{
			float aspectRatio = (float)fullWidth / fullHeight;
			
			float fullTop		= nearZ * tan(fov * PI / 360.0);
			float fullBottom	= -fullTop;
			
			float fullLeft		= fullBottom * aspectRatio;
			float fullRight		= fullTop * aspectRatio;
			
			
			float tileLeft		= fullLeft + (((fullRight - fullLeft) * (_tileIndexX * tileWidth)) / (float)fullWidth);
			float tileRight		= tileLeft + (((fullRight - fullLeft) * tileWidth) / (float)fullWidth);
			
			float tileBottom	= fullBottom + (fullTop - fullBottom) * (_tileIndexY* tileHeight) / (float)fullHeight;
			float tileTop		= tileBottom + (((fullTop - fullBottom) * tileHeight) / (float)fullHeight);
			
			//if( _tileIndexX == 0 ) cout << endl << "fullTop: " << fullTop << " fullBottom: " << fullBottom << " fullLeft: " << fullLeft << " fullRight: " << fullRight << endl;
			//cout << "tileTop: " << tileTop << " tileBottom: " << tileBottom << " tileLeft: " << tileLeft << " tileRight: " << tileRight << endl;
			
			//glViewport(0, 0, tileWidth, tileHeight);
			ofViewport( ofRectangle(0, 0, tileWidth, tileHeight) );
			
			ofMatrix4x4 frustumMatrix;
			frustumMatrix.makeFrustumMatrix( tileLeft, tileRight, tileBottom, tileTop, nearZ, farZ );
			
			glMatrixMode(GL_PROJECTION);
			ofLoadMatrix(frustumMatrix);
			//glFrustum( tileLeft, tileRight, tileBottom, tileTop, nearZ, farZ);
			
			glMatrixMode(GL_MODELVIEW);
		}

		// --------------------------------------------------------------------
		void setFov( float _fov )
		{
			fov = _fov;
		}
	
		// --------------------------------------------------------------------
		void setNearAndFarZ( float _nearZ, float _farZ )
		{
			nearZ = _nearZ;
			farZ = _farZ;
		}
	
		// --------------------------------------------------------------------
		int getNumTilesX()
		{
			return numTilesX;
		}

		// --------------------------------------------------------------------
		int getNumTilesY()
		{
			return numTilesY;
		}
	
	private:
	
		int numTilesX;
		int numTilesY;
	
		int tileWidth;
		int tileHeight;
	
		int fullWidth;
		int fullHeight;
		float aspectRatio;
		
		float fov;
		float nearZ;
		float farZ;
};