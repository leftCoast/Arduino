#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>

#include <blinker.h>
#include <colorObj.h>
//#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
//#include <PulseOut.h>
#include <runningAvg.h>
//#include <servo.h>
#include <timeObj.h>

#include "adafruit_1431_Obj.h"
//#include <bmpObj.h>
//#include <drawObj.h>
//#include <label.h>
//#include <screenObj.h>

#define COLOR_POT   14   // For teensy 
#define DIMMER_POT  15  
#define NUM_AVERAGES  25
#define SCREEN_TIME  30                                     // Like retrace time

//#define DIMMER_POT  A0  // For Uno

#define sclk 13
#define mosi 11
#define dc   7
#define cs   9
#define rst  8

//#define ADAFRUIT_1947 1   // 2.8" TFT Touch Shield for Arduino w/Capacitive Touch.
//#define ADAFRUIT_1431 2   // OLED Breakout Board - 16-bit Color 1.5" w/microSD holder.

adafruit_1431_Obj theOLED(cs, dc, mosi, sclk, rst);

colorMultiMap colorMap;
int colorIndex;
timeObj screenTimer(SCREEN_TIME);
mapper potToPercent(0,1023,0,100);
mapper potToIndex(0,1023,0,39);

colorObj  screenColor;

int lastColor;
int lastDim;

runningAvg dimAve(NUM_AVERAGES);
runningAvg colorAve(NUM_AVERAGES);

bool seenChange;

void setup() {

  //Serial.begin(9600);
  theOLED.begin();

  colorMap.addColor(0,&green);
  colorMap.addColor(10,&red);
  colorMap.addColor(30,&blue);
  colorMap.addColor(40,&green);
  
  lastColor = -1;
  lastDim = -1;
  theOLED.setTextSize(1);
  checkDials();
  setupColors();
  screenTimer.start();
}


void checkDials(void) {

  int      dimControl;
  int      colorControl;

  dimAve.addData(analogRead(DIMMER_POT));
  dimControl = trunc(dimAve.getAve());

  colorAve.addData(analogRead(COLOR_POT));
  colorControl = trunc(colorAve.getAve());
  
  if (dimControl!=lastDim||colorControl!=lastColor) {
    lastDim = dimControl;
    lastColor = colorControl;
    seenChange =  true;
  }
}


void setupColors(void) {

  colorObj aColor;
  
  colorIndex = potToIndex.Map(lastColor);
  aColor = colorMap.Map(colorIndex);
  screenColor = aColor.mixColors(&black,potToPercent.Map(lastDim));
  aColor.setColor(&black);
  aColor.blend(&white,potToPercent.Map(lastDim));
  theOLED.setTextColor(&aColor);
}


void loop() {
  
  checkDials();
  if (screenTimer.ding()) {
    screenTimer.start();
    if (seenChange) {
      setupColors();
      theOLED.fillScreen(&screenColor);
      theOLED.setCursor(20,75);
      theOLED.drawText("Again? Really?");
      seenChange = false;
    }
  }
}


