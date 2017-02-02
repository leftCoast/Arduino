
/*
#include "colorObj.h"
#include "mapper.h"
#include "multiMap.h"
#include "timeObj.h"
*/
#include "neoPixel.h"


#define STRING_PIN        2
#define NUM_PIXELS        120
#define MS_PER_MOVE       50
#define PIXELS_PER_COLOR  20

neoPixel  theString(NUM_PIXELS,STRING_PIN);
timeObj   moveDelay(MS_PER_MOVE);

int           colorCount;
colorObj      currentColor;
enum          colorChoices{ colorRd, colorWt, colorBl };
colorChoices  colorChoice;

void setup() {
  theString.begin();
  theString.show();
  moveDelay.start();
  colorCount = 0;
  colorChoice = colorRd;
  currentColor.setColor(&red);
}


void setupNextMove(void) {

  colorObj  tempColor;
  for (int i=NUM_PIXELS-1;i>=1;i--) {
    tempColor = theString.getPixelColor(i-1);
    theString.setPixelColor(i,&tempColor);
  }
  if (colorCount>=PIXELS_PER_COLOR) {
    switch(colorChoice) {
      case colorRd : 
        colorChoice = colorWt;
        currentColor.setColor(&white);
        break;
      case colorWt :
        colorChoice = colorBl;
        currentColor.setColor(&blue);
        break;
      case colorBl :
        colorChoice = colorRd;
        currentColor.setColor(&red);
        break;  
    }
    colorCount = 0;
  }
  //currentColor.blend(&black,50);  // <<-- Try this for a cool effect.
  theString.setPixelColor(0,&currentColor);
  colorCount++;
}


void loop() {

  if (moveDelay.ding()) {
    moveDelay.stepTime();
    theString.show();
    setupNextMove();
  }
}
