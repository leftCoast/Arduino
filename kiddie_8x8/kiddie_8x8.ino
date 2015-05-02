#include <idlers.h>
#include <timeObj.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

#include "sprit.h"
#include "shakeNTinkle.h"

#define MIN_SHAKE     8
#define MIN_TINKLE    20
#define RUN_TIME      15000

timeObj  shakeTimer(RUN_TIME);

#define SENS1_SWITCH_PIN 3
#define SENS2_SWITCH_PIN 4

shakeNTinkle sensors(SENS1_SWITCH_PIN, SENS2_SWITCH_PIN);

#define KID_NAME " Kayla"
#define KID_FOOD " Cheeseburger!!"

static const uint8_t PROGMEM

eyeR_bmp[] =
{ B00000000,
  B00000000,
  B01100110,
  B10011001,
  B11011101,
  B01100110,
  B00000000,
  B00000000
},

eyeL_bmp[] =
{ B00000000,
  B00000000,
  B01100110,
  B10011001,
  B10111011,
  B01100110,
  B00000000,
  B00000000
},

eyeS1_bmp[] =
{ B00000000,
  B00000000,
  B00000000,
  B10011001,
  B10011001,
  B01100110,
  B00000000,
  B00000000
},

eyeS2_bmp[] =
{ B00000000,
  B00000000,
  B00000000,
  B00000000,
  B10011001,
  B01100110,
  B00000000,
  B00000000
},

heart1_bmp[] =
{ B00000000,
  B00100100,
  B01100110,
  B01111110,
  B00111100,
  B00011000,
  B00000000,
  B00000000
},

heart2_bmp[] =
{ B11111111,
  B10100101,
  B11100111,
  B11111111,
  B10111101,
  B10011001,
  B10000001,
  B11111111
},

smile_bmp[] =
{ B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10100101,
  B10011001,
  B01000010,
  B00111100
},
neutral_bmp[] =
{ B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10000001,
  B10111101,
  B01000010,
  B00111100
},
frown_bmp[] =
{ B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10000001,
  B10011001,
  B01100110,
  B00111100
},
off_bmp[] =
{ B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
},
on_bmp[] =
{ B11111111,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B11111111
};



class smileSprit : public bitmap8x8 {

  public :
    smileSprit(Adafruit_8x8matrix* inMatrix);

    virtual void  showFrame(void);
};

smileSprit::smileSprit(Adafruit_8x8matrix* inMatrix)
  : bitmap8x8(inMatrix)
{
  numFrames = 4;
}


void smileSprit::showFrame(void) {

  switch (frameNum) {
    case 0 : showBitmap(frown_bmp, 500); break;
    case 1 : showBitmap(neutral_bmp, 500); break;
    case 2 : showBitmap(smile_bmp, 1500);  break;
    case 3 : showBitmap(off_bmp, 0);  break;
  }
}


class eyesSprit : public bitmap8x8 {

  public :
    eyesSprit(Adafruit_8x8matrix* inMatrix);

    virtual void  showFrame(void);
};


eyesSprit::eyesSprit(Adafruit_8x8matrix* inMatrix)
  : bitmap8x8(inMatrix)
{
  numFrames = 9;
}


void eyesSprit::showFrame(void) {

  switch (frameNum) {
    case 0 : showBitmap(eyeS2_bmp, 100);  break;
    case 1 : showBitmap(eyeS1_bmp, 100);  break;
    case 2 : showBitmap(eyeL_bmp, 500);  break;
    case 3 : showBitmap(eyeR_bmp, 500);  break;
    case 4 : showBitmap(eyeL_bmp, 500);  break;
    case 5 : showBitmap(eyeR_bmp, 500);  break;
    case 6 : showBitmap(eyeS1_bmp, 100);  break;
    case 7 : showBitmap(eyeS2_bmp, 100);  break;
    case 8 : showBitmap(off_bmp, 0);  break;
  }
}


class heartSprit : public bitmap8x8 {

  public :
    heartSprit(Adafruit_8x8matrix* inMatrix);

    virtual void  showFrame(void);
};


heartSprit::heartSprit(Adafruit_8x8matrix* inMatrix)
  : bitmap8x8(inMatrix)
{
  numFrames = 10;
}


void heartSprit::showFrame(void) {

  switch (frameNum) {
    case 0 : showBitmap(heart1_bmp, 50);  break;
    case 1 : showBitmap(heart2_bmp, 50);  break;
    case 2 : showBitmap(heart1_bmp, 50);  break;
    case 3 : showBitmap(heart2_bmp, 50);  break;
    case 4 : showBitmap(heart1_bmp, 50);  break;
    case 5 : showBitmap(heart2_bmp, 50);  break;
    case 6 : showBitmap(heart1_bmp, 50);  break;
    case 7 : showBitmap(heart2_bmp, 50);  break;
    case 8 : showBitmap(heart1_bmp, 500);  break;
    case 9 : showBitmap(off_bmp, 0);  break;
  }
}


class thinkSprit : public sprit {

  public :
    thinkSprit(Adafruit_8x8matrix* inMatrix);

    virtual void startSprit(void);
            void newMap(byte odds);
            void drawMap(void);
            void addMap(void);
    virtual void idle(void);

    Adafruit_8x8matrix*  matrix;
    byte                 bitmap[8];
    timeObj              slowTimer;
    timeObj              fastTimer;
};


thinkSprit::thinkSprit(Adafruit_8x8matrix* inMatrix) {
  matrix = inMatrix;
  slowTimer.setTime(300);
  fastTimer.setTime(75);
}


void thinkSprit::startSprit(void) {

  hookup();
  matrix->clear();      // clear display
  newMap(20);
  drawMap();
  slowTimer.start();
  fastTimer.start();
  spritRunning = true;
}


void thinkSprit::newMap(byte odds) {

  for (byte i = 0; i < 8; i++) {
    for (byte j = 0; j < 8; j++) {
      if (random(1, 101) <= odds) {
        bitSet(bitmap[i], j);
      } else {
        bitClear(bitmap[i], j);
      }
    }
  }
}


void thinkSprit::drawMap(void) {

  for (byte i = 0; i < 8; i++) {
    for (byte j = 0; j < 8; j++) {
      matrix->drawPixel(i, j, bitRead(bitmap[i], j));
    }
  }
}


void thinkSprit::addMap(void) {

  for (byte i = 0; i < 8; i++) {
    for (byte j = 0; j < 8; j++) {
      if (bitRead(bitmap[i], j)) {
        matrix->drawPixel(i, j, LED_ON );
      }
    }
  }
}


void thinkSprit::idle(void) {

  boolean change = false;

  if (spritRunning) {
    if (slowTimer.ding()) {
      matrix->clear();      // clear display
      newMap(20);
      drawMap();
      change = true;
      slowTimer.stepTime();
    } 
    if (fastTimer.ding()) {
      newMap(4);
      addMap();
      change = true;
      fastTimer.stepTime();
    }
    if (change) matrix->writeDisplay();  // write the changes we just made to the display
  }
}



  Adafruit_8x8matrix matrix;

  timeObj animeTimer(1000);

  boolean litesOn;

  thinkSprit theThinkSprit(&matrix);
  smileSprit theSmileSprit(&matrix);
  eyesSprit  theEyesSprit(&matrix);
  heartSprit theHeartSprit(&matrix);
  text8x8    nameStr(&matrix, KID_NAME, 100);
  text8x8    foodStr(&matrix, KID_FOOD, 100);

  sprit* currentSprit = NULL;

  void setup() {

    sensors.begin();
    matrix.begin(0x70);  // pass in the address
    matrix.setRotation(2);
    animeTimer.start();
    currentSprit = &theThinkSprit;
    currentSprit->startSprit();
    litesOn = true;
  }


  



  /*
  void runLites() {

    bool change =  false;

    if (animeTimer.ding()) {
      int rNum = random(0, 500);
      if (rNum == 0) {
        runStr(KID_NAME, 100);
        resetCompLites();
      } else if (rNum == 25) {
        runStr(KID_FOOD, 50);
        resetCompLites();
      } else if (rNum == 10) {
        runFace();
        resetCompLites();
      } else if (rNum == 40) {
        runHeart();
        resetCompLites();
      } else if (rNum == 30) {
        runEyes();
        resetCompLites();
      }
      animeTimer.start();
    }
    
  }
  */

  void loop(void) {

    idle();
    if (currentSprit->active()) {
      animeTimer.start();
    } else if (animeTimer.ding()) {
      currentSprit->startSprit();
      animeTimer.stepTime();
    }
  }

  /*
  if (shakeTimer.ding()) {
    if (litesOn) {
      ROMbitmapAndHold(off_bmp, 1);
      litesOn = false;
    } else {
      if (sensors.getShake() > MIN_SHAKE) {
        resetCompLites();
        animeTimer.start();
        shakeTimer.start();
        litesOn = true;
      }
    }
  } else {
    if (sensors.getShake() > MIN_SHAKE) {
      shakeTimer.start();
    }
    runLites();
  }

  }
  */
