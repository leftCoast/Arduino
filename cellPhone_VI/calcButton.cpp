#include "calcButton.h"
#include <calculator.h>


#define TEXT_SIZE     2
#define CHAR_WIDTH    6
#define BUTTON_HEIGHT 18
#define RADIUS        3

extern calculator mCalc;
extern boolean    buttonPressed;
       boolean    gSecondFx = false;


colorObj numberActiveBColor(LC_WHITE);
colorObj numberActiveTColor(LC_BLACK);
colorObj numberClickedBColor(LC_BLACK);
colorObj numberClickedTColor(LC_WHITE);

colorObj editActiveBColor(LC_BLUE);
colorObj editActiveTColor(LC_WHITE);
colorObj editClickedBColor(LC_WHITE);
colorObj editClickedTColor(LC_BLACK);

colorObj fxActiveBColor(LC_BLACK);
colorObj fxActiveTColor(LC_WHITE);
colorObj fxClickedBColor(LC_WHITE);
colorObj fxClickedTColor(LC_BLACK);


void setupButtonColors(void) {

  numberActiveBColor.blend(&black, 40);
  numberActiveBColor.blend(&yellow, 25);
  numberActiveTColor.blend(&black, 60);
  editActiveBColor.blend(&white, 40);
  fxActiveBColor.blend(&white, 20);
}


calcButton::calcButton(char* inFStr, word inLocX, word iny, byte width, byte inType)
  : drawObj(inLocX, iny, width, BUTTON_HEIGHT, true) {

  int numChars = strlen(inFStr) + 1;
  fStr = (char*) malloc(numChars);
  strcpy(fStr, inFStr);
  bType = inType;
  aFStr = NULL;
  secondFx = false;
}


calcButton::calcButton(char* inFStr, char* inAFStr, word inLocX, word iny, byte width, byte inType)
  : drawObj(inLocX, iny, width, BUTTON_HEIGHT, true) {

  int numChars = strlen(inFStr) + 1;
  fStr = (char*) malloc(numChars);
  strcpy(fStr, inFStr);

  numChars = strlen(inAFStr) + 1;
  aFStr = (char*) malloc(numChars);
  strcpy(aFStr, inAFStr);
  
  bType = inType;
  secondFx = false;
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

  beep();
  if (secondFx && aFStr ) {        // its second function time, if we have a second function do it.
    mCalc.buttonClick(aFStr);      // Send our fuction label to the calculator object to parse.
  } else {
    mCalc.buttonClick(fStr);        // Send our fuction label to the calculator object to parse.
  }
  gSecondFx = false;                // First keystroke clears it.
  buttonPressed = true;            // Tell the calling program we changed something.
}


void calcButton::beep(void) {

  for(int i=0;i<20;i++) {
    digitalWrite(BEEP_PIN, HIGH);
    delay(1);
    digitalWrite(BEEP_PIN, LOW);
    delay(1);
  }
  digitalWrite(BEEP_PIN, HIGH);
}
  


secondfxButton::secondfxButton(char* inFStr,word inLocX, word iny,byte width,byte inType)
  : calcButton(inFStr,inLocX,iny,width,inType) { }

void secondfxButton::idle() { secondFx = gSecondFx; }         // This one works different..

void secondfxButton::doAction(void) {
  
  beep();
  secondFx = !secondFx;
  gSecondFx = secondFx;
  //needRefresh = true;   // Right now its not actually changing so lets not redraw it.
}



degRadButton::degRadButton(word inLocX, word iny,byte width,byte height)
    : calcButton("-------",inLocX,iny,width,DEG_RAD_BTN) {
      
    setSize(width,height);
} 
                  

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

    beep();
    mCalc.toggleDegrees();
    needRefresh = true;
 }
