#include "drawObj.h"

// ***********************************

drawObj::drawObj() {

  needRefresh	= true;	// Well Duh! We never been drawn yet!
  focus			= false;			// But we're hopefull.
  clicked		= false;
  mEventSet		= noEvents;	// 'Cause this is actually the default.
  callback		= NULL;		// And, no.. We have none of this either.
  lastX			= 0;				// Yes, these are where we were before we moved.
  lastY			= 0;
}


drawObj::drawObj(rect* inRect,eventSet inEventSet)
	: rect(inRect) {
	
	needRefresh	= true;		// Well Duh! We never been drawn yet!
	focus			= false;		// But we're hopefull.
	clicked		= false;
	mEventSet	= inEventSet;	
	callback		= NULL;		// And, no.. We have none of this either.
	lastX			= 0;			// Yes, these are where we were before we moved.
	lastY			= 0;
}


drawObj::drawObj(int inLocX, int inLocY, int inWidth,int inHeight,eventSet inEventSet)
	: rect(inLocX,inLocY,inWidth,inHeight) {
	
	needRefresh	= true;		// Well Duh! We never been drawn yet!
	focus			= false;		// But we're hopefull.
	clicked		= false;
	mEventSet	= inEventSet;
	callback		= NULL;		// And, no.. We have none of this either.
	lastX			= 0;			// Yes, these are where we were before we moved.
	lastY			= 0;
}


// Wake up! Time to die.
drawObj::~drawObj() {

		if (currentFocus == this) {	// We were great..
			currentFocus = NULL;			// And how the great have fallen. (But do it quiet like. We don't want a setFocus call right now!)
		}
		if (theTouched == this) {		// We were the movers and shakers..
			theTouched = NULL;			// But, its all over now.
		}
	}


// When the manager asks if we want a refresh..
bool drawObj::wantRefresh(void) { return needRefresh; }


// Sometimes others know better than us that we need a refresh.
// This allows them to make that happen. Sometimes they want to tell
// us that they'll take care of the refresh stuff themselves.
void drawObj::setNeedRefresh(bool refresh) { needRefresh = refresh; }


void drawObj::setLocation(int inX,int inY) {

	lastX = x;					// Just in the off chance we're doing screen animation.
	lastY = y;					// We'll save where we were.
	rect::setLocation(inX,inY);
	needRefresh = true;
}
	
	
// Call this one to draw..
void  drawObj::draw(void) {

	eraseSelf();			// Like I said, just in case you need it..
	drawSelf();
	needRefresh = false;
}


// Mostly you can ignore this one. Typically used for animation.
// "I've moved, erase where I was before redrawing".
void  drawObj::eraseSelf(void) {  }


// override this one and draw yourself.
void  drawObj::drawSelf(void) {
  
  screen->fillRect(x, y, width, height, &black); // Default draw.
  screen->drawRect(x, y, width, height, &white);
}


// We are either getting or loosing focus.
void  drawObj::setThisFocus(bool setLoose) {

	focus = setLoose;
	needRefresh = true;
}
	
// Well, do we have focus or not?
bool  drawObj::haveFocus(void) { return focus; }

				
// The ability to control this may be handy..
void drawObj::setEventSet(eventSet inEventSet) {

	if (theTouched!=this) {		// I think we better not change this while in the middle of a event set.
		mEventSet = inEventSet;
	}
}


// There may be cases when something needs to see what our even set is.
eventSet drawObj::getEventSet(void) { return mEventSet; }


//nullEvent, touchEvent, liftEvent, dragBegin, dragOn, clickEvent
bool drawObj::acceptEvent(event* inEvent,point* locaPt) {

	switch (mEventSet) {
		case noEvents		: return false;			// noEvents, pass on..
		case touchLift		: 								// Classic button events, clicked lets you draw clicked.
			if (inEvent->mType==touchEvent) {		// If its a touch..
				if (inRect(locaPt)) {					// - and if its on us..
					clicked		= true;					// Might want to show we're clicked on.
					doAction(inEvent,locaPt);			// Do those things we do.
					theTouched	= this;					// Tell the world WE are accepting this event set.
					needRefresh = true;					// touchLift doesn't get a lift event. So it needs the setRefresh here.
					return true;							// Tell the world the event has been accepted.
				}
			} else if (theTouched==this 
				&& inEvent->mType==liftEvent) {		// We only want lifts if we'd accepted the touch.
				clicked		= false;						// And we're no longer clicked.
				needRefresh = true;						// And here.. (see above)
				return true;								// Again, tell the world the event has been accepted.
			}
			break;
		case fullClick 	:								// Things like edit fields. A click changes their state.
			if (inEvent->mType==clickEvent) {		// If its a click event, that matches.
				if (inRect(locaPt)) {					// and if its on us..
					clicked		= false;					// No longer clicked by the time you see this.
					doAction(inEvent,locaPt);			// Do the action.
					return true;							// We don't set touched because this is a one shot event.
				}
			}
			break;
		case dragEvents	:								// Things that move by touch.
			if (inEvent->mType==dragBegin) {			// If, the dragging finger has started..
				if (inRect(locaPt)) {					// and if its on us..
					doAction(inEvent,locaPt);			// Do our stuff.
					theTouched	= this;					// Tell the world WE are accepting this event set.
					return true;
				}
			} else if (inEvent->mType==dragOn) {		// still moving,
				doAction(inEvent,locaPt);					// Stil dragging? Keep drawing.
				return true;									// Event has been accepted.
			} else if (inEvent->mType==liftEvent) {	// Done dragging.
				doAction(inEvent,locaPt);					// Do our stuff.
				return true;									// Again, tell the world the event has been accepted.
			}
			break;
		}
		return false;
	}

// 										****** doAction() ******
//
// Choose one or the other. If you just want something like a button the simple one is best.
// If you need the event, or local point etc. Use the one with the event & local point
// passed in.
//
//											************************

// *** BASIC ACTION, SIMPLE SIMPLE ***
// Override this one to do an action.
void drawObj::doAction(void) {

	if (callback) {	// If you figure out how to setup a callback, it'll work. I can't 
		  callback();	// remember how to do this. (I did it once.) Typically I just
	}						// override this and make it do what I want. Or.. (See below)
}


// *** MORE INCVOLVED ACTION ***
// Override this for more involved action. Like dragging. (The event has global point.
// We'll pass in local point. You may need it.) 
void drawObj::doAction(event* inEvent,point* localPt) { doAction(); }


// Define a void function, for example : void myCallback(void){...} 
// Then, just call this method with your callback's name.
// myDrawObj.setCallback(myCallback);   
// Simple as that.
void drawObj::setCallback(void (*funct)(void)) { callback = funct; }



// ***************************************************
// viewMgr, This is they guy that runs the screenshow.
// Handles redrawing, clicks, etc.
//
//
// Two globals that are somewhat related.
//
// currentFocus : This is a pointer to show who has focus.
// If (this==currentFocus) you know to draw yourslelf in a
// way to show the user you have focus. This can be set by
// anyone by calling setFocusPtr(); Pass in a NULL for no
// focus.
//
// This focus stuff is purely optional. By default it does
// nothing. But, if you have a layout that has a choice of
// things the user can interact with? You can write code
// that uses it to keep track of what the user is
// interacting with. The important part is that that only
// one thing at a time can have focus. So when object A) gets
// a click and grabs focus, object B) is told that its
// loosing focus. Kinda' handy.
//
// theTouched : This is a pointer to who just accepted the
// last screen touch. This way its touchOver() method can
// be located and called. Basically that's all it's for.
// ***************************************************


viewMgr 	viewList;
drawObj*	currentFocus = NULL;	// Totally global because? There's only ONE user.
drawObj*	theTouched = NULL;	// Who's accepted a finger touch on the screen?

void	setFocusPtr(drawObj* newFocus) {

		if (newFocus!=currentFocus) {					// People are lazy. Now they don't need to check.
			if (currentFocus) {							// Check for NULL..
				currentFocus->setThisFocus(false);	// Warn them that their star is falling.
			}
			currentFocus = newFocus;					// The up and coming..
			if (currentFocus) {							// If we're actually passed in something.
				currentFocus->setThisFocus(true);	// Tell 'em their star is rising!!
			}
		}
	}
	
	
viewMgr::viewMgr(void) { }


viewMgr::~viewMgr(void) { listHeader.dumpList(); }



// New objects go on top.
void viewMgr::addObj(drawObj* newObj) {
    
    newObj->linkAfter(&listHeader);	// Put the new guy at the top of the list.
    hookup();								// hookup on the first addition. Ignored otherwise.
}

// Now anyone can tell us to do that.
void viewMgr::dumpDrawObjList(void) { listHeader.dumpList(); }


// Checking events. If no one is set as "theTouched" we begin a
// new set of events. If a drawObj accepts an event it is fed
// all subsequent events through a lift event. Then the process
// starts over.
bool viewMgr::checkEvents(event* theEvent) {
    
	drawObj*	trace;
	point		lPoint;

	if (screen->hasTouchScreen()) {								// Sanity! Does it even have the hardware?
		lPoint = screen->lP(theEvent->mTouchPos);				// Possibly we're sublist, localize.
		if (theTouched==NULL) {										// No one has accepted a touch yet.					
			trace = theList();										// Make sure we're at the top.
			while(trace) {												// While we have something to work with.
				if (trace->acceptEvent(theEvent,&lPoint)) {	// If someone accepts this event..
					return true;										// We are done. Return success.
				}
				trace = (drawObj*)trace->dllNext;				// Hop to the next.
			}
		} else {															// Someone is accepting events.
			theTouched->acceptEvent(theEvent,&lPoint);		// Let them have this one.
			if (theEvent->mType==liftEvent) {					// If it was a lift?
				theTouched = NULL;                        	// Clear out our pointer, event set is complete.
				return true;                           		// We changed something. Lets go.
			}
		}
	}
	return false;														// No changes to report.
}



// Checking for redraws.
void viewMgr::checkRefresh(void) {
    
	drawObj*	trace;
	
	trace = (drawObj*)listHeader.getLast();		// make sure we're at the bottom.
	while(trace && trace!=(&listHeader)) {			// While not NULL or pointing at our header.
		if (trace->wantRefresh()) {					// Does this guy want refresh?
      	trace->draw();								// Call his Draw method.
		}
      trace = (drawObj*)trace->dllPrev;			// Bump up the list.
	}
}
 
 
// Counts & returns the number of objects in the list.           
int viewMgr::numObjects(void) { return (int)listHeader.countTail(); }


// Finds the "nth" item on the list, index starting at 0. NULL if not found.
drawObj* viewMgr::getObj(int index) { return (drawObj*)listHeader.getTailObj((int)index); }


drawObj* viewMgr::theList(void) { return (drawObj*)listHeader.getTailObj(0); }


// We have time to do stuff, NOT A LOT!
void viewMgr::idle(void) {
   
   event	theEvent;
   
   if (ourEventMgr.haveEvent()) {			// Have an event for us?
		theEvent = ourEventMgr.getEvent();	// Grab the event.
		checkEvents(&theEvent);					// Pass it in and see if its handled.
	}
	checkRefresh();								// Check if we need to do some drawing?
}



// ***************************************************
// drawGroup, This is they guy that runs a bunch of sub
// objects. He is both a drawObj and a viewMgr.
// Before taking any action with a sub object the drawGroup
// pushes its location onto the screen. This allows all sub
// objects to work on local coordinates.
// ***************************************************


drawGroup::drawGroup(rect* inRect,eventSet inEventSet) 
	: drawObj(inRect,inEventSet) {  }


drawGroup::drawGroup(int x, int y, int width,int height,eventSet inEventSet)
	: drawObj(x,y,width,height,inEventSet) {  }
	
	
// We don't actually allocate anything new. Just work old tools.
drawGroup::~drawGroup() { }



// Another handy tool. Sometimes you just KNOW everyone you manage
// Needs a refresh. Typically when scrolled, subObjects don't know
// they've been moved. Why? Because their offsets move with them.
// This sets everyone's needRefresh flag.
void drawGroup::setGroupRefresh(bool refresh) {
		
		drawObj* trace;

		trace = theList();										// Make sure we're at the top.
		while(trace) {
			trace->setNeedRefresh(refresh);
			trace = (drawObj*)trace->dllNext;
		} 
	}
	
	
bool drawGroup::checkGroupRefresh(void) {
	
	drawObj* trace;

	trace = theList();										// Make sure we're at the top.
	while(trace && trace!=(&listHeader)) {
		if(trace->wantRefresh()) {
			return true;
		}
		trace = (drawObj*)trace->dllNext;
	}
	return false;
}


// The viewMgr that we live on wants to know if we need a redraw.
// If we don't, the kids might.
bool drawGroup::wantRefresh(void) { return (needRefresh || checkGroupRefresh()); }


// Someone from the outside is hinting to us that we are
// due for a redraw. We'd better let the kids know too..
void drawGroup::setNeedRefresh(bool refresh) {

	drawObj::setNeedRefresh(refresh);
	setGroupRefresh(refresh);
}


// When we move, the subs can't tell 'less we tell 'em.
void drawGroup::setLocation(int x,int y) {

		drawObj::setLocation(x,y);
		setGroupRefresh();
}


// First we see if a list item wants the click. then, if not
// and we are accepting clicks, we take it.
bool drawGroup::acceptEvent(event* inEvent,point* locaPt) {
   
   bool	success;
   
   success = false;												// No accepted yet.
   if (theTouched==this) {										// Oh God no! Its me?!
   	return drawObj::acceptEvent(inEvent,locaPt);		// In this one case, we are being dragged as a group.
   }
	if (inRect(locaPt)) {										// It's in our list or us, somewhere.
		screen->pushOffset(x,y);								// Ok, push our offset for the sublist.
		success = checkEvents(inEvent);						// See if they handle it.
		screen->popOffset(x,y);									// Pop off the offset.
		if (!success) {											// If no one took it.
			return drawObj::acceptEvent(inEvent,locaPt);	// We return the standard check on ourselves.
		}
	}
	return success;												// Return the ultimate result.
}


// The groups are NOT linked into the drawing loop.
// They get their draw calls from the parent getting called.              					
void drawGroup::addObj(drawObj* newObj) {

  	newObj->linkAfter(&listHeader);   	// Put the new guy at the top of the list.
	needRefresh = true;	
}


// Drawing for a group. Sometimes we are visabile sometimes not. Also, drawing is
// done as a stack. Start at the bottom and draw all the pictures 'till you reach
// the top. We as the group are defined as the bottom of this stack.
void	drawGroup::draw(void) {

	drawObj*	trace;

	if (needRefresh) {								// We may be the background.
		eraseSelf();
		drawSelf();										// Do our drawing.
		setGroupRefresh();							// If we're drawing, the entire group needs it.
	}
	trace = (drawObj*)listHeader.getLast();	// make sure we're at the bottom.
	screen->pushOffset(x,y);						// Ok, push our offset for the sublist.
	while(trace && trace!=(&listHeader)) {		// And while we're not done..
		if(trace->wantRefresh()) {					// If this guy needs it.
			trace->draw();								// Call his Draw method.
		}
		trace = (drawObj*)trace->dllPrev;		// Hop to the next one up the list.
	}
	screen->popOffset(x,y);							// Pop off the offset.
	needRefresh = false;								// In any case, we no longer need a refresh.
}


// I'm pretty sure we don't want to be calling idle like viewMgr.
// We're NOT the one to initiate the event chain. Inherited can
// overwrite and do stuff, that'll be fine. This should block
// bad behavior by us.
void	drawGroup::idle(void) { }



// ***************************************************
// drawList is the same as a drawGroup except it arranges
// its sub Objects in list form.
// ***************************************************


drawList::drawList(rect* inRect,eventSet inEventSet,bool vertical) 
	: drawGroup(inRect,inEventSet) {

	mVertical = vertical;	// Save it for now, deal with coding it later.
	itemHeight	= 1;
	itemWidth	= 1;
}


drawList::drawList(int x, int y, int width,int height,eventSet inEventSet,bool vertical) 
	: drawGroup(x,y,width,height,inEventSet) {

	mVertical = vertical;	// Save it for now, deal with coding it later.
	itemHeight	= 1;
	itemWidth	= 1;
}


drawList::~drawList() { }
	
 
// Adding list items we assume they are all the same height.
// We reset our internal list item height each time one is added.
// So if they are NOT all uniform, we treat them like they are
// anyway. Too bad, inherit and fix it if this ain't good enough.					
void drawList::addObj(drawObj* newObj) {

	if (mVertical) {
		itemHeight = max(itemHeight,newObj->height);
		newObj->setLocation(0,numObjects()*itemHeight);
	} else {
		itemWidth = max(itemWidth,newObj->width);
		newObj->setLocation(numObjects()*itemWidth,0);
	}
	newObj->linkToEnd(&listHeader);   									// Put the new guy at the BOTTOM of the list.
	needRefresh = true;
}


// Perhaps you deleted an item from your list? This'll patch the hole.
// This sets the list to item one at top (or left).
void drawList::setPositions(int offset) {

	int		i;
	drawObj*	trace;
	
	i = 0;														// Start our multiplier at zero.
	trace = theList();
	while(trace!=NULL) {										// Standard loop through link list.
		trace->y = offset + i++ *  itemHeight;			// Each is moved by itemHeight from the last.
		trace->setNeedRefresh();							// Just in case, everyione is told to redraw.
		trace = (drawObj* )trace->dllNext;				// Hop to the next in the list.
	}
}


// Seeing that all the list items are positioned as if they were the same size..
// We can calculate the last possible y position an item can have to be viewable.
int drawList::lastY(void) {
	
	int	numViewable;
	
	numViewable = (height/itemHeight);	// Quick int division to force truncation.
	return (numViewable-1) * itemHeight;		// To make sure first is at zero. All overages to bottom.
}


// Any of our items can be passed in and find if its showing or not.
// Good for deciding to draw or not.
bool drawList::isVisible(drawObj* theItem) { return (theItem->y >= 0 && theItem->y<= lastY()); }


// If this itm is not showing, show it. If above the window, bring it down to the first
// position. If below the window bring it up to the last position of the window.
void drawList::showItem(drawObj* theItem) {

  int       yDif;
  drawObj*  trace;
  
  if (theItem->y<0) {							// Need to scroll down.
    yDif = theItem->y;							// Simple, we just bring it down to zero.
  } else if (theItem->y>lastY()) {			// Need to scroll up.
    yDif = theItem->y - lastY();				// This gives us how much to scroll up.
  } else {
    return;                               // No scrolling neccisary. Lets go home.
  }
  trace = theList();                      // EVERYBODY must scroll!
  while(trace!=NULL) {                    // Standard loop through link list.
    trace->y = trace->y-yDif;             // Each is moved by - xDif;
    trace->setNeedRefresh();              // Just in case, everyione is told to redraw.
    trace = (drawObj* )trace->dllNext;    // Hop to the next in the list.
  }
}


// What's the index of the guy at this position?
drawObj* drawList::findItem(point* where) {
	
	drawObj*  trace;
	
	trace = theList();
	while(trace!=NULL) {                    // Standard loop through link list.
		if (trace->inRect(where)) {
			return trace;
		}
		trace = (drawObj* )trace->dllNext;    // Hop to the next in the list. 
	}
	return NULL;
}

