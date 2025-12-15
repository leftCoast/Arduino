#include "ourOS.h"
#include <strTools.h>

// **************** ourSOClass ***************

// Our global OS. We create it here.
ourOSClass ourOS;

ourOSClass::ourOSClass(void) {  }


ourOSClass::~ourOSClass(void) {  }


panel* ourOSClass::createPanel(int panelID) {

   beep();
   switch (panelID) {
      case homeApp         : return new ourHome();                // Make the one we created.
      case calcApp         : return new rpnCalc(calcApp);
      case breakoutApp     : return new breakout(breakoutApp);
      default              : return NULL;
   }
}


void ourOSClass::beep(void) { tone(BEEP_PIN, 800,5); }


int ourOSClass::getTonePin(void) { return BEEP_PIN; }

const char*   ourOSClass::getSystemFolder(void) {
  
  heapStr(&pathBuff,SYSTEM_PATH);
  return pathBuff;
}


const char* ourOSClass::getPanelName(int panelID) {

    switch(panelID) {
      case breakoutApp  : return "breakout";
      case calcApp      : return "rpnCalc";
      default           : return NULL;
    }
}

// ***************** ourHome *****************

#define BREAKOUT_X  68
#define BREAKOUT_Y  290
#define RPN_CALC_X  140
#define RPN_CALC_Y  BREAKOUT_Y

ourHome::ourHome(void)
  : homePanel() {
    
    mBackImage = NULL;
  }

ourHome::~ourHome(void) { if (mBackImage) delete(mBackImage); }

void ourHome::setup(void) {
   
   appIcon*  appIconPtr;

   appIconPtr = new appIcon(BREAKOUT_X,BREAKOUT_Y,breakoutApp,iconPath(breakoutApp));
   addObj(appIconPtr);

   appIconPtr = new appIcon(RPN_CALC_X,RPN_CALC_Y,calcApp,iconPath(calcApp));
   addObj(appIconPtr);
}

void ourHome::loop(void) {  }

void ourHome::drawSelf(void) { 
  
  screen->fillRectGradient(0,0,width,height,&blue,&black);
}
