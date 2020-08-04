#include "timeObj.h"
#include "idlers.h"

// ************************************************************** 
// delayTrigger is your base class. Inherit this to make your
// actual triggered classes.
// **************************************************************


class delayTrigger : public timeObj,
                     public idler {

   public:
               delayTrigger(float delayTime);
   virtual     ~delayTrigger(void);
   
   virtual     void  doAction(void);   // Derived classes inherit this and fill it out.
   
   // Probably ignore these.. (under hood kind of stuff)
   virtual     void  start(void);
   virtual     void  stepTime(void);
   virtual     void  idle(void);

               bool  mTriggered;
};


delayTrigger::delayTrigger(float delayTime)
   : timeObj(delayTime),
   idler() { mTriggered = false; }

 
delayTrigger::~delayTrigger(void) {  }  


// This one is filled out by the inherited class.  
void  delayTrigger::doAction(void) { }


// These next few are just for tying all the bits together.
// We need to add the hookup() call and to clear the triggered flag.
void delayTrigger::start(void) {
 
   hookup();
   mTriggered = false;
   timeObj::start();
}


// Also need to clear the triggered flag here as well.
void delayTrigger::stepTime(void) {

   mTriggered = false;
   timeObj::stepTime();
}


// This thing runs behind in the background. Think of this
// as your own private slice of loop().
void  delayTrigger::idle(void) {
   
   if (!mTriggered && ding()) {
      mTriggered = true;
      doAction();
   }
}



// ************************************************************** 
// A couple exampples of delay trigger classes.
// **************************************************************

class liteOn : public delayTrigger {

   public:
               liteOn(float delayMs);
   virtual     ~liteOn(void);
              
   virtual     void doAction(void);
};


liteOn::liteOn(float delayMs)
   : delayTrigger(delayMs) {  }


liteOn::~liteOn(void) {  }

   
void liteOn::doAction(void) { digitalWrite(13, HIGH); }
   

// ************ 

class liteOff : public delayTrigger {

   public:
               liteOff(float delayMs);
   virtual     ~liteOff(void);
     
   virtual     void doAction(void);
};


liteOff::liteOff(float delayMs)
   : delayTrigger(delayMs) {  }


liteOff::~liteOff(void) {  }


void liteOff::doAction(void) { digitalWrite(13, LOW); }
   


// ************************************************************** 
// And we let the run..
// **************************************************************


liteOn   MrOn(1000);
liteOff  MrOff(2000);


void setup() {
   
   pinMode(13, OUTPUT); // Make LED pin output.
   MrOn.start();        // Start the call to turn on the LED
   MrOff.start();       // Start the call to turn off the LED
}

void loop() {
   idle();                 //Let the idlers have their time in the sun.
}
