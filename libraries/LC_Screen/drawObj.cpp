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
  wantsClicks = false;		// 'Cause this is actually the default.
  parentObj = NULL;				// Well, there ain't one yet..
  callback = NULL;				// And, no.. We have none of this either.
}


drawObj::drawObj(int inLocX, int inLocY, word inWidth,word inHeight,boolean inClicks)
    : rect(inLocX,inLocY,inWidth,inHeight) {
    
    needRefresh = true;
    clicked = false;
    wantsClicks = inClicks;
    parentObj = NULL;
    callback = NULL;
}


drawObj::~drawObj() { }


// When the manager asks if we want a refresh..
boolean drawObj::wantRefresh(void) { return needRefresh; }


// We may be a sub object of another, this gives the x we draw to.
int drawObj::scrX(void) {

	if (parentObj) {
		return(parentObj->scrX() + x);
	} else {
		return x;
	}
}


// Just like the guy above except gives the y we draw to.
int drawObj::scrY(void) {

	if (parentObj) {
		return(parentObj->scrY() + y);
	} else {
		return y;
	}
}


// 
rect drawObj::scrRect(void) {

	rect temp(scrX(),scrY(),width,height);
	return temp;
}

void drawObj::setLocation(int inX,int inY) {

		rect::setLocation(inX,inY);
		needRefresh = true;
}
	
	
// Call this one to draw..
void  drawObj::draw(void) {

  drawSelf();
  needRefresh = false;
}


// override this one and draw yourself.
void  drawObj::drawSelf(void) {
  
  Serial.print("x, y ");Serial.print(x);Serial.print(", ");Serial.println(y);
  Serial.print("scrX(), scrY() ");Serial.print(scrX());Serial.print(", ");Serial.println(scrY());
  Serial.print("width, height ");Serial.print(width);Serial.print(", ");Serial.println(height);
  screen->fillRect(scrX(), scrY(), width, height, &black); // Default draw.
  screen->drawRect(scrX(), scrY(), width, height, &white);
}


// The ability to control this is handy..
void drawObj::clickable(boolean inWantsClicks) { wantsClicks = inWantsClicks; }


// Manager has detected a fresh click, is it ours?
boolean   drawObj::acceptClick(point where) {
    
    rect	gRect;
    
    if (wantsClicks) {
    		gRect = scrRect();
        if (gRect.inRect(where.x,where.y)) {
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

viewMgr viewList;

viewMgr::viewMgr(void) {
    
    theList = NULL;
    theTouched = NULL;
}


viewMgr::~viewMgr(void) {
    
    if (theList) {
        while(theList->next) delete theList->next;
        while(theList->prev) delete theList->prev;
        delete theList;
        theList = NULL;
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
    point       where;
    boolean     done;
    boolean     success;
    
    success = false;                                                // Did we change anything? Not yet.
    if (!theTouched) {                                              // No current touch.
        if (screen->touched()) {                                    // New touch, go look.
            where = screen->getPoint();                             // Touch was here..
            trace = (drawObj*)theList->getFirst();                  // make sure we're at the top.
            done = false;
            while(!done) {                                          // While were not done
                if (trace->acceptClick(where)) {                    // If the object accepts the click
                    theTouched = trace;                             // Save that badboy's address.
                    success = true;                                 // We changed something.
                    done = true;                                    // Loop's done
                } else if (trace->next) {                           // Else if we have a next object.
                    trace = (drawObj*)trace->next;                  // Hop to the next.
                } else  {                                           // in this last case.
                    done = true;                                    // The loop is through.
                }
            }
        }
    } else {                                                        // We've been touched and delt with it.
        if (!screen->touched()) {                                   // Touch has finally passed.
            theTouched->clickOver();                                // Tell the last touched guy its over.
            theTouched = NULL;                                      // Clear out our flag/pointer.
            success = true;                                         // We changed something.
        }
    }
    return success;                                                 // Tell the calling method if we changed something.
}
    

// Checking for redraws, non-empty list already checked.
void viewMgr::checkRefresh(void) {
    
    drawObj*    trace;
    point       where;
    boolean     done;

    trace = (drawObj*)theList->getLast();   // make sure we're at the bottom.
    done = false;                           // Well, were not done yet are we?
    while(!done) {                          // And while we're not done..
        if (trace->wantRefresh()) {         // Does this guy want  refresh?
            trace->draw();                  // Call his Draw method.
        }
        if (trace->prev) {                  // Ok, we have another up the list?
            trace = (drawObj*)trace->prev;  // if so lets go there.
        } else {                            // No one else?
            done = true;                    // Then I guess we are done.
        }
    }
}
            
            
// We have time to do stuff, not a lot!
void viewMgr::idle(void) {
    
    if (theList) {
        if (!checkClicks()){
            checkRefresh();
        }
   }
}
 
