#include "calcButton.h"
#include <calculator.h>

#define TEXT_SIZE     2
#define CHAR_WIDTH    6
#define BUTTON_HEIGHT 18
#define RADIUS        3

extern calculator mCalc;
extern boolean    buttonPressed;

colorObj numberActiveBColor(WHITE);
colorObj numberActiveTColor(BLACK);
colorObj numberClickedBColor(BLACK);
colorObj numberClickedTColor(WHITE);

colorObj editActiveBColor(BLUE);
colorObj editActiveTColor(WHITE);
colorObj editClickedBColor(WHITE);
colorObj editClickedTColor(BLACK);

colorObj fxActiveBColor(BLACK);
colorObj fxActiveTColor(WHITE);
colorObj fxClickedBColor(WHITE);
colorObj fxClickedTColor(BLACK);


void setupButtonColors(void) {

  numberActiveBColor.blend(&black, 40);
  numberActiveBColor.blend(&yellow, 25);
  //numberClickedBColor(&white, 100);
  numberActiveTColor.blend(&black, 60);
  //numberClickedTColor.blend(&red, 80);

  editActiveBColor.blend(&white, 40);
  //editClickedBColor(&white, 100);
  //editActiveTColor.blend(&black, 100);
  //editClickedTColor.blend(&red, 80);

  fxActiveBColor.blend(&white, 20);
  //fxActiveTColor.blend();
  //fxClickedBColor.blend();
  //fxClickedTColor.blend();
}


calcButton::calcButton(char* inFStr, word inLocX, word inLocY, byte width, byte inType)
  : drawObj(inLocX, inLocY, width, BUTTON_HEIGHT, true) {

  int numChars = strlen(inFStr) + 1;
  fStr = (char*) malloc(numChars);
  strcpy(fStr, inFStr);
  bType = inType;
  //aFStr = NULL;
  //invert = false;
}

/*
calcButton::calcButton(char* inFStr, char* inAFStr, word inLocX, word inLocY, byte width, byte inType)
  : drawObj(inLocX, inLocY, width, BUTTON_HEIGHT, true) {

  int numChars = strlen(inFStr) + 1;
  fStr = (char*) malloc(numChars);
  strcpy(fStr, inFStr);

  numChars = strlen(inAFStr) + 1;
  aFStr = (char*) malloc(numChars);
  strcpy(aFStr, inAFStr);
  
  bType = inType;
  invert = false;
}


void calcButton::setInvert(void) {
  invert = true;
  needRefresh = true;
}


void calcButton::clearInvert(void) {
  invert = false;
  needRefresh = true;
}
*/


void calcButton::drawSelf(void) {

  colorObj* activeBaseColor;
  colorObj* touchedBaseColor;
  colorObj* activeTextColor;
  colorObj* touchedTextColor;

  switch (bType) {
    case NUMBER_BTN :
      activeBaseColor = &numberActiveBColor;
      touchedBaseColor = &numberClickedBColor;
      activeTextColor = &numberActiveTColor;
      touchedTextColor = &numberClickedTColor;
      break;
    case  EDIT_BTN :
      activeBaseColor = &editActiveBColor;
      touchedBaseColor = &editClickedBColor;
      activeTextColor = &editActiveTColor;
      touchedTextColor = &editClickedTColor;
      break;
    case FX_BTN :
      activeBaseColor = &fxActiveBColor;
      touchedBaseColor = &fxClickedBColor;
      activeTextColor = &fxActiveTColor;
      touchedTextColor = &fxClickedTColor;
      break;
  }

  if (clicked) {
    screen->fillRoundRect(locX, locY, width, height, RADIUS, touchedBaseColor);
    screen->drawRoundRect(locX-1, locY-1, width+2, height+2, RADIUS+1, &white);
    screen->drawRoundRect(locX-2, locY-2, width+4, height+4, RADIUS+2, &white);
    screen->setTextColor(touchedTextColor, touchedBaseColor);
  } else {
    screen->fillRoundRect(locX-2, locY-2, width+4, height+4, RADIUS+2, &black); 
    screen->fillRoundRect(locX, locY, width, height, RADIUS, activeBaseColor);
    screen->setTextColor(activeTextColor, activeBaseColor);
  }
  screen->setTextSize(TEXT_SIZE);
  screen->setTextWrap(false);
  word dispWidth = width - (2 * RADIUS);
  word textWidth = (CHAR_WIDTH * TEXT_SIZE * strlen(fStr)) - 1;
  if (dispWidth > textWidth) {
    screen->setCursor(locX + RADIUS + ((dispWidth - textWidth) / 2), locY + 1);
    screen->drawText(fStr);
  }
}


/*
void calcButton::doAction(void) {

  if (invert) {
    mCalc.buttonClick(aFStr);    // Send our fuction label to the calculator object to parse.
  } else {
    mCalc.buttonClick(fStr);    // Send our fuction label to the calculator object to parse.
  }
  buttonPressed = true;       // Tell the calling program we changed something.
}
*/

void calcButton::doAction(void) {

  mCalc.buttonClick(fStr);    // Send our fuction label to the calculator object to parse.
  buttonPressed = true;       // Tell the calling program we changed something.
}
