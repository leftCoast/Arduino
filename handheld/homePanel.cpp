#include "homePanel.h"
#include "handheldOS.h"
#include "liveText.h"


// *****************************************************
//                      homeScreen
// *****************************************************


#define X_OFFSET    32 + 8
#define BREAKOUT_X  4
#define BREAKOUT_Y  283
#define GRENADE_X   BREAKOUT_X + X_OFFSET
#define GRENADE_Y   283
#define SETT_X      GRENADE_X + X_OFFSET
#define SETT_Y      283
#define CALC_X      SETT_X + X_OFFSET
#define CALC_Y      283
#define SLIDER_X    CALC_X + X_OFFSET
#define SLIDER_Y    283
#define S_LIST_X    SLIDER_X + X_OFFSET
#define S_LIST_Y    283

//#define SPLASH_SCR_BMP          "/system/images/sunset.bmp"
#define SPLASH_SCR_BMP           "/system/images/lake.bmp"


homeScreen::homeScreen(void)
  : homePanel() {
  mBackImage = NULL;
}

homeScreen::~homeScreen(void) {
  if (mBackImage) delete(mBackImage);
}

void homeScreen::setup(void) {

  rect imageRect;
  colorObj aColor;

  imageRect.setRect(0, 0, 240, 280);
  mBackImage = new bmpObj(&imageRect, SPLASH_SCR_BMP);

  //appIcon*  starTrek = new appIcon(STARTREK_X,STARTREK_Y,starTrekApp,STAR_TREK_PANEL_BMP);
  //addObj(starTrek);


  appIcon*  breakOut = new appIcon(BREAKOUT_X, BREAKOUT_Y, breakoutApp, iconPath(breakoutApp));
  addObj(breakOut);

  appIcon*  grenade = new appIcon(GRENADE_X, GRENADE_Y, grenadeApp, iconPath(grenadeApp));
  addObj(grenade);

  appIcon*  sett = new appIcon(SETT_X, SETT_Y, settApp, iconPath(settApp));
  addObj(sett);

  appIcon*  calc = new appIcon(CALC_X, CALC_Y, calcApp, iconPath(calcApp));
  addObj(calc);

  //appIcon* navTest = new appIcon(NAV_APP_X,NAV_APP_Y,navTestApp,iconPath(navTestApp));
  //addObj(navTest);

  //appIcon* sTerm = new appIcon(NAV_APP_X,NAV_APP_Y,sTermApp,iconPath(sTermApp));
  //addObj(sTerm);

  appIcon* sliderPuzzle = new appIcon(SLIDER_X, SLIDER_Y, sliderApp, iconPath(sliderApp));
  addObj(sliderPuzzle);

  appIcon* shoppingList = new appIcon(S_LIST_X, S_LIST_Y, shopListApp, iconPath(shopListApp));
  addObj(shoppingList);
}


void homeScreen::drawSelf(void) {

  /*
    rect  bBar(0,280,240,40);

    screen->fillRect(&bBar,&black);
    mBackImage->draw();
  */
  screen->fillRectGradient(0, 0, width, height, &blue, &black);
}


void homeScreen::loop(void) { }
