#include "scrollingList.h"


scrollingList::scrollingList(int x, int y, int width,int height, scrollType sType,eventSet inEventSet,bool vertical)
	: drawList(x,y,width,height,inEventSet,vertical) {

	mType = sType;                // The kind of scroller we are.
	mLoc = onList;
}


// Looks like we don't have anything to toss out. 
scrollingList::~scrollingList(void) { }



// ************************************
// *********** touchScroll ************
// ************************************
//
// Well, drag it around with your finger. (Gulp!)

void scrollingList::dragVertical(event* inEvent) {
	
	int		clicks;
	int		dY;
	int		startY;
	int		lastY;
	drawObj*	firstItem;
	
	firstItem = theList();												// Get pointer to the first item on list.								
	if (firstItem) {														// Sanity check, is there an item here?
		clicks = inEvent->mYDist/itemHeight;						// Figure out, in units of item height, how far we've draged.
		if (clicks!=0) {													// If we have dragged more than an item height..
			dY = clicks * itemHeight;									// Truncate it back to pixels. (Gives us even item jumps)
			startY = mStartPt.y + dY;									// This is where the user dragged the top corner.
			lastY = startY + itemHeight * numObjects();			// And where she drug the bottom corner.
			if (startY>0) {												// If the drag ended up with the top below our window..
				return;														// We're not doing that!
			} else if (lastY<height) {									// Else, if we want to scroll the bottom too far up?
				return;														// We're not doing that either!
			} else {															// Else everything is, I guess ok..
				setPositions(startY);									// We'll do that.
			}
		}
	}
}


void scrollingList::dragHorizontal(event* inEvent) {

	// After vertical is working, I'll look into writing this one.
}


void scrollingList::doAction(event* inEvent,point* locaPt) {
	
	drawObj*	trace;
	
	if (inEvent->mType==dragBegin) {			// If its the start of a drag..
		trace = theList();						// Locate the top item of the list.
		if (trace) {
			mStartPt.x = trace->x;				// Save off where it is now.
			mStartPt.y = trace->y;				// More save off where it is now.
		}
	} else if (inEvent->mType==dragOn) {	// Or else, we're actually dragging..
		if (mVertical) {							// If we're a vertical list..
			dragVertical(inEvent);				// Deal with a vertical drag.
		} else {										// Or esle we are a horizontal list..
			dragHorizontal(inEvent);			// Deal with a horizontal drag.
		}
	} else if (inEvent->mType==liftEvent) {	// Or else, we're finished dragging..
		needRefresh = false;							// We shouldn't need the refresh. We only scroll full lists. (No gaps)
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

  numItems = numObjects();            	        // How many items on the list.
  switch(mType) {
    case dialScroll : itemMapper.setValues(0,100,0,numItems-1); break;  // Setup the mapper, 0-100 to 0..last item. Just the list.
    case dSOpenTop  : itemMapper.setValues(0,100,-1,numItems-1); break; // Setup the mapper,0-100 to -1..last item. The list and "above".
    case dSOpenBtm  : itemMapper.setValues(0,100,0,numItems); break;    // Setup the mapper,0-100 to 0..last item + 1. The list and "below".
    case dSOpenBoth : itemMapper.setValues(0,100,-1,numItems); break;   // Setup the mapper,0-100 to -1..last item +1 . The list above and below.
    case touchScroll		: break;	// These next two are for shutting up the compiler.
    case buttonScroll	: break;
  }
  itemIndex = round(itemMapper.map(percent));   // Map to get item index.
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
