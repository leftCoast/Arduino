#include "labelButton.h"

labelButton::labelButton(char* inFStr,word inLocX, word inLocY,byte width,byte height) {

  
}


labelButton::~labelButton(void) {

            
}

                  
void labelButton::idle(void) {

  
}


void labelButton::drawSelf(void) {

  colorObj activeBaseColor(;
  colorObj touchedBaseColor;
  colorObj activeTextColor;
  colorObj touchedTextColor;
  
  hookup();                   // Need to do a hookup, this is a good time.
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
      if (secondFx && aFStr ) {                 // We sportin' a second fx? Let's show it child!
        activeTextColor = &editActiveBColor;
      } else {
        activeTextColor = &fxActiveTColor;
      }
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
  if (secondFx && aFStr ) {
    word textWidth = (CHAR_WIDTH * TEXT_SIZE * strlen(aFStr)) - 1;
    if (dispWidth > textWidth) {
      screen->setCursor(locX + RADIUS + ((dispWidth - textWidth) / 2), locY + 1);
      screen->drawText(aFStr);
    }
  } else {
    word textWidth = (CHAR_WIDTH * TEXT_SIZE * strlen(fStr)) - 1;
    if (dispWidth > textWidth) {
      screen->setCursor(locX + RADIUS + ((dispWidth - textWidth) / 2), locY + 1);
      screen->drawText(fStr);
    }
  }
}
}


void labelButton::doAction(void) {

  
}
