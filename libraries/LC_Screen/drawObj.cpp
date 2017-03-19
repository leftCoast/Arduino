#include "drawObj.h"

#define DEF_LOC_X  16    // Just anything so the user can see something.
#define DEF_LOC_Y  16
#define DEF_SIZE_X 16
#define DEF_SIZE_Y 16

rect::rect(void) {
   
   setLocation(DEF_LOC_X,DEF_LOC_Y);
   setSize(DEF_SIZE_X,DEF_SIZE_Y);
 }
 
 
rect::rect(int inX, int inY, word inWidth,word inHeight) {
   
   setLocation(inX,inY);
   setSize(inWidth,inHeight);
 }

 
rect::~rect(void) { }
 
 
void  rect::setLocation(int inX, int inY) {
  
  x = inX;
  y = inY;
}


void  rect::setSize(word inWidth,word inHeight) {

  width = inWidth;
  height = inHeight;
}


void rect::setRect(rect* inRect) {
  
  setLocation(inRect->x,inRect->y);
  setSize(inRect->width,inRect->height);
}


void rect::setRect(point* inPt1,point* inPt2) {

  int locX = min(inPt1->x,inPt2->x);
  int locY = max(inPt1->y,inPt2->y);
  word width = abs((int)inPt1->x-(int)inPt2->x);
  word height = abs((int)inPt1->y-(int)inPt2->y);
  setLocation(locX,locY);
  setSize(width,height);
}


int rect::maxX(void) { return(x + width); }
int rect::maxY(void)  { return(y + height); }
int rect::minX(void) { return(x); }
int rect::minY(void)  { return(y); }

bool rect::inRect(int inX, int inY) {

  return(
  inX >= minX() &&
  inX <= maxX() &&
  inY >= minY() &&
  inY <= maxY()
  );
}


point rect::getCorner(rectPt corner) {

	point	tempPt;
	
	switch(corner) {
		case topLeftPt :
			tempPt.x = x;
			tempPt.y = y;
		break;
		case topRightPt :
			tempPt.x = x + width;
			tempPt.y = y;
		break;
		case bottomLeftPt :
			tempPt.x = x;
			tempPt.y = y + height;
		break;
		case bottomRightPt :
			tempPt.x = x + width;
			tempPt.y = y + height;
		break;
	}
	return tempPt;
}


// Are we touching this passed in rectangle?
bool rect::overlap(rect* checkRect) {

		if(maxX()<checkRect->minX()) return false;
		if(minX()>checkRect->maxX()) return false;
		if(maxY()<checkRect->minY()) return false;
		if(minY()>checkRect->maxY()) return false;
		return true;
	}




// ***********************************

drawObj::drawObj() {

  needRefresh = true;			// Well Duh! We never been drawn yet!
  focus = false;					// But we're hopefull.
  clicked = false;
  wantsClicks = false;		// 'Cause this is actually the default.
  callback = NULL;				// And, no.. We have none of this either.
}


drawObj::drawObj(int inLocX, int inLocY, word inWidth,word inHeight,boolean inClicks)
    : rect(inLocX,inLocY,inWidth,inHeight) {
    
    needRefresh = true;
    focus = false;
    clicked = false;
    wantsClicks = inClicks;
    callback = NULL;
}


// Wake up! Time to die.
drawObj::~drawObj() {

		if (currentFocus == this) {	// We were great..
			currentFocus = NULL;			// But, its all over now.
		}
	}


// When the manager asks if we want a refresh..
boolean drawObj::wantRefresh(void) { return needRefresh; }


// Sometimes others know better than us that we need a refresh.
// This allows them to make that happen.
void drawObj::setNeedRefresh(void) { needRefresh = true; }


void drawObj::setLocation(int x,int y) {

		rect::setLocation(x,y);
		needRefresh = true;
}
	
	
// Call this one to draw..
void  drawObj::draw(void) {

  drawSelf();
  needRefresh = false;
}


// override this one and draw yourself.
void  drawObj::drawSelf(void) {
  
  screen->fillRect(x, y, width, height, &black); // Default draw.
  screen->drawRect(x, y, width, height, &white);
}


// We are either getting or loosing focus.
void  drawObj::setFocus(boolean setLoose) {

	focus = setLoose;
	needRefresh = true;
}
	
				
// The ability to control this is handy..
void drawObj::clickable(boolean inWantsClicks) { wantsClicks = inWantsClicks; }


// Manager has detected a fresh click, is it ours?
boolean   drawObj::acceptClick(point where) {
        
    if (wantsClicks) {
        if (inRect(where.x,where.y)) {
            needRefresh = true;
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


void drawObj::setCallback(void (*funct)(void)) { callback = funct; }



// ***************************************************
// viewMgr, This is they guy that runs the screenshow.
// Handles redrawing, clicks, etc.
// ***************************************************


viewMgr 	viewList;
drawObj*	currentFocus = NULL;	// Totally global because? There's only ONE user.

void	setFocusPtr(drawObj* newFocus) {

		if (newFocus!=currentFocus) {					// People are lazy. Now they don't need to check.
			if (newFocus) {											// If we're actually passed in something.
				if (currentFocus) {								// Check for NULL..
					currentFocus->setFocus(false);	// Warn them that their star is falling.
				}
				currentFocus = newFocus;					// The up and coming..
				currentFocus->setFocus(true);			// Tell 'em their star is rising!!
			} else {														// We got a NULL.
				currentFocus = NULL;							// Just stamp it in.
			}
		}
	}
	
	
viewMgr::viewMgr(void) {
    
    theList = NULL;
    theTouched = NULL;
}


viewMgr::~viewMgr(void) { dumpList(); }


// Delete all the objects. Watch out! Were these
// created on the stack or in memory? If on the
// stack don't do this! ONLY use for dynamic lists!
void  viewMgr::dumpList(void) {

	drawObj*	trace;
	
	while(theList) {
		trace = (drawObj*)theList->next;
		delete(theList);
		theList = trace;
	}
  theTouched = NULL;
}


// New objects go on top.
void viewMgr::addObj(drawObj* newObj) {
    
    if (theList) {                      // We got a list?
        newObj->linkToStart(theList);   // Put the new guy at the top of the list.
    } else {
        hookup();                       // On our first addition, hook up idling.
    }
    theList = newObj;                   // Either way, point to the top of the list.
}


// Checking clicks, non-empty list already checked.
boolean viewMgr::checkClicks(void) {
    
    drawObj*    trace;
    point       gPoint;
    point       lPoint;
    boolean     done;
    boolean     success;
    
    if (!screen->hasTouchScreen()) return false;			// Sanity! Does it even have the hardware?
    success = false;                                  // Did we change anything? Not yet.
    if (!theTouched) {                                // No current touch.
        if (screen->touched()) {                      // New touch, go look.
            gPoint = screen->getPoint();              // Touch was here..
            lPoint = screen->lP(gPoint);							// Possibly we're sublist, localize.						
            trace = (drawObj*)theList->getFirst();    // make sure we're at the top.
            done = false;															// Get ready..
            while(!done) {														// While were not done
                if (trace->acceptClick(lPoint)) {			// If the object accepts the click
                    theTouched = trace;								// Save that badboy's address.
                    success = true;                   // We changed something.
                    done = true;                      // Loop's done
                } else if (trace->next) {             // Else if we have a next object.
                    trace = (drawObj*)trace->next;    // Hop to the next.
                } else  {                             // in this last case.
                    done = true;                      // The loop is through.
                }
            }
        }
    } else {																					// We've been touched and delt with it.
        if (!screen->touched()) {                     // Touch has finally passed.
            theTouched->clickOver();                  // Tell the last touched guy its over.
            theTouched = NULL;                        // Clear out our flag/pointer.
            success = true;                           // We changed something.
        }
    }
    return success;                                   // Tell the calling method if we changed something.
}


// Checking for redraws, non-empty list already checked.
void viewMgr::checkRefresh(void) {
    
    drawObj*    trace;
    boolean     done;

    trace = (drawObj*)theList->getLast();   // make sure we're at the bottom.
    done = false;                           // Well, were not done yet are we?
    while(!done) {                          // And while we're not done..
        if (trace->wantRefresh()) {         // Does this guy want refresh?
            trace->draw();                  // Call his Draw method.
        }
        if (trace->prev) {                  // Ok, we have another up the list?
            trace = (drawObj*)trace->prev;  // if so lets go there.
        } else {                            // No one else?
            done = true;                    // Then I guess we are done.
        }
    }
}
 
 
// Counts & returns the number of objects in the list.           
word viewMgr::numObjInList(void) {

	drawObj*	trace;
	word 			count;
	
	trace = (drawObj*)theList;
	count = 0;
	while(trace) {
		count++;
		trace = (drawObj*)trace->next;
	}
	return count;
}


// Its sometimes handy to grab an object by index on the list.
drawObj* viewMgr::getObj(int index) {

	drawObj*	trace;
	int				i;
	
	i = 0;
	trace = (drawObj*)theList;
	while(trace) {
		if (index==i) return trace;
		i++;
		trace = (drawObj*)trace->next;
	}
	return NULL;
}

        
// We have time to do stuff, NOT A LOT!
void viewMgr::idle(void) {
    
    if (theList) {
        if (!checkClicks()){
            checkRefresh();
        }
   }
}



// ***************************************************
// drawGroup, This is they guy that runs a bunch of sub
// objects. He is both a drawObj and a viewMgr.
// Before taking any action with a sub object the drawGroup
// pushes its location onto the screen. This allows all sub
// objects to work on local coordinates.
//
// Once difference is that this viewMgr sublist is NOT linked
// to the idler list. Only the master list gets the idle calls.
// The sub lists get called through the drawGroup methods. 
// ***************************************************


drawGroup::drawGroup(int x, int y, word width,word height,boolean clicks) 
	: drawObj(x,y,width,height,clicks) { needRefresh = true; }


// We call dumpList() because the odds are we were
// dynamically created.
drawGroup::~drawGroup() { dumpList(); }



// Another handy tool. Sometimes you just KNOW everyone you manage
// Needs a refresh. Typically when scrolled, subObjects don't know
// they've been moved. Why? Because their offsets move with them.
// This sets everyone's needRefresh flag.
void drawGroup::setGroupRefresh(void) {
		
		drawObj* trace;

		trace = theList;
		while(trace) {
			trace->setNeedRefresh();
			trace = (drawObj*)trace->next;
		} 
	}
	

// The viewMgr that we live on wants to know if we need a redraw.
// Before we act on that, lets let the kids redraw first.
// NOTE : THATS MESSED UP. WE SHOULD DRAW FIRST THEN THE KIDS. RETHINK THIS.
boolean drawGroup::wantRefresh(void) {

	if (theList) {
		screen->pushOffset(x,y);				// So we set in our offset.
		checkRefresh();									// Now, if they get called to draw? Everything will be fine.
		screen->popOffset(x,y);					// Clear offset.
	}
	return needRefresh;
}


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
boolean   drawGroup::acceptClick(point where) {
    
    boolean	success;
    
    success = false;   
		if (inRect(where.x,where.y)) {			// It's in our list or us, somewhere.
				screen->pushOffset(x,y);				// Ok, push our offset for the sublist.
				success = checkClicks();				// See if they handle it.
				screen->popOffset(x,y);					// Pop off the offset.
				if (!success && wantsClicks) {	// No one else wanted it? Do we want clicks?
					needRefresh = true;						// We'll take it.
					doAction();										// And do things.
					success = true;								// Noted..
				}
		}
		return success;
	}


// click over. Possibly a sub obj?
void drawGroup::clickOver(void) {
	
	if (theTouched) {							// If it was a subgroup.
		theTouched->clickOver();    // Tell the last touched guy its over.
  	theTouched = NULL;          // Clear out our flag/pointer.
  }
}


// The groups are NOT linked into the idle loop.
// They get their time from the parent getting called.              					
void drawGroup::addObj(drawObj* newObj) {

  if (theList) {                      // We got a list?
  	newObj->linkToStart(theList);   	// Put the new guy at the top of the list.
  }
  theList = newObj;                   // Either way, point to the top of the list.
	needRefresh = true;	
}

 
// Do nothing as default. Its all about the sub objects.
void  drawGroup::drawSelf(void) { }



// ***************************************************
// drawList is the same as a drawGroup except it arranges
// its sub Objects in list form.
// ***************************************************


drawList::drawList(int x, int y, word width,word height,boolean clicks) 
	: drawGroup(x,y,width,height,clicks) { listHeight = 1; }


drawList::~drawList() { }
	
 
// Adding list items we assume they are all the same height.
// We reset our internal list item height each time one is added.
// So if they are NOT all uniform, we treat them like they are
// anyway. Too bad, inherit and fix it if this ain't good enough.					
void drawList::addObj(drawObj* newObj) {

	word numObjs;
	
	listHeight = newObj->height;
	numObjs = numObjInList();
	newObj->setLocation(0,numObjs*listHeight);
	drawGroup::addObj(newObj);	
}
