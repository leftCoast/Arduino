#ifndef viewMgr_h
#define viewMgr_h

#include <lists.h>
#include <idlers.h>
#include <timeObj.h>
#include <baseGraphics.h>


// ***************************************************************
// Here we have the management of screen actions. Get the touches
// and lifts then interpret them into touch, click, drag etc.
// events.
// ***************************************************************


enum eventType { nullEvent, touchEvent, liftEvent, dragBegin, dragOn, clickEvent };



// ***************************************************
// event, this is all the information we can gather about
// what the user is doing with the screen. A touch, a lift,
// where it happend, how fast is his finger moving? How
// far has it moved since the last event?
// ***************************************************


struct event {
	eventType		mType;
	point				mLastPos;
	point				mWhere;
	unsigned long	mMs;
	unsigned long	mLastMs;
	unsigned long	mNumMs;
	int				mXDist;
	float				mXPixlePerSec;
	int				mYDist;
	float				mYPixlePerSec;
	float				mDist;
	float				mPixalPerSec;
	float				mAngle;
};



// ***************************************************
// eventObj, this is the object we pack the event into
// so it will be able go into the event queue. Once the
// event has been read, the manager recycles the object
// and the object recycles the event.
// ***************************************************


class eventObj	:	public linkListObj {

	public:
            eventObj(event* inEvent);
	virtual	~eventObj(void);
	
				event*	mEvent;				
};
		


// ***************************************************
// eventMgr: This is the object that creates the events.
// Once created, the events are queued up for the viewMgr
// to dispatch them to its drawObj(s). After an even has
// been dispatched, the eventMgr recycles the event's
// memory.
// ***************************************************


class eventMgr :	public queue,
						public idler,
						public timeObj {
	public:
            eventMgr(void);
	virtual	~eventMgr(void);
    
				
    			void	flushEvents(void);
				event	getEvent(void);
				void	addEvent(event* newEvent);
				void	createEvent(eventType inType);
	virtual	void	idle(void);
	
				bool				mTouched;
				bool				mDragging;
				point				mLastPos;
				unsigned long	mLastMs;
				event				mNullEvent;           	
};

extern	eventMgr 	ourEvntMgr;								// Our global event manager.


#endif