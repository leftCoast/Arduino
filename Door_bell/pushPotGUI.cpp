#include "pushPotGUI.h"
#include <drawObj.h>

// This on is to be called for setting the controller.
void  setControlPtr(pushPotGUI* newController) {

  controlPtr = newController;      // The up and coming..
  if (controlPtr) {                // If we're actually passed in something.
      controlPtr->gainControl();   // Tell 'em they got the wheel!
  }
}


// Send the pot val to whomever is the current controller.
// Returns false if theris no controller. True if there is one.
// Don't have a clue as to what it will do with it.
bool  potVal(int aVal) {

  if (controlPtr) {               // Check for NULL..
    controlPtr->doPotVal(aVal);  // Try this on for size..
    return true;
  }
  return false;
}


// Send the button click to whomever has current focus.
// Returns false if theris no one with focus. True if there
// was one.  Don't have a clue as to what it will do with it.
bool  buttonClick(void) {

  if (currentFocus) {
    currentFocus->doAction(); 
    return true;
  }
  return false;
}

pushPotGUI*  controlPtr = NULL;



pushPotGUI::pushPotGUI(void) { }
pushPotGUI::~pushPotGUI(void) { if (controlPtr==this) setControlPtr(NULL); }

// Stuff to do when we're started or restarted.
void pushPotGUI::reset(void) {  }           


// Notifyer, you have the wheel now, take appropriate action.
void pushPotGUI::gainControl(void) {  }


// Calliing program sends us in raw pot values.
// I guess we're the controller.
void pushPotGUI::doPotVal(int aVal) {  }  





