#include "calcButton.h"
#include <calculator.h>

#define TEXT_SIZE     2
#define CHAR_WIDTH    6
#define BUTTON_HEIGHT 18
#define RADIUS        3

extern calculator mCalc;
extern boolean    buttonPressed;

calcButton::calcButton(char* inFStr, word inLocX, word inLocY, byte width)
  : drawObj(inLocX, inLocY, width, BUTTON_HEIGHT, true) {

  int numChars = strlen(inFStr) + 1;
  fStr = (char*) malloc(numChars);
  strcpy(fStr, inFStr);
}

void calcButton::drawSelf(void) {

  colorObj activeColor(blue);
  activeColor.blend(&black, 40);
  activeColor.blend(&red, 10);
  colorObj touchedColor(white);

  if (clicked) {
    screen->fillRoundRect(locX, locY, width, height, RADIUS, touchedColor.getColor16());
    screen->setTextColor(BLACK, touchedColor.getColor16());
  } else {
    screen->fillRoundRect(locX, locY, width, height, RADIUS, activeColor.getColor16());
    screen->setTextColor(BLACK, activeColor.getColor16());
  }
  screen->setTextSize(TEXT_SIZE);
  screen->setTextWrap(false);
  word dispWidth = width - (2 * RADIUS);
  word textWidth = CHAR_WIDTH * TEXT_SIZE;
  if (dispWidth > textWidth) {
    screen->setCursor(locX + ((dispWidth - textWidth) / 2), locY + 1);
    screen->print(fStr);
  }
}


void calcButton::doAction(void) {

  mCalc.buttonClick(fStr);    // Send our fuction label to the calculator object to parse.
  buttonPressed = true;       // Tell the calling program we changed something.
}

