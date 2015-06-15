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
colorObj numberClickedBColor(WHITE);
colorObj numberClickedTColor(BLACK);

colorObj editActiveBColor(BLUE);
colorObj editActiveTColor(BLACK);
colorObj editClickedBColor(BLUE);
colorObj editClickedTColor(WHITE);

colorObj fxActiveBColor(BLACK);
colorObj fxActiveTColor(WHITE);
colorObj fxClickedBColor(WHITE);
colorObj fxClickedTColor(BLACK);


void setupButtonColors(void) {

  numberActiveBColor = numberActiveBColor.blend(&black, 40);
  numberActiveBColor = numberActiveBColor.blend(&yellow, 25);
  //numberClickedBColor = numberClickedBColor(&white, 100);
  //numberActiveTColor = numberActiveTColor.blend(&black, 100);
  numberClickedTColor = numberClickedTColor.blend(&red, 80);

  editActiveBColor = editActiveBColor.blend(&white, 20);
  //editClickedBColor = editClickedBColor(&white, 100);
  //editActiveTColor = editActiveTColor.blend(&black, 100);
  editClickedTColor = editClickedTColor.blend(&red, 80);

  fxActiveBColor = fxActiveBColor.blend(&white, 20);
  //fxActiveTColor = fxActiveTColor.blend();
  //fxClickedBColor = fxClickedBColor.blend();
  //fxClickedTColor = fxClickedTColor.blend();
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
    screen->fillRoundRect(locX, locY, width, height, RADIUS, touchedBaseColor->getColor16());
    screen->setTextColor(touchedTextColor->getColor16(), touchedBaseColor->getColor16());
  } else {
    screen->fillRoundRect(locX, locY, width, height, RADIUS, activeBaseColor->getColor16());
    screen->setTextColor(activeTextColor->getColor16(), activeBaseColor->getColor16());
  }
  screen->setTextSize(TEXT_SIZE);
  screen->setTextWrap(false);
  word dispWidth = width - (2 * RADIUS);
  word textWidth = (CHAR_WIDTH * TEXT_SIZE * strlen(fStr)) - 1;
  if (dispWidth > textWidth) {
    screen->setCursor(locX + RADIUS + ((dispWidth - textWidth) / 2), locY + 1);
    screen->print(fStr);
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
