#include "homeScr.h"
#include <lilOS.h>


#define TOP_Y     286
#define STEP_Y    52
#define LEFT_X    52
#define STEP_X    52


homeScr::homeScr(void)
   : panel(&ourOS,homeApp,noMenuBar) { }

   
homeScr::~homeScr(void) {  }


char* homeScr::iconPath(int appID,char* iconName) {

   strcpy(pathBuff,mOSPtr->getPanelFolder(appID));
   strcat(pathBuff,iconName);
   return pathBuff; 
}


void homeScr::setup(void) {
   
   int      traceY;
   int      traceX;
   appIcon* theAppIcon;
   
   traceX = LEFT_X;
   traceY = TOP_Y;

   theAppIcon = new appIcon(traceX,traceY,calcApp,iconPath(calcApp,"calc32.bmp"));
   theAppIcon->setMask(&iconMask);
   theAppIcon->begin();
   addObj(theAppIcon); 
  
   traceX = traceX + STEP_X;
   theAppIcon = new appIcon(traceX,traceY,breakoutApp,iconPath(breakoutApp,"breakout.bmp"));
   theAppIcon->setMask(&iconMask);
   theAppIcon->begin();
   addObj(theAppIcon);
}


void homeScr::loop(void) { }


void homeScr::drawSelf(void) { 

   colorObj lineColor;
   bmpObj   theScreenImage(0,0,240,282,"/system/images/lake.bmp");

   ourOS.setBrightness(0);
   lineColor.setColor(LC_CHARCOAL);
   screen->fillRectGradient(0,282,240,38,&lineColor,&black);
   theScreenImage.begin();
   theScreenImage.draw();
   ourOS.setBrightness(255);
}
