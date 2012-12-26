//
//  CommonTime.cpp
//  MultiScreenCommonTiming
//
//  Created by Andreas Müller on 24/12/2012.
//
//

#include "CommonTimeBase.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
CommonTimeBase::CommonTimeBase()
{
	// during this time, we are more aggressive about sending out pings
	TIMING_INITIALISATION_DURATION_MILLIS	= 3 * 1000;
	
	INITIALISATION_PING_DELAY_MILLIS		= 60;
	POST_INITIALISATION_PING_DELAY_MILLIS	= 5 * 1000;
	
	offsetMillis = 0;
	lastPingSentTimeMillis = 0;
	millisBetweenPings = INITIALISATION_PING_DELAY_MILLIS;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
CommonTimeBase::~CommonTimeBase()
{
	ofRemoveListener(ofEvents().update, this, &CommonTimeBase::_update);
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void CommonTimeBase::baseInit()
{
	ofAddListener(ofEvents().update, this, &CommonTimeBase::_update );
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
int	CommonTimeBase::getTimeMillis()
{
	return ofGetElapsedTimeMillis() + offsetMillis;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
float CommonTimeBase::getTimeSecs()
{
	return getTimeMillis() / 1000.0f;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void CommonTimeBase::_update(ofEventArgs &e)
{
	int currentLocalMillis = ofGetElapsedTimeMillis();
	if( currentLocalMillis < TIMING_INITIALISATION_DURATION_MILLIS )
	{
		millisBetweenPings = INITIALISATION_PING_DELAY_MILLIS;
	}
	else
	{
		millisBetweenPings = POST_INITIALISATION_PING_DELAY_MILLIS;
	}
	
	if( (currentLocalMillis-lastPingSentTimeMillis) > millisBetweenPings )
	{
		sendPing();
		lastPingSentTimeMillis = currentLocalMillis;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void CommonTimeBase::newReading( int _serverTimeMillis, int _pingMillis )
{
	// bail if the ping round trip is just too long to be reliable
	if( _pingMillis > MAX_CONSIDERED_PING_TIME_MILLIS )
	{
		return;
	}
	
	pingMillis.insert( _pingMillis );
	
	if( pingMillis.size() > PING_AMOUNT_TO_KEEP )
	{
		pingMillis.erase( pingMillis.end() );
	}
	
	calculateOffset( _serverTimeMillis );
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void CommonTimeBase::calculateOffset( int _serverTimeMillis )
{
	int tmpPingTime = 0;
	
	// Don't do anything statistical until we have enough readings to do so,
	// currently waiting until we have half of the amount of values we want
	if( pingMillis.size() < (PING_AMOUNT_TO_KEEP/2) )
	{
		// grad the best X percent of values, get the average and use that
		float percentile = 0.2f;
		int percentileIndex = pingMillis.size() * percentile;
		int tmpIndex = 0;
		int tmpPingAccumulator = 0;
		
		// this is clunky and needs to be re-written
		multiset<int>::iterator tmpIt;
		for ( tmpIt = pingMillis.begin(); tmpIt != pingMillis.end(); tmpIt++)
		{
			if( tmpIndex < percentileIndex )
			{
				tmpPingAccumulator += *tmpIt;
			}
			tmpIndex++;
		}
		
		tmpPingTime = tmpPingAccumulator / percentileIndex;
	}
	else
	{
		// we just grab the best ping time
		multiset<int>::iterator tmpIt = pingMillis.begin();
		tmpPingTime = *tmpIt;
	}
	
	int currServerTime = _serverTimeMillis + tmpPingTime;
	offsetMillis = ofGetElapsedTimeMillis() - currServerTime;
}