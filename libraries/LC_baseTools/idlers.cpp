#include "idlers.h"

// The global idler object  list and call for "loop();"
idlers theIdlers;

void idle(void) { 
  theIdlers.idle(); 
}


// *******************************
// idler, base object to give things the ability to run behind the scenes.
// *******************************

idler::idler(void) { 
  hookedIn = false; 
}

idler::~idler(void) {

  theIdlers.deleteObj(this);
}


// You can't call this in the contstructor. Love to but can't.
// So call this the first time you add data or turn your idler on.
void idler::hookup(void) {
  //Serial.println("Attempting to hookup an idler");
  if (!hookedIn) {
    //Serial.print("calling idlers addToTop List: "); Serial.print((unsigned long)&theIdlers); Serial.print("  newObj: "); Serial.println((unsigned long) this);
    theIdlers.addToTop(this); 
    hookedIn = true;
  }
}


// idle. We don't do anything here, but whomever inherits us will.
void idler::idle(void) { }



// *******************************
// idlers, management for the list of idlers. 
// Is delared as a global and runs in th background.
// *******************************

idlers::idlers(void) : linkList(false) {  }


idlers::~idlers(void) { }


// Run down the list and call the idle() method on each one.
void idlers::idle(void) {

  idler* trace;

  trace = (idler*) theList;
  while(trace!=NULL) {
    trace->idle();
    trace = (idler*)trace->next;
  }
}


