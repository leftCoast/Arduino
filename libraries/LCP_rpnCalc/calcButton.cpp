#include "calcButton.h"
#include <calculator.h>
#include <lilOS.h>

#include "rpnCalc.h"

#define TEXT_SIZE     2
#define CHAR_WIDTH    6
#define BUTTON_HEIGHT 18
#define RADIUS        3

extern calculator mCalc;
extern bool       buttonPressed;
       bool       gSecondFx = false;
extern int			nextPanel;

colorObj numberActiveBColor;
colorObj numberActiveTColor;
colorObj numberClickedBColor;
colorObj numberClickedTColor;

colorObj editActiveBColor;
colorObj editActiveTColor;
colorObj editClickedBColor;
colorObj editClickedTColor;

colorObj fxActiveBColor(LC_BLACK);
colorObj fxActiveTColor;
colorObj fxClickedBColor;
colorObj fxClickedTColor;


void setupButtonColors(void) {

  numberActiveBColor.setColor(LC_WHITE);
  numberActiveTColor.setColor(LC_BLACK);
  numberClickedBColor.setColor(LC_BLACK);
  numberClickedTColor.setColor(LC_WHITE);
  
  editActiveBColor.setColor(LC_BLUE);
  editActiveTColor.setColor(LC_WHITE);
  editClickedBColor.setColor(LC_WHITE);
  editClickedTColor.setColor(LC_BLACK);
  
  fxActiveBColor.setColor(LC_BLACK);
  fxActiveTColor.setColor(LC_WHITE);
  fxClickedBColor.setColor(LC_WHITE);
  fxClickedTColor.setColor(LC_BLACK);

  numberActiveBColor.blend(&black, 40);
  numberActiveBColor.blend(&yellow, 25);
  numberActiveTColor.blend(&black, 60);
  editActiveBColor.blend(&white, 40);
  fxActiveBColor.blend(&white, 20);
}


calcButton::calcButton(char* inFStr, word inLocX, word iny, byte width, byte inType)
  : drawObj(inLocX, iny, width, BUTTON_HEIGHT, touchLift) {

  int numChars = strlen(inFStr) + 1;
  fStr = (char*) malloc(numChars);
  strcpy(fStr, inFStr);
  bType = inType;
  aFStr = NULL;
  secondFx = false;
}


calcButton::calcButton(char* inFStr, char* inAFStr, word inLocX, word iny, byte width, byte inType)
  : drawObj(inLocX, iny, width, BUTTON_HEIGHT, touchLift) {

  int numChars = strlen(inFStr) + 1;
  fStr = (char*) malloc(numChars);
  strcpy(fStr, inFStr);

  numChars = strlen(inAFStr) + 1;
  aFStr = (char*) malloc(numChars);
  strcpy(aFStr, inAFStr);
  
  bType = inType;
  secondFx = false;
}

calcButton::~calcButton(void) {

  if (fStr) free(fStr);
  if (aFStr) free(aFStr);
}


void calcButton::idle(void) {

  if (aFStr && gSecondFx!=secondFx) {
    secondFx = gSecondFx;
    needRefresh = true;
  }
}


void calcButton::drawSelf(void) {

  colorObj* activeBaseColor;
  colorObj* touchedBaseColor;
  colorObj* activeTextColor;
  colorObj* touchedTextColor;
  
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
  if (secondFx && aFStr ) {
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

  calcOS->beep();
  if (secondFx && aFStr ) {        // its second function time, if we have a second function do it.
    mCalc.buttonClick(aFStr);      // Send our fuction label to the calculator object to parse.
  } else {
    mCalc.buttonClick(fStr);        // Send our fuction label to the calculator object to parse.
  }
  gSecondFx = false;                // First keystroke clears it.
  buttonPressed = true;            // Tell the calling program we changed something.
}

  
secondfxButton::secondfxButton(char* inFStr,word inLocX, word iny,byte width,byte inType)
  : calcButton(inFStr,inLocX,iny,width,inType) {  }


secondfxButton::~secondfxButton(void) {  }


void secondfxButton::idle() { secondFx = gSecondFx; }         // This one works different..


void secondfxButton::doAction(void) {
  
  calcOS->beep();
  secondFx = !secondFx;
  gSecondFx = secondFx;
}



degRadButton::degRadButton(word inLocX, word iny,byte width,byte height)
    : calcButton("-------",inLocX,iny,width,DEG_RAD_BTN) {
      
    setSize(width,height);
} 


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
    
    if ( mCalc.getDegrees()) {
      screen->drawText("Degrees");
    } else {
      screen->drawText("Radians");
    }
  }

  
 void  degRadButton::doAction(void) {

    calcOS->beep();
    mCalc.toggleDegrees();
    needRefresh = true;
 }


closeButton::closeButton(word inLocX, word inLocY,word inWidth, word inHeight,rpnCalc* inCalc)
  : calcButton("X",inLocX,inLocY,inWidth,inHeight) { mCalc =  inCalc; }

closeButton::~closeButton(void) { }


void closeButton::drawSelf(void) {

  screen->fillRoundRect(x, y, width, height, RADIUS, &red);
  screen->setTextColor(&black, &red);
  word textWidth = (CHAR_WIDTH * TEXT_SIZE) - 1;
  word dispWidth = width - (2 * RADIUS);
  screen->setCursor(x + RADIUS + ((dispWidth - textWidth) / 2), y + 1);
  screen->drawText("X");
}

  
void closeButton::doAction(void) { mCalc->close(); }
