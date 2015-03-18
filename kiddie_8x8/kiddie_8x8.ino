#include <lists.h>
#include <mapper.h>
#include <PulseOut.h>
#include <timeObj.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

//#define KID_NAME " Kayla"
#define KID_NAME " Shelbers"

static const uint8_t PROGMEM
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

Adafruit_8x8matrix matrix;

timeObj slowTimer(300);
timeObj fastTimer(75);
timeObj animeTimer(100);

byte bitmap[8];

void setup() {

  matrix.begin(0x70);  // pass in the address
  slowTimer.start();
  fastTimer.start();
  animeTimer.start();
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


void runFace(void) {

  matrix.clear();
  matrix.drawBitmap(0, 0, frown_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(500);

  matrix.clear();
  matrix.drawBitmap(0, 0, neutral_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(500);

  matrix.clear();
  matrix.drawBitmap(0, 0, smile_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(1500);

}

void runHeart(void) {

  for (int i = 0; i < 4; i++) {
    matrix.clear();
    matrix.drawBitmap(0, 0, heart1_bmp, 8, 8, LED_ON);
    matrix.writeDisplay();
    delay(50);
    matrix.clear();
    matrix.drawBitmap(0, 0, heart2_bmp, 8, 8, LED_ON);
    matrix.writeDisplay();
    delay(50);
  }
  matrix.clear();
  matrix.drawBitmap(0, 0, heart1_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(500);
}

void runStr(char* inStr) {

  matrix.setTextSize(1);
  matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
  matrix.setTextColor(LED_ON);
  int endNum = -6 * strlen(inStr);
  for (int8_t x = 0; x >= endNum; x--) {
    matrix.clear();
    matrix.setCursor(x, 0);
    matrix.print(inStr);
    matrix.writeDisplay();
    delay(100);
  }

}


void loop() {


  bool change =  false;
  if (animeTimer.ding()) {
    int rNum = random(0, 500);
    if (rNum == 0) {
      runStr(KID_NAME);
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
}
