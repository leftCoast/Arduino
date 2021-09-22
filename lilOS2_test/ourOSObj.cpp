#include "ourOSObj.h"
#include <rpnCalc.h>
#include "homeScr.h"

#define BEEP_PIN    23

// Our OS object.
ourOSObj  ourOS;


ourOSObj::ourOSObj(void)
   : lilOS() {
   
}


ourOSObj::~ourOSObj(void) {  }


// The hardware is online, do hookups.
int ourOSObj::begin(void) {
   
   pinMode(BEEP_PIN, OUTPUT);       // Setup The beeper pin.
   digitalWrite(BEEP_PIN, HIGH);    //Means off.
   return lilOS::begin();          // Return result of the inherited
}


// We need to write our own panel creation method.
panel* ourOSObj::createPanel(int panelID) {
   
   beep();
   switch (panelID) {
      case homeApp      : return new homeScr();
      case calcApp      : return new rpnCalc(this,panelID);
      //case sTermApp     : return new sTermPanel();
      //case breakoutApp  : return new breakout();
      default           : return NULL;
   }
}


// Only WE know how to make it beep.
void ourOSObj::beep(void) { tone(BEEP_PIN, 750,20); }            
