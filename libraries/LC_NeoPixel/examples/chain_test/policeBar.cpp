
#include "policeBar.h"

#define FLASH_TIME   40
#define NUM_FLASHES  60

byte flashPattern[NUM_FLASHES] {
  B11100000, B00000000,
  B11100000, B00000000,
  B11100000, B00000000,

  B00000111, B00000000,
  B00000111, B00000000,
  B00000111, B00000000,

  B11100000, B00000000,
  B11100000, B00000000,
  B11100000, B00000000,

  B00000111, B00000000,
  B00000111, B00000000,
  B00000111, B00000000,

  B11100000, B00000000,
  B11100000, B00000000,
  B11100000, B00000000,

  B00000111, B00000000,
  B00000111, B00000000,
  B00000111, B00000000,

  B11111000, B11100000,
  B11111000, B11100000,
  B11111000, B11100000,

  B00011111, B00000111,
  B00011111, B00000111,
  B00011111, B00000111,

  B11111000, B11100000,
  B11111000, B11100000,
  B11111000, B11100000,

  B00011111, B00000111,
  B00011111, B00000111,
  B00011111, B00000111
};


policeBar::policeBar(void)
  : pixelGroup(8) {

  init = false;
  nextState = pbOff;
  state = pbOff;
  flashTimer.setTime(FLASH_TIME);

  pbRed.setColor(&red);
  //pbRed = pbRed.blend(&white,5);
  pbBlue.setColor(&blue);
  pbBlue = pbBlue.blend(&white, 10);
}


policeBar::~policeBar(void) { }


void policeBar::lightOff(void) {
  nextState = pbOff;
}


void policeBar::floodOn(void) {
  nextState = pbFlood;
}


void policeBar::blinkOn(void) {
  nextState = pbBlink;
}


void policeBar::setupBlink(void) {

  setPixels(&black);  // Clear 'm first
  flashIndex = 0;
  flashTimer.start();
}


colorObj* policeBar::chooseColor(int index) {

  if (random(0, NUM_FLASHES) < flashIndex/1.5) {
    long val = random(0, 11);
    switch (val) {
      case 0 :
      case 1 :
      case 2 :
      case 3 : return &pbRed;
      case 4 :
      case 5 :
      case 6 :
      case 7 : return &pbBlue;
      case 8 :  return &white;
      case 9 :
      case 10 : return &black;
    }
  } else {
    switch (index) {
      case 0 :
      case 1 :
      case 2 : return &pbRed;
      case 3 :
      case 4 : if (random(1, 6) > 3) return &white; else return &black;
      case 5 :
      case 6 :
      case 7 : return &pbBlue;
    }
    return &black;
  }
}


void  policeBar::setLights(int index) {

  for (byte i = 0; i < 8; i++) {
    if (bitRead(flashPattern[index], i)) {
      setPixel(i, chooseColor(i));
    } else {
      setPixel(i, &black);
    }
  }
}


void policeBar::blinkTime(void) {

  if (flashTimer.ding()) {
    setLights(flashIndex);
    flashIndex++;
    if (flashIndex >= NUM_FLASHES) {
      flashIndex = 0;
    }
    flashTimer.stepTime();
  }
}


void policeBar::draw(void) {

  if (!init) {                    // if were not initilized..
    setPixels(&black);
    init = true;
  } else if (nextState != state) { // Or if state is changing..
    switch (nextState) {
      case pbOff : setPixels(&black); break;
      case pbFlood : setPixels(&white); break;
      case pbBlink : setupBlink(); break;
    }
    state = nextState;
  } else if (state == pbBlink) {  // Or.. We're running the blinkers.
    blinkTime();
  }
}


