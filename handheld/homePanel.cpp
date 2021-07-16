#include "homePanel.h"
#include "handheldOS.h"
#include "liveText.h"


// *****************************************************
//                      homeScreen
// *****************************************************


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
#define SPLASH_SCR_BMP        "/system/images/starTrek/StTrkScr.bmp"
#define CALC_PANEL_BMP        "/system/icons/RPNCalc/calc32.bmp"
#define BREAKOUT_PANEL_BMP    "/system/icons/breakout/breakout.bmp"
#define STAR_TREK_PANEL_BMP   "/system/icons/starTrek/sTrek32.bmp"
#define REGION_TEST_PANEL_BMP	"/system/icons/rgnTest/test32.bmp"
#define ICON_EDIT_PANEL_BMP   "/system/icons/iconEdit/iconEd32.bmp"

homeScreen::homeScreen(void)
   : homePanel() { }

homeScreen::~homeScreen(void) { }
  
void homeScreen::setup(void) {
   
   rect imageRect;
   colorObj aColor;
   
   imageRect.setRect(0,0,240,300);
   mBackImage = new bmpPipe(&imageRect);
   mBackImage->openPipe(SPLASH_SCR_BMP);

   appIcon*  starTrek = new appIcon(STARTREK_X,STARTREK_Y,starTrekApp,STAR_TREK_PANEL_BMP);
   starTrek->begin();
   addObj(starTrek);
   
   appIcon*  breakOut = new appIcon(BREAKOUT_X,BREAKOUT_Y,breakoutApp,BREAKOUT_PANEL_BMP);
   breakOut->begin();
   addObj(breakOut);
   
   appIcon*  rgnTest = new appIcon(TEST_X,TEST_Y,rgnTestApp,REGION_TEST_PANEL_BMP);
   rgnTest->begin();
   addObj(rgnTest);

   appIcon*  iconEdit = new appIcon(EDIT_X,EDIT_Y,iconEditApp,ICON_EDIT_PANEL_BMP);
   iconEdit->begin();
   addObj(iconEdit);

   appIcon*  calc = new appIcon(CALC_X,CALC_Y,calcApp,CALC_PANEL_BMP);
   calc->begin();
   addObj(calc);

   liveText*   header = new liveText(0,7,240,8,50,true);
   header->setValue("Left Coast classic code machine");
   header->setTextSize(1);
   header->setColors(&yellow);
   header->setJustify(TEXT_CENTER);
   aColor.setColor(&yellow);
   aColor.blend(&black,75);
   header->addAColor(0,&aColor);
   header->addAColor(1500,&aColor);
   aColor.setColor(&yellow);
   aColor.blend(&white,50);
   header->addAColor(750,&aColor);
   addObj(header);
}


void homeScreen::drawSelf(void) { mBackImage->drawImage(0,MENU_BAR_H); } 


void homeScreen::loop(void) { }
