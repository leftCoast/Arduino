#include <blinker.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <runningAvg.h>
#include <servo.h>
#include <timeObj.h>


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

Adafruit_8x8matrix matrix;
timeObj slowTimer(300);
timeObj fastTimer(75);

void setup() {

  matrix.begin(0x70);  // pass in the address
  slowTimer.start();
  fastTimer.start();
}

byte bitmap[8];



void newMap(byte odds) {

  for (byte i = 0; i < 8; i++) {
    for (byte j = 0; j < 8; j++) {
      if (random(1,101)<=odds) {
        bitSet(bitmap[i],j);
      } else {
        bitClear(bitmap[i],j);
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
      if (bitRead(bitmap[i],j)) {
        matrix.drawPixel(i,j,LED_ON );
      }
    }
  }
}

void loop() {

  bool change =  false;
  
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
