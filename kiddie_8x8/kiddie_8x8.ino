#include <timeObj.h>

#include <Wire.h>
#include <Adafruit_GFX.h>

#include <Adafruit_LEDBackpack.h>


#include "sprit.h"

#define KID_NAME " Julie"
#define KID_FOOD " Shrimp, Yum!!"

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
};


class smileSprit : public sprit8x8 {
  
  public :
                smileSprit(Adafruit_8x8matrix* inMatrix);
                ~smileSprit(void);
  
  virtual void  idle(void);
};

smileSprit::smileSprit(Adafruit_8x8matrix* inMatrix) 
: sprit8x8(inMatrix)
{ numFrames = 3; }


smileSprit::~smileSprit(void) {  }


void smileSprit::idle(void) {
  
  if (changeFrame()) {
    switch(frameNum) {
      case 0 : showFrame(frown_bmp,500); break;
      case 1 : showFrame(neutral_bmp,500); break;
      case 2 : showFrame(smile_bmp,1500);  break;
    }
  }
}

  
Adafruit_8x8matrix matrix;

timeObj slowTimer(300);
timeObj fastTimer(75);
timeObj animeTimer(100);

byte bitmap[8];

sprit8x8 theSmileSprit(&matrix);

void setup() {

  matrix.begin(0x70);  // pass in the address
  matrix.setRotation(2);
  slowTimer.start();
  fastTimer.start();
  animeTimer.start();
  
  theSmileSprit.startSprit();
}


void newMap(byte odds) {

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


void drawMap(byte *bitMap) {

  for (byte i = 0; i < 8; i++) {
    for (byte j = 0; j < 8; j++) {
      matrix.drawPixel(i, j, bitRead(bitmap[i], j));
    }
  }
}


void addMap(byte *bitMap) {

  for (byte i = 0; i < 8; i++) {
    for (byte j = 0; j < 8; j++) {
      if (bitRead(bitmap[i], j)) {
        matrix.drawPixel(i, j, LED_ON );
      }
    }
  }
}


void ROMbitmapAndHold(const uint8_t* bitmap,unsigned long inMs) {
  
  matrix.clear();
  matrix.drawBitmap(0, 0, bitmap, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(inMs);
}


void runFace(void) {

  ROMbitmapAndHold(frown_bmp,500);
  ROMbitmapAndHold(neutral_bmp,500);
  ROMbitmapAndHold(smile_bmp,1500);
}


void runEyes(void) {
  
  ROMbitmapAndHold(eyeS2_bmp,100);
  ROMbitmapAndHold(eyeS1_bmp,100);
  ROMbitmapAndHold(eyeL_bmp,500);
  ROMbitmapAndHold(eyeR_bmp,500);
  ROMbitmapAndHold(eyeL_bmp,500);
  ROMbitmapAndHold(eyeR_bmp,500);
  ROMbitmapAndHold(eyeS1_bmp,100);
  ROMbitmapAndHold(eyeS2_bmp,100);
  matrix.clear();
}


void runHeart(void) {

  for (int i = 0; i < 4; i++) {
    ROMbitmapAndHold(heart1_bmp,50);
    ROMbitmapAndHold(heart2_bmp,50);
  }
  ROMbitmapAndHold(heart1_bmp,500);
}


void runStr(char* inStr,unsigned int inDelay) {

  matrix.setTextSize(1);
  matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
  matrix.setTextColor(LED_ON);
  int endNum = -6 * strlen(inStr);
  for (int8_t x = 0; x >= endNum; x--) {
    matrix.clear();
    matrix.setCursor(x, 0);
    matrix.print(inStr);
    matrix.writeDisplay();
    delay(inDelay);
  }
}


void loop() {


  idle();
  if (!theSmileSprit.active() && animeTimer.ding()) {
    theSmileSprit.startSprit();
    animeTimer.stepTime();
  }
  /*
  bool change =  false;
  if (animeTimer.ding()) {
    int rNum = random(0, 500);
    if (rNum == 0) {
      runStr(KID_NAME,100);
      slowTimer.start();
      fastTimer.start();
    } else if (rNum == 25) {
      runStr(KID_FOOD,50);
      slowTimer.start();
      fastTimer.start();
    } else if (rNum == 10) {
      runFace();
      slowTimer.start();
      fastTimer.start();
    } else if (rNum == 40) {
      runHeart();
      slowTimer.start();
      fastTimer.start();
    } else if (rNum == 30) {
      runEyes();
      slowTimer.start();
      fastTimer.start();
    }
    animeTimer.start();
  }
  if (slowTimer.ding()) {
    matrix.clear();      // clear display
    newMap(20);
    drawMap(bitmap);
    change = true;
    slowTimer.stepTime();
  }
  if (fastTimer.ding()) {
    newMap(4);
    addMap(bitmap);
    change = true;
    fastTimer.stepTime();
  }
  if (change) matrix.writeDisplay();  // write the changes we just made to the display
  */
}
