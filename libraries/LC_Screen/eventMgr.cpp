#include <eventMgr.h>
#include <resizeBuff.h>
#include <screen.h>


// Our global event manager.
eventMgr 	ourEventMgr;

eventType		mType;
	
	unsigned long	mTouchMs;
	unsigned long	mLastMs;
	unsigned long	mNumMs;
	
	point				mTouchPos;
	point				mLastPos;
	
	int				mXDist;
	int				mYDist;
	float				mDist;
	
	float				mXPixlePerSec;
	float				mYPixlePerSec;
	float				mPixalPerSec;
	
	float				mAngle;

/*	
void printEvent(event* anEvent) {

	
	Serial.print(F("mType         : "));
	switch (anEvent->mType) {
		case nullEvent		: Serial.println(F("nullEvent"));	break;
		case touchEvent	: Serial.println(F("touchEvent"));	break;
		case liftEvent		: Serial.println(F("liftEvent"));	break;
		case dragBegin		: Serial.println(F("dragBegin"));	break;
		case dragOn			: Serial.println(F("dragOn")); 		break;
		case clickEvent	: Serial.println(F("clickEvent"));	break;
	}
	Serial.println();
	
	Serial.print(F("Size in bytes : "));Serial.println(sizeof(event));
	Serial.println();
	
	Serial.print(F("mTouchMs      : "));Serial.println(anEvent->mTouchMs);
	Serial.print(F("mLastMs       : "));Serial.println(anEvent->mLastMs);
	Serial.print(F("mNumMs        : "));Serial.println(anEvent->mNumMs);
	Serial.println();
	
	printPoint(&anEvent->mTouchPos,"mTouchPos     : ");Serial.println();
	printPoint(&anEvent->mLastPos,"mLastPos     : ");Serial.println();
	Serial.println();
	
	Serial.print(F("mXDist        : "));Serial.println(anEvent->mXDist);
	Serial.print(F("mYDist        : "));Serial.println(anEvent->mYDist);
	Serial.print(F("mDist         : "));Serial.println(anEvent->mDist);
	Serial.println();
	
	Serial.print(F("mXPixlePerSec : "));Serial.println(anEvent->mXPixlePerSec);
	Serial.print(F("mYPixlePerSec : "));Serial.println(anEvent->mYPixlePerSec);
	Serial.print(F("mPixalPerSec  : "));Serial.println(anEvent->mPixalPerSec);
	Serial.println();
	
	Serial.print(F("mAngle        : "));Serial.println(anEvent->mAngle);
};
*/


eventObj::eventObj(event* inEvent) { mEvent = inEvent; }


eventObj::~eventObj(void) { if (mEvent) free(mEvent); };
				


// ***************************************************
// eventMgr, This is the guy that figures out what kind
// of screen things are going on. touch, lift, dragBegin,
// dragOn, click or nothing.
// ***************************************************
   
				
eventMgr::eventMgr(void) {
	
	setTime(DRAG_TIME);		// Set up our timer.
	
	mTouched		= false;		// Initialize the past.
	mDragging	= false;		// This is a lot like rewriting history.
	
	mTouchMs		= 0;
	mLastMs		= 0;
	
	mTouchPos.x	= 0;
	mTouchPos.y	= 0;
	mLastPos.x	= 0;
	mLastPos.y	= 0;
				
	mNullEvent.mType				= nullEvent;	// Setup a global, ready to go null event.						
	mNullEvent.mTouchMs			= 0;				// This way if nothing's going on. We won't	
	mNullEvent.mLastMs			= 0;
	mNullEvent.mNumMs				= 0;
	
	mNullEvent.mTouchPos			= mLastPos;		// waste time creating a bunch of these.						
	mNullEvent.mLastPos			= mLastPos;		// Using mLastPos for all these 'cause its currently 0,0.
								
	mNullEvent.mXDist				= 0;				
	mNullEvent.mYDist				= 0;
	mNullEvent.mDist				= 0;
	
	mNullEvent.mXPixlePerSec	= 0;	
	mNullEvent.mYPixlePerSec	= 0;				
	mNullEvent.mPixalPerSec		= 0;
	
	mNullEvent.mAngle				= 0;
}


eventMgr::~eventMgr(void) { }


void eventMgr::begin(void) { hookup(); }


/*
// New events go in here.
void eventMgr::addEvent(event* newEvent) { push((linkListObj*)newEvent); }
*/

// Sometimes you want a fresh start.
void eventMgr::flushEvents(void) { dumpList(); }


// We have an event for somebody?
bool eventMgr::haveEvent(void) { return !isEmpty(); }


// Someone wants the next event. That's our job!
event eventMgr::getEvent(void) {

	eventObj*	nextEvent;
	event			anEvent;
	
	nextEvent = (eventObj*)pop();			// Pop the queue for a pointer to the next event.
	if (nextEvent) {							// If there was an event to pop off the queue..
		anEvent = *(nextEvent->mEvent);	// Save off the data to stack memory.
		delete nextEvent;						// Delete the copy we pulled off the queue.
		return anEvent;						// Return our local copy.
	}
	return mNullEvent;						// No event waiting? Send them a nullEvent.
}


// We figure out every possible bit of information we
// can about an event.
void eventMgr::addEvent(eventType inType) {

	event*		newEvent;
	eventObj*	newEventObj;
	
	newEvent = NULL;															// The pointer needs to start as NULL.
	if (resizeBuff(sizeof(event),(uint8_t**)&newEvent)) {			// Becuse we allocate with resizeBuff();
		switch(inType) {
			case nullEvent		: free(newEvent); return;				// We don't make nullEvents, we
			case touchEvent	:
				newEvent->mType			= touchEvent;					
				newEvent->mTouchMs		= mTouchMs;						// Fist thing, grab the time.					
				newEvent->mLastMs			= mTouchMs;						// Same time, we just touched.
				newEvent->mNumMs			= 0;								// Calculate the number of milliseconds since last time.

				newEvent->mTouchPos		= mTouchPos;					// Grab the location of the event.					
				newEvent->mLastPos		= mTouchPos;
								
				newEvent->mXDist			= 0;				
				newEvent->mYDist			= 0;
				newEvent->mDist			= 0;
	
				newEvent->mXPixlePerSec	= 0;	
				newEvent->mYPixlePerSec	= 0;				
				newEvent->mPixalPerSec	= 0;
	
				newEvent->mAngle			= 0;
			break;
			case liftEvent		:
				newEvent->mType			= liftEvent;					
				newEvent->mTouchMs		= mTouchMs;					
				newEvent->mLastMs			= millis();
				newEvent->mNumMs			= abs(newEvent->mLastMs-mTouchMs);					// Calculate the number of milliseconds since touch.
	
				newEvent->mTouchPos		= mTouchPos;												// Saved when we got the touch.					
				newEvent->mLastPos		= mLastPos;													// Updated during idle time while touched.
								
				newEvent->mXDist			= xDistance(mTouchPos,mLastPos);						// Calculate the x distance.				
				newEvent->mYDist			= yDistance(mTouchPos,mLastPos);						// Calculate the y distance.	
				newEvent->mDist			= distance(mTouchPos,mLastPos);						// Calculate the total distance.	
	
				newEvent->mXPixlePerSec	= (newEvent->mXDist/newEvent->mNumMs)*1000;		// Calculate the x speed. Pixels/sec.
				newEvent->mYPixlePerSec	= (newEvent->mYDist/newEvent->mNumMs)*1000;		// Calculate the y speed. Pixels/sec.				
				newEvent->mPixalPerSec	= (newEvent->mDist/newEvent->mNumMs)*1000;		// Calculate the total speed. Pixels/sec.
	
				newEvent->mAngle			= angle(mTouchPos,mLastPos);							// Calculate the actual angle, in radians.
			break;
			case dragBegin		:
				newEvent->mType			= dragBegin;					
				newEvent->mTouchMs		= mTouchMs;					
				newEvent->mLastMs			= millis();
				newEvent->mNumMs			= abs(newEvent->mLastMs-mTouchMs);					// Calculate the number of milliseconds since touch.
	
				newEvent->mTouchPos		= mTouchPos;												// Saved when we got the touch.					
				newEvent->mLastPos		= mLastPos;													// Updated during idle time while touched.
								
				newEvent->mXDist			= xDistance(mTouchPos,mLastPos);						// Calculate the x distance.				
				newEvent->mYDist			= yDistance(mTouchPos,mLastPos);						// Calculate the y distance.	
				newEvent->mDist			= distance(mTouchPos,mLastPos);						// Calculate the total distance.	
	
				newEvent->mXPixlePerSec	= (newEvent->mXDist/newEvent->mNumMs)*1000;		// Calculate the x speed. Pixels/sec.
				newEvent->mYPixlePerSec	= (newEvent->mYDist/newEvent->mNumMs)*1000;		// Calculate the y speed. Pixels/sec.				
				newEvent->mPixalPerSec	= (newEvent->mDist/newEvent->mNumMs)*1000;		// Calculate the total speed. Pixels/sec.
	
				newEvent->mAngle			= angle(mTouchPos,mLastPos);							// Calculate the actual angle, in radians.
			break;
			case dragOn			:
				newEvent->mType			= dragOn;					
				newEvent->mTouchMs		= mTouchMs;					
				newEvent->mLastMs			= millis();
				newEvent->mNumMs			= abs(newEvent->mLastMs-mTouchMs);					// Calculate the number of milliseconds since touch.
	
				newEvent->mTouchPos		= mTouchPos;												// Saved when we got the touch.					
				newEvent->mLastPos		= mLastPos;													// Updated during idle time while touched.
								
				newEvent->mXDist			= xDistance(mTouchPos,mLastPos);						// Calculate the x distance.				
				newEvent->mYDist			= yDistance(mTouchPos,mLastPos);						// Calculate the y distance.	
				newEvent->mDist			= distance(mTouchPos,mLastPos);						// Calculate the total distance.	
	
				newEvent->mXPixlePerSec	= (newEvent->mXDist/newEvent->mNumMs)*1000;		// Calculate the x speed. Pixels/sec.
				newEvent->mYPixlePerSec	= (newEvent->mYDist/newEvent->mNumMs)*1000;		// Calculate the y speed. Pixels/sec.				
				newEvent->mPixalPerSec	= (newEvent->mDist/newEvent->mNumMs)*1000;		// Calculate the total speed. Pixels/sec.
	
				newEvent->mAngle			= angle(mTouchPos,mLastPos);							// Calculate the actual angle, in radians.
			break;
			case clickEvent	:
				newEvent->mType			= clickEvent;					
				newEvent->mTouchMs		= mTouchMs;					
				newEvent->mLastMs			= millis();
				newEvent->mNumMs			= abs(newEvent->mLastMs-mTouchMs);					// Calculate the number of milliseconds since touch.
	
				newEvent->mTouchPos		= mTouchPos;												// Saved when we got the touch.					
				newEvent->mLastPos		= mLastPos;													// Updated during idle time while touched.
								
				newEvent->mXDist			= xDistance(mTouchPos,mLastPos);						// Calculate the x distance.				
				newEvent->mYDist			= yDistance(mTouchPos,mLastPos);						// Calculate the y distance.	
				newEvent->mDist			= distance(mTouchPos,mLastPos);						// Calculate the total distance.	
	
				newEvent->mXPixlePerSec	= (newEvent->mXDist/newEvent->mNumMs)*1000;		// Calculate the x speed. Pixels/sec.
				newEvent->mYPixlePerSec	= (newEvent->mYDist/newEvent->mNumMs)*1000;		// Calculate the y speed. Pixels/sec.				
				newEvent->mPixalPerSec	= (newEvent->mDist/newEvent->mNumMs)*1000;		// Calculate the total speed. Pixels/sec.
	
				newEvent->mAngle			= angle(mTouchPos,mLastPos);							// Calculate the actual angle, in radians.
			break;
		}
		newEventObj = new eventObj(newEvent);														// Create the node this will live in.
		if (newEventObj) { push(newEventObj); }													// We are a queue. Push the new guy in.
	}
}


// Use this so we can have some type checking.
void eventMgr::push(eventObj* newEventObj) { queue::push((linkListObj*)newEventObj); }


// And this. I was bit by doing it fast and loose earlier.
eventObj* eventMgr::pop(void) { return (eventObj*)queue::pop();	}


void eventMgr::idle(void) {
	
	float	moveDist;
	
	if (mTouched) {											// Last time we checked we were mTouched.
		if (!screen->touched()) {							// This time we are NOT mTouched.
			addEvent(liftEvent);								// This means we got a lift.
			mDragging = false;								// No matter, we're not mDragging.
			mTouched = false;									// Save off that we're no longer mTouched.
			if (!ding()) {										// If it was a "short" touch.
				addEvent(clickEvent);						// That'll pass for a "click".
			}
		} else {													// Else, still mTouched.
			mLastPos = screen->getPoint();				//	Update the last point we saw.
			moveDist = distance(mTouchPos,mLastPos);	// Calculate the total distance.	
			if (ding()||moveDist>DRAG_DIST) {			// If our timer expired or were moving, it's a drag.
				if (!mDragging) {								// If we we weren't already dagging..
					addEvent(dragBegin);						// Create a drag begin event.
					mDragging = true;							// Note that we are mDragging.
				} else if (isEmpty()) {						// Else if already mDragging AND we have an empty queue.
					addEvent(dragOn);							// We pop in a dragOn event. Don't want to swamp the queue.
				}
			}
		}
	} else {											// Else we were not mTouched last time we checked..
		if (screen->touched()) {				// And we ARE mTouched now..
			mTouchMs = millis();					// Save time..
			mTouchPos = screen->getPoint();	// And place of touch!
			addEvent(touchEvent);			// Create a touch event.
			mLastPos = screen->getPoint();	// Init. the last position seen. Not used for touchEvent.
			mTouched = true;						// Note that we've been mTouched.
			start();									// Start up the drag timer.
		}
	}
}


