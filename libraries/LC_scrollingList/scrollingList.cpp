#include "scrollingList.h"


scrollingList::scrollingList(int x, int y, word width,word height,scrollType sType,eventSet inEventSet,bool vertical)
	: drawList(x,y,width,height,inEventSet,vertical) {

	mType = sType;                // The kind of scroller we are.
	mLoc = onList;
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

void scrollingList::dragVertical(event* inEvent) {
	
	int	clicks;
	int	dY;
	
	clicks = inEvent->mYDist/itemHeight;
	Serial.print("Clicks : ");Serial.println(clicks);
	dY = clicks * itemHeight;
	if (dY>0) {										// dY>0 is down.
		for (int i=0;i<abs(clicks);i++) {
			movelist(false);
		}
	} else {
		for (int i=0;i<abs(clicks);i++) {
			movelist(true);
		}
	}
}


void scrollingList::dragHorizontal(event* inEvent) {

}


void scrollingList::doAction(event* inEvent,point* locaPt) {
	
	drawObj*	trace;
	
	if (inEvent->mType==dragBegin) {
		trace = theList();
		mStartPt.x = trace.x;
		mStartPt.y = trace.y;
	} else if (inEvent->mType==dragOn) {
		if (mVertical) {
			dragVertical(inEvent);
		} else {
			dragHorizontal(inEvent);
   	}
	}
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
