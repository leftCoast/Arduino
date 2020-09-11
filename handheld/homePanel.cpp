#include "homePanel.h"
#include "handheldOS.h"
#include "label.h"

// *****************************************************
//                      homeScreen
// *****************************************************

#define STARTREK_X   55
#define STARTREK_Y   140
#define BREAKOUT_X   STARTREK_X - 15
#define BREAKOUT_Y   STARTREK_Y + 40
#define CALC_X       BREAKOUT_X - 15
#define CALC_Y       BREAKOUT_Y + 40

#define SPLASH_SCR_BMP        "/system/images/starTrek/StTrkScr.bmp"
#define CALC_PANEL_BMP        "/system/icons/RPNCalc/calc32.bmp"
#define BREAKOUT_PANEL_BMP    "/system/icons/breakout/breakout.bmp"
#define STAR_TREK_PANEL_BMP   "/system/icons/starTrek/sTrek32.bmp"

homeScreen::homeScreen(void)
   : homePanel() { }

homeScreen::~homeScreen(void) { }
  
void homeScreen::setup(void) {
   
   rect imageRect;
   
   imageRect.setRect(0,0,240,300);
   mBackImage = new bmpPipe(&imageRect);
   mBackImage->openPipe(SPLASH_SCR_BMP);

   appIcon*  starTrek = new appIcon(STARTREK_X,STARTREK_Y,starTrekApp,STAR_TREK_PANEL_BMP);
   starTrek->begin();
   addObj(starTrek);
   
   appIcon*  breakOut = new appIcon(BREAKOUT_X,BREAKOUT_Y,breakoutApp,BREAKOUT_PANEL_BMP);
   breakOut->begin();
   addObj(breakOut);

   appIcon*  calc = new appIcon(CALC_X,CALC_Y,calcApp,CALC_PANEL_BMP);
   calc->begin();
   addObj(calc);

   label*   header = new label(0,6,240,8,"Left Coast classic code machine",1);
   header->setColors(&yellow);
   header->setJustify(TEXT_CENTER);
   addObj(header);
}


void homeScreen::drawSelf(void) { mBackImage->drawImage(0,MENU_BAR_H); } 


void homeScreen::loop(void) { }
