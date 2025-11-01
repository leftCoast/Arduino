#include "calcButton.h"
#include <calculator.h>
#include <lilOS.h>

#define TEXT_SIZE     2
#define CHAR_WIDTH    6
#define BUTTON_HEIGHT 18
#define RADIUS        3



// ****************** calcButton ******************


calcButton::calcButton(const char* inFStr, word inLocX, word iny, byte width, byte inType,rpnCalc* inCalc)
  : drawObj(inLocX, iny, width, BUTTON_HEIGHT, touchLift) {

	
	int numChars = strlen(inFStr) + 1;
	fStr = (char*) malloc(numChars);
	strcpy(fStr, inFStr);
	
	ourCalc		= inCalc;
	bType			= inType;
	aFStr			= NULL;
	saved2ndFx	= false;
}


calcButton::calcButton(const char* inFStr,const char* inAFStr, word inLocX, word iny, byte width, byte inType,rpnCalc* inCalc)
  : drawObj(inLocX, iny, width, BUTTON_HEIGHT, touchLift) {

	int numChars = strlen(inFStr) + 1;
	fStr = (char*) malloc(numChars);
	strcpy(fStr, inFStr);

	numChars = strlen(inAFStr) + 1;
	aFStr = (char*) malloc(numChars);
	strcpy(aFStr, inAFStr);
  
	ourCalc		= inCalc;
	bType			= inType;
	saved2ndFx	= false;
}


calcButton::~calcButton(void) {

  if (fStr) free(fStr);
  if (aFStr) free(aFStr);
}


void calcButton::idle(void) {

  if (aFStr && saved2ndFx!=ourCalc->secondFx) {
    saved2ndFx = ourCalc->secondFx;
    needRefresh = true;
  }
}


void calcButton::drawSelf(void) {

  colorObj* activeBaseColor	= NULL;	// Set to NULL to shut up compiler.
  colorObj* touchedBaseColor	= NULL;
  colorObj* activeTextColor	= NULL;
  colorObj* touchedTextColor	= NULL;
  
	hookup();                   			// Need a hookup? This is a good time.
	switch (bType) {
		case NUMBER_BTN :
			activeBaseColor = &ourCalc->numberActiveBColor;
			touchedBaseColor = &ourCalc->numberClickedBColor;
			activeTextColor = &ourCalc->numberActiveTColor;
			touchedTextColor = &ourCalc->numberClickedTColor;
		break;
		case  EDIT_BTN :
			activeBaseColor = &ourCalc->editActiveBColor;
			touchedBaseColor = &ourCalc->editClickedBColor;
			activeTextColor = &ourCalc->editActiveTColor;
			touchedTextColor = &ourCalc->editClickedTColor;
      break;
		case FX_BTN :
			activeBaseColor = &ourCalc->fxActiveBColor;
			touchedBaseColor = &ourCalc->fxClickedBColor;
			if (ourCalc->secondFx && aFStr ) {			// We sportin' a second fx? Let's show it child!
				activeTextColor = &ourCalc->editActiveBColor;
			} else {
				activeTextColor = &ourCalc->fxActiveTColor;
			}
			touchedTextColor = &ourCalc->fxClickedTColor;
		break;
	}
	if (clicked) {
		screen->fillRoundRect(x, y, width, height, RADIUS, touchedBaseColor);
		screen->drawRoundRect(x-1, y-1, width+2, height+2, RADIUS+1, &white);
		screen->drawRoundRect(x-2, y-2, width+4, height+4, RADIUS+2, &white);
		screen->setTextColor(touchedTextColor, touchedBaseColor);
	} else {
		screen->fillRoundRect(x-2, y-2, width+4, height+4, RADIUS+2, &black); 
		screen->fillRoundRect(x, y, width, height, RADIUS, activeBaseColor);
		screen->setTextColor(activeTextColor, activeBaseColor);
	}
	screen->setTextSize(TEXT_SIZE);
	screen->setTextWrap(false);
	word dispWidth = width - (2 * RADIUS);
	if (ourCalc->secondFx && aFStr ) {
		word textWidth = (CHAR_WIDTH * TEXT_SIZE * strlen(aFStr)) - 1;
		if (dispWidth > textWidth) {
			screen->setCursor(x + RADIUS + ((dispWidth - textWidth) / 2), y + 1);
			screen->drawText(aFStr);
		}
	} else {
		word textWidth = (CHAR_WIDTH * TEXT_SIZE * strlen(fStr)) - 1;
		if (dispWidth > textWidth) {
			screen->setCursor(x + RADIUS + ((dispWidth - textWidth) / 2), y + 1);
			screen->drawText(fStr);
		}
  	}
}


void calcButton::doAction(void) {

	OSPtr->beep();								// Amuse the user.
	if (ourCalc->secondFx && aFStr ) {	// If it's second function time, AND we have a second function..
		ourCalc->buttonClick(aFStr);		// Send our 2nd function label to the calc object to parse.
	} else {										// Else it's not 2nd function time or we don't have one..
		ourCalc->buttonClick(fStr);		// Send our default function label to the calc object to parse.
	}												//
	ourCalc->secondFx		= false;			// First keystroke clears it.
	ourCalc->btnPressed	= true;			// Tell the calling program we changed something.
}



// ****************** secondfxButton ******************

  
secondfxButton::secondfxButton(const char* inFStr,word inLocX, word iny,byte width,byte inType,rpnCalc* inCalc)
  : calcButton(inFStr,inLocX,iny,width,inType,inCalc) { }


secondfxButton::~secondfxButton(void) {  }


void secondfxButton::doAction(void) {
  
  OSPtr->beep();
  ourCalc->toggle2ndFx();
}



// ****************** degRadButton ******************


degRadButton::degRadButton(word inLocX, word inY,byte width,byte height,rpnCalc* inCalc)
    : calcButton("-------",inLocX,inY,width,DEG_RAD_BTN,inCalc) { setSize(width,height); } 


degRadButton::~degRadButton(void) {  }


void  degRadButton::setColors(colorObj* inTColor,colorObj* inHTColor,colorObj* inBColor) {

    tColor.setColor(inTColor);
    hColor.setColor(inHTColor);
    bColor.setColor(inBColor);
    needRefresh = true;
  }


void  degRadButton::drawSelf(void) {

    screen->setTextSize(1);
    screen->setTextWrap(false);
    if (clicked) {
      screen->setTextColor(&hColor, &bColor);
    } else {
      screen->setTextColor(&tColor, &bColor);
    }
    screen->setCursor(x + 2, y + 1);
    
    if (ourCalc->getDegrees()) {
      screen->drawText("Degrees");
    } else {
      screen->drawText("Radians");
    }
  }

  
 void  degRadButton::doAction(void) {

    OSPtr->beep();
    ourCalc->toggleDegrees();
    needRefresh = true;
 }

