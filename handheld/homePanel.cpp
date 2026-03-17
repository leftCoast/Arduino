#include "homePanel.h"
#include "handheldOS.h"
#include <liveText.h>
#include <rectArrange.h>

/*
#define X_OFFSET    32 + 8
#define BREAKOUT_X  4
#define BREAKOUT_Y  445
#define GRENADE_X   BREAKOUT_X + X_OFFSET
#define GRENADE_Y   445 //283
#define SETT_X      GRENADE_X + X_OFFSET
#define SETT_Y      445 //283
#define CALC_X      SETT_X + X_OFFSET
#define CALC_Y      445 //283
#define SLIDER_X    CALC_X + X_OFFSET
#define SLIDER_Y    445 //283
#define S_LIST_X    SLIDER_X + X_OFFSET
#define S_LIST_Y    445 //283
*/
#define APP_ICON_H	40
#define APP_ICON_Y	4

// *****************************************************
//                      iconArrange
// *****************************************************

	
iconArrange::iconArrange(void)
  : rectArrange() {  }


iconArrange::~iconArrange(void) { }


void iconArrange::arrangeList(void) {

	int				xLoc;
	int				space;
	rectListObj*	trace;

	if (minWidth()<=areaRect.width) {							// If we can make it fit.
		if (maxWidth()<=areaRect.width) {						// No matter, it'll fit..
			xLoc = areaRect.width - maxWidth();					// Streach 'em out.
			space = maxWSpace;										// Choose max space.
		} else {															// Too many?
			xLoc = areaRect.width - minWidth();					// Shrink 'em up.
			space = minWSpace;										// Choose narrow.
		}																	//
		xLoc = (xLoc + areaRect.x)/2;								// Don't forget the offset..
		trace = (rectListObj*)getFirst();						// Grab the first one on the list.
		while(trace) {													// For ever rect we can find..
			trace->ourRect->x = xLoc;								// Set this rect's x location.
			trace->ourRect->y = areaRect.y + APP_ICON_Y;		// Set this rect's y location.
			xLoc = xLoc + trace->ourRect->width + space;		// Calcualte the next rect's location.
			trace = (rectListObj*)trace->getNext();			// Hop to the next rect on the list.
		}																	//
	}																		// If they don't fit? Leave 'em be.
}



// *****************************************************
//                      homeScreen
// *****************************************************


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

	rect 			imageRect;
	rect			iconBar;
	colorObj 	aColor;
	iconArrange	spreader;
	int			defX;
	int			defY;

	iconBar.x = 0;
	iconBar.y = screen->height() - APP_ICON_H;
	iconBar.width = screen->width();
	iconBar.height = APP_ICON_H;
	spreader.settings(&iconBar,10,20);

	imageRect.setRect(0, 0, 240, 280);
	mBackImage = new bmpObj(&imageRect, SPLASH_SCR_BMP);

  //appIcon*  starTrek = new appIcon(STARTREK_X,STARTREK_Y,starTrekApp,STAR_TREK_PANEL_BMP);
  //addObj(starTrek);

	defX = 0;
	defY = 0;
 	appIcon*  breakOut = new appIcon(defX++, defY++, breakoutApp, iconPath(breakoutApp));
 	addObj(breakOut);
	spreader.addRect(breakOut);

  //appIcon*  grenade = new appIcon(GRENADE_X, GRENADE_Y, grenadeApp, iconPath(grenadeApp));
  //addObj(grenade);

	appIcon*  sett = new appIcon(defX++, defY++, settApp, iconPath(settApp));
	addObj(sett);
	spreader.addRect(sett);

	appIcon*  calc = new appIcon(defX++, defY++, calcApp, iconPath(calcApp));
	addObj(calc);
	spreader.addRect(calc);

  //appIcon* navTest = new appIcon(NAV_APP_X,NAV_APP_Y,navTestApp,iconPath(navTestApp));
  //addObj(navTest);

  //appIcon* sTerm = new appIcon(NAV_APP_X,NAV_APP_Y,sTermApp,iconPath(sTermApp));
  //addObj(sTerm);

	appIcon* sliderPuzzle = new appIcon(defX++, defY++, sliderApp, iconPath(sliderApp));
	addObj(sliderPuzzle);
	spreader.addRect(sliderPuzzle);

	appIcon* shoppingList = new appIcon(defX++, defY++, shopListApp, iconPath(shopListApp));
	addObj(shoppingList);
	spreader.addRect(shoppingList);
}


void homeScreen::drawSelf(void) {

  /*
    rect  bBar(0,280,240,40);

    screen->fillRect(&bBar,&black);
    mBackImage->draw();
  */
	rect	iconBar;
	colorObj	startColor;
	colorObj	endColor;

	iconBar.x = 0;
	iconBar.y = screen->height() - APP_ICON_H;
	iconBar.width = screen->width();
	iconBar.height = APP_ICON_H;

	startColor.setColor(&blue);
	endColor.setColor(&black);
	endColor.blend(&blue,5);
	screen->fillRectGradient(0, 0, width, height-APP_ICON_H, &startColor, &endColor);
	screen->fillRect(&iconBar,&black);
}


void homeScreen::loop(void) { }
