#ifndef eventMgr_h
#define eventMgr_h

#include <lists.h>
#include <idlers.h>
#include <timeObj.h>
#include <baseGraphics.h>

#define	DRAG_TIME	500	// How many milliseconds before it goes from a click to a drag?
#define	DRAG_DIST	10		// If the finger moves further than this, we'll call it a drag.
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
};

//void printEvent(event* anEvent);


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
    
				void			begin(void);
    			void			flushEvents(void);
    			bool			haveEvent(void);
				event			getEvent(void);
				
				void			addEvent(eventType inType);
	virtual	void			push(eventObj* newEventObj);
	virtual	eventObj*	pop(void);
	virtual	void			idle(void);
	
				bool				mTouched;
				bool				mDragging;
				unsigned long	mTouchMs;
				unsigned long	mLastMs;
				point				mTouchPos;
				point				mLastPos;
				event				mNullEvent;           	
};

extern	eventMgr 	ourEventMgr;								// Our global event manager.


#endif