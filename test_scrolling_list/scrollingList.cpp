#include "scrollingList.h"



scrollingList::scrollingList(int x, int y, word width,word height,scrollType sType,bool clicks)
  : drawList(x,y,width,height,clicks) {

  mType = sType;                // The kind of scroller we are.
  if (mType == touchScroll) {   // If we're the touch type.
    clickable(true);            // We need to "feel" the touch.
  }
}


// Looks like we don't have anything to toss out. 
scrollingList::~scrollingList(void) {  }


// Many times the list itself doesn't draw. 
void scrollingList::drawSelf(void) { screen->fillRect(x,y,width,height,&red); }



// ************************************
// *********** touchScroll ************
// ************************************





// ************************************
// ************ dialScroll ************
// ************************************
// 
// This is where you run the focused list item up
// and down the list using something like a pot. So
// what we are going to do is take in a real number 
// from 0 to 100 and use this to select the item of
// choice.


// The plan is to have analog values from 0-100 sent in here.
// This figures out wich list item gets focus because of this.
void scrollingList::setScrollValue(float percent) {

  int       numItems;
  mapper    itemMapper;
  int       itemIndex;
  drawObj* newFocus;

  numItems = numObjInList();                    // How many items on the list.
  itemMapper.setValues(0,100,0,numItems-1);     // Setup a mapper to map 0-100 to 0..last item.
  itemIndex = round(itemMapper.Map(percent));   // Map to get item index.
  newFocus = getObj(itemIndex);                 // Use the index to locate the actual list item.
  setFocusPtr(newFocus);                        // Set this item as the one who has focus.  
  showItem(newFocus);                           // Do we need to scroll to show this item? Do so.
}
