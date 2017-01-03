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

//#include <bmpObj.h>
//#include <drawObj.h>
//#include <label.h>
//#include <screenObj.h>

//#define DIMMER_POT  15  // For teensy
#define DIMMER_POT  A0  // For Uno

#define sclk 13
#define mosi 11
#define dc   7
#define cs   9
#define rst  8

//Adafruit_SSD1351 theOLED = Adafruit_SSD1351(cs, dc, mosi, sclk, rst);
Adafruit_SSD1351 theOLED = Adafruit_SSD1351(cs, dc, rst); // Should be HW pins. 
//SPISettings settings(24000000, MSBFIRST, SPI_MODE3); // Teensy 3.1 max SPI

colorMultiMap colorMap;
int colorIndex;
timeObj screenTimer(50);
mapper dimmer(0,1023,0,100);

void setup() {

  //Serial.begin(9600);
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
    theOLED.fillScreen(aColor.getColor16());
    aColor.setColor(&black);
    aColor.blend(&white,dimmer.Map(dimControl));
    theOLED.setTextColor(aColor.getColor16());
    theOLED.setCursor(20,100);
    theOLED.print("What the heck?");
  }
}


