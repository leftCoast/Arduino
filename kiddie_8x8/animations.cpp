#include "animations.h"


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
twelve_bmp[] =
{ B00000000,
  B00100100,
  B01101010,
  B00100010,
  B00100100,
  B00101000,
  B00101110,
  B00000000
},
star1_bmp[] =
{ B11100000,
  B11100000,
  B11000000,
  B00001000,
  B00010000,
  B00000011,
  B00000111,
  B00000111
},
star2_bmp[] =
{ B11000000,
  B10000100,
  B00001110,
  B00000100,
  B00100000,
  B01110000,
  B00100001,
  B00000011
},
star3_bmp[] =
{ B00000100,
  B00000111,
  B00001110,
  B00010010,
  B01001000,
  B01110000,
  B11100000,
  B00100000
},
star4_bmp[] =
{ B00000111,
  B00100111,
  B01110011,
  B00100000,
  B00000100,
  B11001110,
  B11100100,
  B11100000
},
star5_bmp[] =
{ B00100011,
  B11100001,
  B01110000,
  B01000000,
  B00000010,
  B00001110,
  B10000111,
  B11000100
},
starS1_bmp[] =
{ B00000000,
  B00000000,
  B00000000,
  B00001000,
  B00010000,
  B00000000,
  B00000000,
  B00000000
},
starS2_bmp[] =
{ B00000000,
  B00000100,
  B00001110,
  B00000100,
  B00100000,
  B01110000,
  B00100000,
  B00000000
},
starE1_bmp[] =
{ B00000100,
  B00000111,
  B00001110,
  B00000010,
  B01000000,
  B01110000,
  B11100000,
  B00100000
},
starE2_bmp[] =
{ B00000111,
  B00000111,
  B00000011,
  B00000000,
  B00000000,
  B11000000,
  B11100000,
  B11100000
},
starE3_bmp[] =
{ B00000011,
  B00000001,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B10000000,
  B11000000
},

restFace_bmp[] =
{ B00000000,
  B00000000,
  B11100111,
  B00000000,
  B00000000,
  B00111100,
  B00000000,
  B00000000
},
shockFace_bmp[] =
{ B00000000,
  B01000010,
  B11100111,
  B01000010,
  B00011000,
  B00100100,
  B00011000,
  B00000000
};

// class smileSprit

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


// class eyesSprit


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


// class heartSprit


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


// class starSprit


starSprit::starSprit(Adafruit_8x8matrix* inMatrix)
  : bitmap8x8(inMatrix)
{
  numFrames = 46;
}


void starSprit::showFrame(void) {

  if (frameNum == 0) {
    showBitmap(starS1_bmp, 150);
  } else if (frameNum == 1) {
    showBitmap(starS2_bmp, 75);
  } else if (frameNum == 42) {
    showBitmap(starE1_bmp, 50);
  } else if (frameNum == 43) {
    showBitmap(starE2_bmp, 50);
  } else if (frameNum == 44) {
    showBitmap(starE3_bmp, 50);
  } else if (frameNum == 45) {
    showBitmap(off_bmp, 200);
  } else {
    switch (frameNum % 5) {
      case 0 :  showBitmap(star1_bmp, 50);  break;
      case 1 : showBitmap(star2_bmp, 50);  break;
      case 2 : showBitmap(star3_bmp, 50);  break;
      case 3 : showBitmap(star4_bmp, 50);  break;
      case 4 : showBitmap(star5_bmp, 50);  break; 
    }
  }
}


// class twelveSprit

twelveSprit::twelveSprit(Adafruit_8x8matrix* inMatrix)
  : bitmap8x8(inMatrix)
{
  numFrames = 10;
}


void twelveSprit::showFrame(void) {

  switch (frameNum) {
    case 0 : showBitmap(twelve_bmp, 1000);     break;
    case 1 : showBitmap(twelve_bmp, 50,true);  break;
    case 2 : showBitmap(twelve_bmp, 50);       break;
    case 3 : showBitmap(twelve_bmp, 50,true);  break;
    case 4 : showBitmap(twelve_bmp, 50);       break;
    case 5 : showBitmap(twelve_bmp, 50,true);  break;
    case 6 : showBitmap(twelve_bmp, 50);       break;
    case 9 : showBitmap(off_bmp, 100);         break;
  }
}


// class thinkSprit

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


