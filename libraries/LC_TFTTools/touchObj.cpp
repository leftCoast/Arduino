#include "touchObj.h"

touchList theTouchList;

// ****************************
// touchObj
// ****************************


touchObj::touchObj() { state = active; }


touchObj::touchObj(TS_Point inLoc, word inWidth,word inHeight) :
drawObj(inLoc,inWidth,inHeight) {

  state = active;
}


touchObj::~touchObj() { }


void touchObj::drawSelf(void) {

  
  switch (state) {
  case active :
    screen->fillRect(location.x, location.y, width, height, GREEN);
    break;
  case inactive :
    screen->fillRect(location.x, location.y, width, height, BLACK);
    break;
  case touched :
    screen->fillRect(location.x, location.y, width, height, RED);
    break;
  case dragging :
    screen->fillRect(location.x, location.y, width, height, BLUE);
    break;
  };
}



void touchObj::setState(byte inState) {

  if (inState==active && (state==touched||state==dragging)) { // We've been active and now we're done. let the list know.
    theTouchList.deselect();
  }
  state = inState;
  setRefresh();
}


boolean touchObj::checkActiveTouch(TS_Point where) {

  if (screen->touched(where)) {    // Its a touch
    if (inRect(where)) {           // and its in us!
      setState(touched);           // we are so touched!
      return(true);                // we delt with it!
    }
  } 
  return(false);                  // Any other case, its not our problem..
}


boolean touchObj::checkInactiveTouch(TS_Point where) { return(false); }


boolean touchObj::checkTouchedTouch(TS_Point where) {

  if (!screen->touched(where)) {    // It a lift!
    if (inRect(where)) {            // And its in us!
      doAction(where);
      setState(active);
      return(true);                 // we delt with it!
    } 
    else {                          // Not in us, user cancelled.
      setState(active);
      return(true);                 // we delt with it.
    } 
  } 
  else {                            // its not a lift.
    if (state == dragable) {
      setState(dragging);
      return checkDraggingTouch(where);
    } 
    else {                          // We don't drag, but we're active..
      return(true);                 // We have to take the touch anyway.
    }
  }
} 


boolean touchObj::checkDraggingTouch(TS_Point where) {

  // Lets write this code later..
}


boolean touchObj::checkTouch(TS_Point where) {

  switch(state) {
  case active :  return (checkActiveTouch(where)); break;
  case inactive : return (checkInactiveTouch(where)); break;
  case touched : return (checkTouchedTouch(where)); break;
  case dragging : return (checkDraggingTouch(where)); break;
  }
}


void touchObj::doAction(TS_Point where) {  }



// ****************************
// touchList
// ****************************


touchList::touchList(void) :
idler(), linkList(false) { selected = NULL; }


touchList::~touchList(void) { }


void touchList::addObj(touchObj* newObj) {
  
  if (!hookedIn) {
    hookup();          // If they can add stuff to us, we can hookup.
  }
  addToEnd((linkListObj*) newObj);
}


void touchList::deselect(void) { 
  selected = NULL; 
}


void touchList::idle(void) {

  TS_Point   where;
  boolean    done;

  //where = screen->getPoint();
  //screen->drawPixel(where.x, where.y, RED);
  if (selected == NULL) {                                            // If no one is selected.
    if (screen->touched(where)) {                                    // And the thing is pressed.
      selected = (touchObj*) theList;                                // Set up to search.
      done = false;                                                  // Not done yet..
      while(selected!=NULL&&!done) {                                 // See if anyone wants this
        done = selected->checkTouch(where);                          // if someone acceps the touch, we're done.
        if (!done) {
          selected = (touchObj*)selected->next;
        }
      }
    }
  } 
  else {                                     // At this point someone is selected!
    if (!selected->checkTouch(where)) {      // Selected doesn't want any more clicks?
      deselect();                            // Fine, no longer selected.
    }
  }
}

