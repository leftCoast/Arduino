#include "homeScr.h"
#include <lilOS.h>

#define out(x) Serial.print(x)
#define outln(x) Serial.println(x)

#define BAR_Y        286


struct spacer {
   float startPos;
   float stepSize;
};

//
spacer calcSpacer(float numItems, float itemLength, float areaLength) {
   
   spacer   aSpacer;
   
   numItems++;
   aSpacer.stepSize = areaLength/numItems;
   aSpacer.startPos = aSpacer.stepSize - itemLength/2;
   return aSpacer;
}


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
   int      stepX;
   appIcon* theAppIcon;
   spacer   barSpacer;

   barSpacer   = calcSpacer(4,32,240);                                                          // Calculate bar spacing. Num apps, icon width, bar width.
   traceX      = barSpacer.startPos;                                                            // Set initial x position.
   stepX       = barSpacer.stepSize;                                                            // Set our step size.
   traceY      = BAR_Y;                                                                         // Set our y location.
   
   theAppIcon = new appIcon(traceX,traceY,calcApp,iconPath(calcApp,"calc32.bmp"));              // Create icon for the calculator.
   theAppIcon->setMask(&(mOSPtr->icon32Mask));                                                  // Add the mask to the icon.
   addObj(theAppIcon);                                                                          // Send the icon to our drawObj list to be displayed.
   
   traceX = traceX + stepX;
   theAppIcon = new appIcon(traceX,traceY,breakoutApp,iconPath(breakoutApp,"breakout.bmp"));    // Create icon for the breakout game.
   theAppIcon->setMask(&(mOSPtr->icon32Mask));                                                  // Add the mask to the icon. (Its the same mask over and over.)
   addObj(theAppIcon);                                                                          // Send the icon to our drawObj list to be displayed.
                                                                                                
   traceX = traceX + stepX;
   theAppIcon = new appIcon(traceX,traceY,iconEditApp,iconPath(iconEditApp,"iconEdit.bmp"));    // Create icon for the icon editor disaster.
   theAppIcon->setMask(&(mOSPtr->icon32Mask));                                                  // Add the mask.
   addObj(theAppIcon);                                                                          // And its off to the races.

   traceX = traceX + stepX;
   theAppIcon = new appIcon(traceX,traceY,starTrekApp,iconPath(starTrekApp,"sTrek32.bmp"));     // Create icon for the Star Trek game.
   theAppIcon->setMask(&(mOSPtr->icon32Mask));                                                  // Mask.
   addObj(theAppIcon);                                                                          // Drop it into the list. 
}


void homeScr::loop(void) { }


void homeScr::drawSelf(void) { 

   colorObj lineColor;
   colorObj scrFadeColor;
   //bmpObj   theScreenImage(0,0,240,282,"/system/images/lake.bmp");

   ourOS.setBrightness(0);
   lineColor.setColor(LC_CHARCOAL);
   screen->fillRectGradient(0,282,240,38,&lineColor,&black);
   scrFadeColor.setColor(LC_LIGHT_BLUE);
   scrFadeColor.blend(&blue,50);
   screen->fillRectGradient(0,0,240,282,&black,&scrFadeColor);
   //theScreenImage.draw();
   ourOS.setBrightness(255);
}
