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
	
	INITIALISATION_PING_DELAY_MILLIS		= 60;
	POST_INITIALISATION_PING_DELAY_MILLIS	= 5 * 1000;
	
	offsetMillis = 0;
	lastPingSentTimeMillis = 0;
	millisBetweenPings = INITIALISATION_PING_DELAY_MILLIS;

	maxDiffAdjustmentThreshold = 10;		
	
	easeOffset = false;
	offsetMillisTarget = 0;
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
	return getInternalTimeMillis() + offsetMillis;
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
	int currentLocalMillis = getInternalTimeMillis();
	
	if( (currentLocalMillis-lastPingSentTimeMillis) > millisBetweenPings )
	{
		sendPing();
		lastPingSentTimeMillis = currentLocalMillis;
	}
	
	// Ease the timing offset if this is activated
	if( easeOffset )
	{
		if( offsetMillis != offsetMillisTarget )
		{
			float tmpDiff = (offsetMillisTarget-offsetMillis)/10.0;
			if( fabsf(tmpDiff) < 1.0f )
			{
				offsetMillis = offsetMillisTarget;
			}
			else
			{
				offsetMillis += tmpDiff;
			}
		}
	}
	else
	{
		offsetMillis = offsetMillisTarget;
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
		pingMillis.erase( --pingMillis.end() );
	}
	
	/*
	// debug print the multiset
	cout << "--------------------------------------------------------" << endl;
	multiset<int>::iterator tmpIt;
	for ( tmpIt = pingMillis.begin(); tmpIt != pingMillis.end(); tmpIt++)
	{
		cout << *tmpIt << endl;
	}
	cout << "--------------------------------------------------------" << endl;
	*/
	 
	calculateOffset( _serverTimeMillis );
	
	// If we are still in the initialisation phase, be more aggressive in how often we send out pings
	if( pingMillis.size() < PING_AMOUNT_TO_KEEP )
	{
		millisBetweenPings = INITIALISATION_PING_DELAY_MILLIS;
	}
	else
	{
		millisBetweenPings = POST_INITIALISATION_PING_DELAY_MILLIS;
	}
	
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void CommonTimeBase::calculateOffset( int _serverTimeMillis )
{
	int tmpPingTime = 0;
	
	// Don't do anything statistical until we have enough readings to do so,
	// currently waiting until we have half of the amount of values we want
	if( pingMillis.size() > (PING_AMOUNT_TO_KEEP/2) )
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
		
		//cout << "We are assuming an average ping time of: " << tmpPingTime << endl;
	}
	else
	{
		// we just grab the best ping time
		multiset<int>::iterator tmpIt = pingMillis.begin();
		tmpPingTime = *tmpIt;
	}
	
	// Todo: Seems more logical here to check against the lowest percentile,
	// if the ping is below, we can use it to adjust the clock.
	
	int currServerTime = _serverTimeMillis + tmpPingTime;
	
	// if the clock seems to be off by a certain threshold
	int tmpDiffPrevServerTime = currServerTime - getTimeMillis();
	if( abs(tmpDiffPrevServerTime) > maxDiffAdjustmentThreshold )
	{
		offsetMillisTarget = currServerTime - getInternalTimeMillis();
	}
	else
	{
		cout << "We weren't off by that much, so no need to adjust, " << tmpDiffPrevServerTime << endl;
	}
	
	// if we only have one entry in the pingMillis multiset, we just set the offset straight away, easing or no easing
	if( pingMillis.size() <= 1) offsetMillis = offsetMillisTarget;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
int CommonTimeBase::getInternalTimeMillis()
{
	return ofGetElapsedTimeMillis();
}
