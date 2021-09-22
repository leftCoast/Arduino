#include "homeScr.h"
#include <lilOS.h>


#define TOP_Y     285
#define STEP_Y    52
#define LEFT_X    52
#define STEP_X    52

char* panelIconPath = "/system/icons/";


homeScr::homeScr(void)
   : panel(&ourOS,homeApp,noMenuBar) { }

   
homeScr::~homeScr(void) {  }


char* homeScr::panelPath(char* panelFolder) {

   strcpy(pathBuff,panelIconPath);
   strcat(pathBuff,panelFolder);
   return pathBuff; 
}


void homeScr::setup(void) {
   
   int      traceY;
   int      traceX;
   appIcon* theAppIcon;
   
   traceX = LEFT_X;
   traceY = TOP_Y;
  
   theAppIcon = new appIcon(traceX,traceY,calcApp,panelPath("RPNCalc/calc32.bmp"));
   Serial.println(pathBuff);
   theAppIcon->begin();
   addObj(theAppIcon); 
}


void homeScr::loop(void) { }


void homeScr::drawSelf(void) { screen->fillScreen(&black); } 
