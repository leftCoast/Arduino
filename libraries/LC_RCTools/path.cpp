#include "path.h"
#include "mapper.h"
/*
mapper percentToAngle(0,100,0,90);

basePath::basePath() {
enum moveType { noMove, maxAccel, trapMove, triMove };
}


basePath::~path(void) {  }


void	basePath::setPath(float inX1,float inX2, float inYMax, float accelPercent) {

	float	x1;
	float	x2;
	float yMax;
	float	rampAngle;
	float halfDist;
	float	rampDist;
	
	rampAngle	= percentToAngle.Map(accelPercent);	// Map the acceleration percentage value to angle.
	halfDist		= x1 + ((x2-x1)/2.0);					// Calculate the midway point.
	rampDist		= sin(rampAngle)*yMax;					// rampAngle of 0 will crash it.
	if (rampAngle = 0) {										// If we get a zero acceleration..
		mMoveType = noMove;									// We won't move at all.
		return;													// We're done here, let's go.
	} else if (rampAngle = 90) {							// If we get maximum acceleration..
		mMoveType = maxAccel;								// Set the type to maxAccel.
		return;													// And again, we are done here.
	} else if (rampDist < halfDist) {					// If we get to max speed..
		mMoveType = trapMove;								// Set the type to trapezoid move.
		return;													// Done, move along.	
	} else {														// Else, we never make it to max speed..
		mMoveType = triMove;									// This is going to be a triangular move.
		yMax = cos(rampAngle) * halfDist;				// Recalculate the maxSpeed to what we can attain.
	}
}


void	basePath::setStartTime(void)  { mStartTime = micros(); }


float	basePath::mapPos(float x) {

	float fraction;
	
	switch (mMoveType) {
		case noMove : return x1;
		case maxAccel : return x2;
		case trapMove :
			if (x>=x2) { return 2 * rampTotal + cruiseTotal; }
			else if (x>x2-rampDist) {
				fraction = rampTotal-((x2-x0 * cos(rampAngle)/2);
				return rampTotal + runTotal + fraction;
			} else if (x>
			 
			
			
			
			if (x<rampDist) { return x * cos(rampAngle) / 2.0; }
			else if (x==rampDist) { return (rampTotal); }
			
		break;
		case triMove :
			sdkfsdkfjh
		break;
	}
}
	
	
*/