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
//#include <runningAvg.h>
//#include <servo.h>
#include <timeObj.h>

#include "adafruit_1431_Obj.h"
//#include <bmpObj.h>
//#include <drawObj.h>
//#include <label.h>
//#include <screenObj.h>

#define DIMMER_POT  15  // For teensy
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
timeObj screenTimer(50);
mapper dimmer(0,1023,0,100);

void setup() {

  Serial.begin(9600);
  theOLED.begin();

  colorMap.addColor(0,&green);
  colorMap.addColor(10,&red);
  colorMap.addColor(30,&blue);
  colorMap.addColor(40,&green);
  colorIndex = 0;
  
  theOLED.setTextSize(1);

  screenTimer.start();
}

void loop() {

  colorObj aColor;
  int      dimControl;
  
  if (screenTimer.ding() ) {
    screenTimer.start();
    dimControl = analogRead(DIMMER_POT);
    aColor = colorMap.Map(colorIndex++);
    if (colorIndex>=40) colorIndex = 0;
    aColor.blend(&black,dimmer.Map(dimControl));
    //theOLED.fillRect(0, 0, 128, 128, aColor.getColor16());
    theOLED.fillScreen(&aColor);
    aColor.setColor(&black);
    aColor.blend(&white,dimmer.Map(dimControl));
    theOLED.setTextColor(&aColor);
    theOLED.setCursor(20,100);
    theOLED.drawText("No, Really?");
  }
}


