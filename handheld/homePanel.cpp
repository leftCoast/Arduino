#include "homePanel.h"
#include "handheldOS.h"
#include "liveText.h"


// *****************************************************
//                      homeScreen
// *****************************************************

/*
#define STARTREK_X   55
#define STARTREK_Y   140
#define BREAKOUT_X   STARTREK_X - 15
#define BREAKOUT_Y   STARTREK_Y + 40
#define CALC_X       BREAKOUT_X - 15
#define CALC_Y       BREAKOUT_Y + 40
#define TEST_X       BREAKOUT_X + 128
#define TEST_Y       BREAKOUT_Y
#define EDIT_X       BREAKOUT_X + 148
#define EDIT_Y       BREAKOUT_Y+55
*/
#define X_OFFSET    -16
#define BREAKOUT_X  40 + X_OFFSET
#define BREAKOUT_Y  283 
#define GRENADE_X   80 + X_OFFSET
#define GRENADE_Y   283
#define SETT_X      120 + X_OFFSET
#define SETT_Y      283
#define CALC_X      160 + X_OFFSET
#define CALC_Y      283
#define NAV_APP_X   200 + X_OFFSET
#define NAV_APP_Y   283

//#define SPLASH_SCR_BMP          "/system/images/sunset.bmp"
#define SPLASH_SCR_BMP           "/system/images/lake.bmp"
//#define SPLASH_SCR_BMP         "/system/images/starTrek/StTrkScr.bmp"
#define CALC_PANEL_BMP           "/system/appFiles/RPNCalc/calc32.bmp"
#define BREAKOUT_PANEL_BMP       "/system/appFiles/breakout/breakout.bmp"
#define GRENADE_PANEL_BMP        "/system/appFiles/grenade/grenade.bmp"
#define SETT_PANEL_BMP           "/system/appFiles/sett/sett.bmp"
//#define STAR_TREK_PANEL_BMP    "/system/appFiles/starTrek/sTrek32.bmp"
//#define REGION_TEST_PANEL_BMP  "/system/appFiles/rgnTest/test32.bmp"
//#define ICON_EDIT_PANEL_BMP    "/system/appFiles/iconEdit/iconEd32.bmp"
//#define NAV_TEST_PANEL_BMP       "/system/appFiles/navtest/navTst32.bmp"
#define STERM_PANEL_BMP          "/system/appFiles/sTerm/term32.bmp"
#define SLIDER_PANEL_BMP          "/system/appFiles/slider/slider.bmp"

homeScreen::homeScreen(void)
   : homePanel() { }

homeScreen::~homeScreen(void) { }
  
void homeScreen::setup(void) {
   
   rect imageRect;
   colorObj aColor;
   
   imageRect.setRect(0,0,240,280);
   mBackImage = new bmpObj(&imageRect,SPLASH_SCR_BMP);
  
   //appIcon*  starTrek = new appIcon(STARTREK_X,STARTREK_Y,starTrekApp,STAR_TREK_PANEL_BMP);
   //addObj(starTrek);z

   
   appIcon*  breakOut = new appIcon(BREAKOUT_X,BREAKOUT_Y,breakoutApp,BREAKOUT_PANEL_BMP);
   addObj(breakOut);

   appIcon*  grenade = new appIcon(GRENADE_X,GRENADE_Y,grenadeApp,GRENADE_PANEL_BMP);
   addObj(grenade);

   appIcon*  sett = new appIcon(SETT_X,SETT_Y,settApp,SETT_PANEL_BMP);
   addObj(sett);

   appIcon*  calc = new appIcon(CALC_X,CALC_Y,calcApp,CALC_PANEL_BMP);
   addObj(calc);

   //appIcon* navTest = new appIcon(NAV_APP_X,NAV_APP_Y,navTestApp,NAV_TEST_PANEL_BMP);  
   //addObj(navTest);
   
   appIcon* sTerm = new appIcon(NAV_APP_X,NAV_APP_Y,sTermApp,STERM_PANEL_BMP);  
   addObj(sTerm);

   appIcon* sliderPuzzle = new appIcon(NAV_APP_X,NAV_APP_Y,sPuzzleApp,SLIDER_PANEL_BMP);  
   addObj(sliderPuzzle);
   
   /*
   appIcon*  rgnTest = new appIcon(TEST_X,TEST_Y,rgnTestApp,REGION_TEST_PANEL_BMP);
   addObj(rgnTest);

   appIcon*  iconEdit = new appIcon(EDIT_X,EDIT_Y,iconEditApp,ICON_EDIT_PANEL_BMP);
   addObj(iconEdit);

   
   
   liveText*   header = new liveText(0,7,240,8,100,true);
   header->setValue("Left Coast classic code machine");
   header->setTextSize(1);
   header->setJustify(TEXT_CENTER);
   header->setColors(&yellow);
   
   aColor.setColor(&yellow);
   aColor.blend(&black,75);
   header->addAColor(0,&aColor);
   header->addAColor(2000,&aColor);
   
   aColor.setColor(&yellow);
   aColor.blend(&white,60);
   header->addAColor(1000,&aColor);
   
   addObj(header);
   */
}


void homeScreen::drawSelf(void) { 
   
   rect  bBar(0,280,240,40);
   
   screen->fillRect(&bBar,&black);
   mBackImage->draw();
  } 


void homeScreen::loop(void) { }
