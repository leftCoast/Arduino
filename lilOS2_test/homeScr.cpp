#include "homeScr.h"
#include <lilOS.h>


#define TOP_Y     285
#define STEP_Y    52
#define LEFT_X    52
#define STEP_X    52


homeScr::homeScr(void)
   : panel(&ourOS,homeApp,noMenuBar) { }

   
homeScr::~homeScr(void) {  }


char* homeScr::iconPath(int appID,char* iconName) {

   strcpy(pathBuff,mOSPtr->getPanelFolder(appID));
   strcat(pathBuff,iconName);
   Serial.println(pathBuff);
   return pathBuff; 
}


void homeScr::setup(void) {
   
   int      traceY;
   int      traceX;
   appIcon* theAppIcon;
   
   traceX = LEFT_X;
   traceY = TOP_Y;
   Serial.println("doing the calcApp thing");
   Serial.flush();
   theAppIcon = new appIcon(traceX,traceY,calcApp,iconPath(calcApp,"calc32.bmp"));
   Serial.println("Calling begin()");
   theAppIcon->begin();
   Serial.println("Calling addObj()");
   addObj(theAppIcon); 
   Serial.println("doing the breakoutApp thing");
   Serial.flush();
   traceX = traceX + STEP_X;
   theAppIcon = new appIcon(traceX,traceY,breakoutApp,iconPath(breakoutApp,"breakout.bmp"));
   theAppIcon->begin();
   addObj(theAppIcon);
   Serial.println("Done them things..");
    Serial.flush();
}


void homeScr::loop(void) { }


void homeScr::drawSelf(void) { screen->fillScreen(&black); } 
