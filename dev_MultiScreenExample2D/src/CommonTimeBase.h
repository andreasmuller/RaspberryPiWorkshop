//
//  CommonTimeBase.h
//  MultiScreenCommonTiming
//
//  Created by Andreas Müller on 24/12/2012.
//
//

#pragma once

#include "ofMain.h"
#include <set>

// We don't consider values that took longer than this to reach us
#define MAX_CONSIDERED_PING_TIME_MILLIS			8000

#define PING_AMOUNT_TO_KEEP						32

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
class CommonTimeBase
{
	public:
	
		CommonTimeBase();
		~CommonTimeBase();

		void	baseInit();
	
		float	getTimeSecs();
		int		getTimeMillis();
	
		void	newReading( int _serverTimeMillis, int _pingMillis );
	
		void	_update(ofEventArgs &e);
	
		// When doing animation with the timing value, it's nicer to smothly "rewind" or "fast forward" the offset rather than set it straight away
		inline bool	setEaseOffset( bool _ease )
		{
			easeOffset = _ease;
		}
	
		int		getInternalTimeMillis();	// get the local computer time, it's a function (as opposed to just using say ofGetElapsedTimeMillis() ) so that we can easily replace the source
		
		// make these private once we finish debugging
		int		offsetMillis;
		int		offsetMillisTarget;
	
	private:
	
		virtual void sendPing() {}	// override with something using the protocol of your choice
	
		void	calculateOffset( int _serverTimeMillis );
			
		multiset<int> pingMillis; // the std::multiset will automatically sort the entries for us, small to large.
	
		bool	easeOffset;
	
		int		lastPingSentTimeMillis;
		int		millisBetweenPings;
	
		int		maxDiffAdjustmentThreshold;	// don't adjust the time unless it's off by thi much, otherwise we will be adjusting quite often and animation will be jittery
	
		int		INITIALISATION_PING_DELAY_MILLIS;
		int		POST_INITIALISATION_PING_DELAY_MILLIS;
	
};


/*


From: http://www.codewhore.com/howto1.html
 
 1.	The first host records an initial timestamp from his clock into an unsigned 32-bit value. 
 All others defer to his authority for the session-wide clock (which is only as accurate as his hardware and network allow).
 
 2.	Other hosts periodically poll their clock (say every 3 or 5 seconds) and send the value in a "ping packet" to the server. 
 This ping can even be piggybacked on a normal data packet.
 
 3.	The server polls his clock, appends the result, and sends back a "pong packet" in response -- as quickly as possible. 
 Dropped or extremely late packets are not resent, because they would be inaccurate and useless.
 
 4.	On receipt, the client timestamps the response once more. He now has 3 numbers: the original timestamp, the server's timestamp, and the final timestamp. 
 Once again, very slow responses (over 10 seconds) are discarded immediately.
 
 5.	From these values, calculation of the round trip time and average latency is easy. 
 By adding the average latency to the server's timestamp, the client knows what its clock should read.
 
 6.	Record the round trip time into an array of the last 32 or 64 pings.
 
 7.	Create a temporary array and fill it with the entries in increasing order.
 
 8.	Extract the values at the 20th, 50th, and 80th percentile, or perform a similar statistical operation (eg, square of differences).
 
 9.	If the round trip time for the latest ping falls within the 20th %ile and the clock appears to be over 3 milliseconds off, then the client synchronizes his clock to the "correct" time.
 
 The percentiles above are slightly arbitrary, but they are good indicators of network performance in several ways:
 •	20th %ile: These are the absolute best ping times for the link recently, so the application shouldn't expect much better. 
	Use this value to select only the most accurate ping packets for synchronizing the clock.
 
 •	50th %ile: The median value describes the latency of exactly half of the pings. 
 Use this (or a true average) to predict how long a given packet should spend on the wire.
 
 •	80th %ile: Most packets arrive within this time, so this determines when a message should be considered "late". 
 A reliable packet that hasn't been acknowledged by twice this value (round trip time) should be considered lost or late -- and resent.
 
 You may decide that sending 32-bit timestamps is a waste of bandwidth -- and you'd be right. 
 A 32-bit millisecond timer has a range of 50 days, but sending just a 16-bit timestamp delta has a range of +/- 32 seconds. 
 This means that you can reduce your packet size by stripping off the most signficant bits of the timestamp, and then extrapolating them on the remote side. 
 This must be done immediately, though, because a slow packet may actually spend 30 seconds on the wire or in a queue waiting for acknowledgement.
 
 Once the clock on every machine is accurate within about 6 milliseconds of each other, it becomes easy to compare timestamps and perform detection or prediction in real time. 
 To measure the elapsed time between two events A and B, simply subtract the timestamps (A - B) and examine the signed result:
 
 Positive: Event A occurred (A - B) milliseconds after event B.
 Negative: Event A occurred (B - A) milliseconds before event B.
 Zero: Event A occurred at exactly the same time as event B.
 
 There are problems, of course. Accuracy is still limited by the network latency and frequent errors can cause the clock to "jitter". 
 One fix is to widen the tolerance from 3 to 7 milliseconds over time, so that clocks are well synchronized at start and touched later only if they drift heavily.
 Assymmetric links are another problem. When the latency from host X to host Y is significantly greater than the latency from host Y to host X 
 (eg, some cable modems use Ethernet for downstream data, but an analog modem for upstream), it is practically impossible to measure using the round-trip time 
 of ping packets. This and other network time issues are still under active research.




*/