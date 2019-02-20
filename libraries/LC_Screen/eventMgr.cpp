#include <eventMgr.h>
#include <resizeBuff.h>
#include <screen.h>


// Our global event manager.
eventMgr 	ourEventMgr;

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
	Serial.print(F("Size in bytes : "));Serial.println(sizeof(event));
	printPoint(&(anEvent->mLastPos),"mLastPos      : ");
	printPoint(&(anEvent->mWhere),"mWhere        : ");
	Serial.print(F("mMs           : "));Serial.println(anEvent->mMs);
	Serial.print(F("mLastMs       : "));Serial.println(anEvent->mLastMs);
	Serial.print(F("mNumMs        : "));Serial.println(anEvent->mNumMs);
	Serial.print(F("mXDist        : "));Serial.println(anEvent->mXDist);
	Serial.print(F("mXPixlePerSec : "));Serial.println(anEvent->mXPixlePerSec);
	Serial.print(F("mYDist        : "));Serial.println(anEvent->mYDist);
	Serial.print(F("mYPixlePerSec : "));Serial.println(anEvent->mYPixlePerSec);
	Serial.print(F("mDist         : "));Serial.println(anEvent->mDist);
	Serial.print(F("mPixalPerSec  : "));Serial.println(anEvent->mPixalPerSec);
	Serial.print(F("mAngle        : "));Serial.println(anEvent->mAngle);
};



eventObj::eventObj(event* inEvent) { mEvent = inEvent; }


eventObj::~eventObj(void) {

	if (mEvent) {
		resizeBuff(0,(uint8_t**)&mEvent);
		//free(mEvent);
		}
	};
				


// ***************************************************
// eventMgr, This is the guy that figures out what kind
// of screen things are going on. touch, lift, dragBegin,
// dragOn, click or nothing.
// ***************************************************


eventMgr::eventMgr(void) {
	
	mTouched		= false;		// Initialize the past.
	mDragging	= false;		// This is a lot like rewriting history.
	mLastPos.x	= 0;
	mLastPos.y	= 0;
	mLastMs		= 0;

	mNullEvent.mMs					= 0;				// Setup a global, ready to go null event.							
	mNullEvent.mType				= nullEvent;	// This way if nothing's going on. We won't								
	mNullEvent.mLastPos			= mLastPos;		// waste time creating a bunch of these.						
	mNullEvent.mWhere				= mLastPos;		// Using mLastPos for all these 'cause its currently 0,0.					
	mNullEvent.mLastMs			= 0;							
	mNullEvent.mNumMs				= 0;					
	mNullEvent.mXDist				= 0;		
	mNullEvent.mXPixlePerSec	= 0;				
	mNullEvent.mYDist				= 0;	
	mNullEvent.mYPixlePerSec	= 0;				
	mNullEvent.mDist				= 0;		
	mNullEvent.mPixalPerSec		= 0;
	mNullEvent.mAngle				= 0;
}


eventMgr::~eventMgr(void) { }


void eventMgr::begin(void) { hookup(); }


// New events go in here.
void eventMgr::addEvent(event* newEvent) { push((linkListObj*)newEvent); }


// Sometimes you want a fresh start.
void eventMgr::flushEvents(void) { dumpList(); }


// We have an event for somebody?
bool eventMgr::haveEvent(void) { return !isEmpty(); }


// Someone wants the next event. That's our job!
event eventMgr::getEvent(void) {

	eventObj*	nextEvent;
	event			anEvent;
	
	Serial.println("In getEvent()");Serial.flush();
	nextEvent = (eventObj*)pop();			// Pop the queue for a pointer to the next event.
	//printEvent(nextEvent->mEvent);
	if (nextEvent) {							// If there was an event to pop off the queue..
		anEvent = *(nextEvent->mEvent);	// Save off the data to stack memory.
		Serial.println("Grabbing event");Serial.flush();
		printEvent(&anEvent);
		Serial.println("Deleting eventObj");Serial.flush();
		delete nextEvent;						// Delete the copy we pulled off the queue.
		Serial.println("Returning event");Serial.flush();
		return anEvent;						// Return our local copy.
	}
	return mNullEvent;						// No event waiting? Send them a nullEvent.
}


// We figure out every possible bit of information we
// can about an event.
void eventMgr::createEvent(eventType inType) {

	event* newEvent;
	eventObj* newEventObj;
	newEvent = NULL;																				// The pointer needs to start as NULL.
	if (resizeBuff(sizeof(event),(uint8_t**)&newEvent)) {								// Becuse we allocate with resizeBuff();
		newEvent->mMs				= millis();													// Fist thing, grab the time.
		newEvent->mType			= inType;													// They give us the type.
		newEvent->mLastPos		= mLastPos;													// Save off the lat position we saw.
		newEvent->mWhere			= screen->getPoint();									// Grab the location of the event.
		mLastPos						= newEvent->mWhere;										// Update the global last position.
		newEvent->mLastMs			= mLastMs;													// Save off the last time.
		mLastMs						= newEvent->mMs;											// Update the global last time.
		newEvent->mNumMs			= abs(newEvent->mMs-mLastMs);							// Calculate the number of milliseconds since last time.
		newEvent->mXDist			= xDistance(mLastPos,newEvent->mWhere);			// Calculate the x distance, for a drag.
		newEvent->mXPixlePerSec	= (newEvent->mXDist/newEvent->mNumMs)*1000;		// Calculate the x speed, for a drag.
		newEvent->mYDist			= yDistance(mLastPos,newEvent->mWhere);			// Calculate the y distance, for a drag.
		newEvent->mYPixlePerSec	= (newEvent->mYDist/newEvent->mNumMs)*1000;		// Calculate the y sped, for a drag.
		newEvent->mDist			= distance(mLastPos,newEvent->mWhere);				// Calculate the actual distance, for a drag.
		newEvent->mPixalPerSec	= (newEvent->mDist/newEvent->mNumMs)*1000;		// Calculate the actual speed, for a drag.
		newEvent->mAngle			= angle(newEvent->mLastPos,newEvent->mWhere);	// Calculate the actual angle, for a drag. In radians.
		newEventObj = new eventObj(newEvent);												// Create the node this will live in.
		printEvent(newEvent);
		if (newEventObj) { push((linkListObj*)newEvent); }								// We are a queue. Push the new guy in.
	}
}
	

void eventMgr::idle(void) {
	
	if (mTouched) {								// Last time we checked we were mTouched.
		if (!screen->touched()) {				// This time we are NOT mTouched.
			createEvent(liftEvent);				// This means we got a lift.
			mDragging = false;					// No matter, we're not mDragging.
			mTouched = false;						// Save off that we're no longer mTouched.
			if (!ding()) {							// If it was a "short" touch.
				createEvent(clickEvent);		// That'll pass for a "click".
			}
		} else {										// Else, still mTouched..
			if (ding()) {							// If our timer expired, its a drag.
				if (!mDragging) {					// If we we weren't already dagging..
					createEvent(dragBegin);		// Create a drag begin event.
					mDragging = true;				// Note that we are mDragging.
				} else if (isEmpty()) {			// Else if already mDragging AND we have an empty queue.
					createEvent(dragOn);			// We pop in a dragOn event. Don't want to swamp the queue.
				}
			}
		}
	} else {											// Else we were not mTouched last time we checked..
		if (screen->touched()) {					// And we ARE mTouched now..
			createEvent(touchEvent);			// Create a touch event.
			mTouched = true;						// Note that we've been mTouched.
			start();									// Start up the drag timer.
		}
	}
}


