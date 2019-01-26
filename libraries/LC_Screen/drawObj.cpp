#include "drawObj.h"


// ***********************************

drawObj::drawObj() {

  needRefresh = true;	// Well Duh! We never been drawn yet!
  focus = false;			// But we're hopefull.
  clicked = false;
  wantsClicks = false;	// 'Cause this is actually the default.
  callback = NULL;		// And, no.. We have none of this either.
  lastX = 0;				// Yes, these are where we were before we moved.
  lastY = 0;
}


drawObj::drawObj(rect* inRect,bool inClicks)
	: rect(inRect) {
	
	needRefresh = true;
	focus = false;
	clicked = false;
	wantsClicks = inClicks;
	callback = NULL;
	lastX = x;		
	lastY = y;
}


drawObj::drawObj(int inLocX, int inLocY, int inWidth,int inHeight,bool inClicks)
    : rect(inLocX,inLocY,inWidth,inHeight) {
    
	needRefresh = true;
	focus = false;
	clicked = false;
	wantsClicks = inClicks;
	callback = NULL;
	lastX = x;		
	lastY = y;
}


// Wake up! Time to die.
drawObj::~drawObj() {

		if (currentFocus == this) {	// We were great..
			currentFocus = NULL;			// But, its all over now.
		}
	}


// When the manager asks if we want a refresh..
bool drawObj::wantRefresh(void) { return needRefresh; }


// Sometimes others know better than us that we need a refresh.
// This allows them to make that happen.
void drawObj::setNeedRefresh(void) { needRefresh = true; }


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
void  drawObj::setFocus(bool setLoose) {

	focus = setLoose;
	needRefresh = true;
}
	
				
// The ability to control this is handy..
void drawObj::clickable(bool inWantsClicks) { wantsClicks = inWantsClicks; }


// Manager has detected a fresh click, is it ours?
bool   drawObj::acceptClick(point where) {
	if (wantsClicks) {
		if (inRect(where.x,where.y)) {
			needRefresh = true;				// Is it right to just assume this here? 1/26/2019
			clicked = true;
			doAction();
			return true;
		}
	}
	return false;
}
  
            
// We accepted a click. Well, now its over, deal with it.
void   drawObj::clickOver(void) {
    
    needRefresh = true;
    clicked = false;
}
            

// Override me for different action.
void drawObj::doAction(void) {

	if (callback) {
		  callback();
	}
}

// I tested this and it worked. But for the life of me, I can no longer figure
// out how it make it work anymore.
void drawObj::setCallback(void (*funct)(void)) { callback = funct; }



// ***************************************************
// viewMgr, This is they guy that runs the screenshow.
// Handles redrawing, clicks, etc.
// ***************************************************


viewMgr 	viewList;
drawObj*	currentFocus = NULL;	// Totally global because? There's only ONE user.

void	setFocusPtr(drawObj* newFocus) {

		if (newFocus!=currentFocus) {					// People are lazy. Now they don't need to check.
			if (newFocus) {								// If we're actually passed in something.
				if (currentFocus) {						// Check for NULL..
					currentFocus->setFocus(false);	// Warn them that their star is falling.
				}
				currentFocus = newFocus;				// The up and coming..
				currentFocus->setFocus(true);			// Tell 'em their star is rising!!
			} else {											// We got a NULL.
				currentFocus = NULL;						// Just stamp it in.
			}
		}
	}
	
	
viewMgr::viewMgr(void) { theTouched = NULL; }


viewMgr::~viewMgr(void) { listHeader.dumpList(); }



// New objects go on top.
void viewMgr::addObj(drawObj* newObj) {
    
    newObj->linkAfter(&listHeader);	// Put the new guy at the top of the list.
    hookup();								// hookup on the first addition. Ignored otherwise.
}

// Now anyone can tell us to do that.
void viewMgr::dumpDrawObjList(void) { listHeader.dumpList(); }


// Checking clicks.
bool viewMgr::checkClicks(void) {
    
	drawObj*	trace;
	point		gPoint;
	point		lPoint;
	bool		success;
    
	if (!screen->hasTouchScreen()) return false;		// Sanity! Does it even have the hardware?
	success = false;											// Did we change anything? Not yet.
	if (!theTouched) {										// No current touch.
		if (screen->touched()) {							// New touch, go look.
			gPoint = screen->getPoint();              // Touch was here..
			lPoint = screen->lP(gPoint);					// Possibly we're sublist, localize.						
			trace = (drawObj*)listHeader.getFirst();	// make sure we're at the top.
			while(trace) {										// While we have something to work with.
				if (trace->acceptClick(lPoint)) {		// If the object accepts the click
					theTouched = trace;						// Save that badboy's address.
					success = true;							// We changed something.
					trace = NULL;								// Flag that the loop's done.
				} else {
					trace = (drawObj*)trace->dllNext;	// Hop to the next.
				}
			}
		}
	} else {														// We've been touched and delt with it.
		if (!screen->touched()) {							// Touch has finally passed.
			theTouched->clickOver();                  // Tell the last touched guy its over.
			theTouched = NULL;                        // Clear out our flag/pointer.
			success = true;                           // We changed something.
		}
	}
	return success;											// Tell the calling method if we changed something.
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
int viewMgr::numObjInList(void) { return (int)listHeader.countTail(); }


// Finds the "nth" item on the list, index starting at 0. NULL if not found.
drawObj* viewMgr::getObj(int index) { return (drawObj*)listHeader.getTailObj((int)index); }


drawObj* viewMgr::theList(void) { return (drawObj*)listHeader.getTailObj(0); }


// We have time to do stuff, NOT A LOT!
void viewMgr::idle(void) {
    
	if (!checkClicks()){		// I guess if we have a click we need  to do that.
		checkRefresh();		// Otherwise check if we need to do some drawing?
	}
}



// ***************************************************
// drawGroup, This is they guy that runs a bunch of sub
// objects. He is both a drawObj and a viewMgr.
// Before taking any action with a sub object the drawGroup
// pushes its location onto the screen. This allows all sub
// objects to work on local coordinates.
// ***************************************************


drawGroup::drawGroup(rect* inRect,bool clicks) 
	: drawObj(inRect,clicks) {  } // drawObj sets wantRefresh, we don't need to. 1/21/2019


drawGroup::drawGroup(int x, int y, int width,int height,bool clicks) 
	: drawObj(x,y,width,height,clicks) {  }
	
	
// We don't actually allocate anything new. Just work old tools.
drawGroup::~drawGroup() { }



// Another handy tool. Sometimes you just KNOW everyone you manage
// Needs a refresh. Typically when scrolled, subObjects don't know
// they've been moved. Why? Because their offsets move with them.
// This sets everyone's needRefresh flag.
void drawGroup::setGroupRefresh(void) {
		
		drawObj* trace;

		trace = (drawObj*)listHeader.getFirst();
		while(trace) {
			trace->setNeedRefresh();
			trace = (drawObj*)trace->dllNext;
		} 
	}
	
	
bool drawGroup::checkGroupRefresh(void) {
	
	drawObj* trace;

	trace = (drawObj*)listHeader.getTailObj(0);
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
void drawGroup::setNeedRefresh(void) {

	drawObj::setNeedRefresh();
	setGroupRefresh();
}


// When we move, the subs can't tell 'less we tell 'em.
void drawGroup::setLocation(int x,int y) {

		drawObj::setLocation(x,y);
		setGroupRefresh();
}


// First we see if a list item wants the click. then, if not
// and we are accepting clicks, we take it.
bool   drawGroup::acceptClick(point where) {
    
	bool	success;
    
	success = false;
	if (inRect(where.x,where.y)) {			// It's in our list or us, somewhere.
		screen->pushOffset(x,y);				// Ok, push our offset for the sublist.
		success = checkClicks();				// See if they handle it.
		screen->popOffset(x,y);					// Pop off the offset.
		if (!success && wantsClicks) {		// No one else wanted it? Do we want clicks?
			needRefresh = true;					// We'll take it.
			doAction();								// And do things.
			success = true;						// Noted..
		}
	}
	return success;
}


// click over. Possibly a sub obj?
void drawGroup::clickOver(void) {
	
	if (theTouched) {					// If it was a subgroup.
		theTouched->clickOver();	// Tell the last touched guy its over.
		theTouched = NULL;			// Clear out our flag/pointer.
	}
}


// The groups are NOT linked into the drawing loop.
// They get their draw calls from the parent getting called.              					
void drawGroup::addObj(drawObj* newObj) {

  	newObj->linkAfter(&listHeader);   	// Put the new guy at the top of the list.
	needRefresh = true;	
}


void	drawGroup::draw(void) {

	drawObj*	trace;

	if (needRefresh) {								// We may be the background.
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



// ***************************************************
// drawList is the same as a drawGroup except it arranges
// its sub Objects in list form.
// ***************************************************


drawList::drawList(rect* inRect,bool clicks,bool vertical) 
	: drawGroup(inRect,clicks) {

	mVertical = vertical;	// Save it for now, deal with coding it later.
	itemHeight = 1;
}


drawList::drawList(int x, int y, int width,int height,bool clicks,bool vertical) 
	: drawGroup(x,y,width,height,clicks) {

	mVertical = vertical;	// Save it for now, deal with coding it later.
	itemHeight = 1;
}


drawList::~drawList() { }
	
 
// Adding list items we assume they are all the same height.
// We reset our internal list item height each time one is added.
// So if they are NOT all uniform, we treat them like they are
// anyway. Too bad, inherit and fix it if this ain't good enough.					
void drawList::addObj(drawObj* newObj) {

	itemHeight = max(itemHeight,newObj->height);
	newObj->setLocation(0,numObjInList()*itemHeight);
	drawGroup::addObj(newObj);	
}


// Good idea after adding your list items to run a reset on them all.
void drawList::resetPositions(void) {

	int		i;
	drawObj*	trace;
	
	i = 0;											// Start our multiplier at zero.
	trace = theList();
	while(trace!=NULL) {							// Standard loop through link list.
		trace->y = i++ *  itemHeight;			// Each is moved by itemHeight from the last.
		trace->setNeedRefresh();				// Just in case, everyione is told to redraw.
		trace = (drawObj* )trace->dllNext;	// Hop to the next in the list.
	}
}


// Seeing that all the list items are positioned as if they were the same size..
// We can calculate the last possible y position an item can have to be viewable.
int drawList::lastY(void) {
	
	int       numViewable;
	
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

