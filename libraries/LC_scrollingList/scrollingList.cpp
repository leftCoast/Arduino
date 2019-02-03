#include "scrollingList.h"

#define DRAG_TIME		750	// How long to wait before we call it a drag?
#define DRAG_RAD		50		// Distacnce window for "Oh yeah, he's dragging".

scrollingList::scrollingList(int x, int y, word width,word height,scrollType sType,bool clicks,bool vertical)
	: drawList(x,y,width,height,clicks,vertical) {

	mType = sType;                // The kind of scroller we are.
	mLoc = onList;
	if (mType == touchScroll) {   // If we're the touch type.
		touchTimer = new timeObj(DRAG_TIME);
	}
}


// Looks like we don't have anything to toss out. 
scrollingList::~scrollingList(void) {

	if (touchTimer) { delete touchTimer; }
}


// Many times the list itself doesn't draw. 
void scrollingList::drawSelf(void) { }



// ************************************
// *********** touchScroll ************
// ************************************
//
// Well, drag it around with your finger. (Gulp!)

void scrollingList::dragVertical(void) {
	
	point		now;
	int		delta;
	mapper	scrolTimeMApper(0,40,100,20);
	timeObj	scrollTimer(100);
	int		timeCount;
	
	Serial.println("Vertical drag");
	timeCount = 0;
	while(screen->touched()) {
		now = screen->getPoint();
		delta = now.y - mTouchPoint.y;
		if (scrollTimer.ding()){
			timeCount++;
			scrollTimer.start();
			if (timeCount>10) {
				movelist(delta<0);
				timeCount = 0;
				scrollTimer.setTime(scrolTimeMApper.Map(delta));
			}
		}
	}
}


void scrollingList::dragHorizontal(void) {

}


void scrollingList::doDrag(void) {

	if (mVertical) {
		dragVertical();
	} else {
		dragHorizontal();
	}
}


int scrollingList::dragLen() { return round(distance(mTouchPoint,screen->getPoint())); }


// This didn't work anyway.
bool scrollingList::acceptClick(point where) {
}
	

// ************************************
// ************ dialScroll ************
// ************************************
// 
// This is where you run the focused list item up
// and down the list using something like a pot. So
// what we are going to do is take in a real number 
// from 0 to 100 and use this to select the item of
// choice.


// Posiblly we allow running off the list? Let the user know here.
void scrollingList::offList(void) {  }


// The plan is to have analog values from 0-100 sent in here.
// This figures out wich list item gets focus because of this.
void scrollingList::setScrollValue(float percent) {

  int       numItems;
  mapper    itemMapper;
  int       itemIndex;
  drawObj* newFocus;

  numItems = numObjInList();                    // How many items on the list.
  switch(mType) {
    case dialScroll : itemMapper.setValues(0,100,0,numItems-1); break;  // Setup the mapper, 0-100 to 0..last item. Just the list.
    case dSOpenTop  : itemMapper.setValues(0,100,-1,numItems-1); break; // Setup the mapper,0-100 to -1..last item. The list and "above".
    case dSOpenBtm  : itemMapper.setValues(0,100,0,numItems); break;    // Setup the mapper,0-100 to 0..last item + 1. The list and "below".
    case dSOpenBoth : itemMapper.setValues(0,100,-1,numItems); break;   // Setup the mapper,0-100 to -1..last item +1 . The list above and below.
  }
  itemIndex = round(itemMapper.Map(percent));   // Map to get item index.
  if (itemIndex==-1) {                          // Below 0, abovve list.
    mLoc = aboveList;                           // setOur location type to above.
    offList();                                  // Let 'em know if they want.
  } else if (itemIndex==numItems) {             // numItems is actually one past the list.
    mLoc = belowList;                           // setOur location type to below.
    offList();                                  // They may want to know, being polite and all..
  } else {
    mLoc = onList;                              // Your standard scrolling..
    newFocus = getObj(itemIndex);               // Use the index to locate the actual list item.
    setFocusPtr(newFocus);                      // Set this item as the one who has focus.  
    showItem(newFocus);                         // Do we need to scroll to show this item? Do so.
  }
}
