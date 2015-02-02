#include <colorObj.h>
#include <SPI.h>
#include <TouchScreen.h>
#include <Adafruit_TFTLCD.h>
#include <pin_magic.h>


#include <SD.h>

#include <Adafruit_GFX.h>

#include <bmpObj.h>
#include <drawObj.h>
#include <label.h>
#include <radioBtn.h>
#include <screenObj.h>
#include <touchObj.h>

#include <blinker.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <servo.h>
#include <timeObj.h>
#include "LM35.h"

#define NUM_SIZE   2
#define LEFT_EDGE  90
#define BASELINE1  70
#define BASELINE2  163
#define PRECISION  1

timeObj printTimer(1000);
LM35 tempSensor(A4);

label temp1("-000.0",NUM_SIZE);
label temp2("-000.0",NUM_SIZE);

void setup() {
  
  Point inDest;
  
  //Serial.begin(9600);
  if (initScreen(INV_PORTRAIT)) {
    screen->fillScreen(WHITE);
    /*
    if (initSDCard()) {
      inDest.x = 0;
      inDest.y = 0;
      bmpObj mBitmap("panel.bmp",inDest);
      //bmpObj mBitmap("on.bmp",inDest);
      mBitmap.getInfo();
      if (mBitmap.haveInfo){
        mBitmap.plotBmp();
      }
    }
    */
    temp1.setLocation(LEFT_EDGE,BASELINE1);
    temp1.setJustify(TEXT_RIGHT);
    temp1.setPrecision(PRECISION);

    temp2.setLocation(LEFT_EDGE,BASELINE2);
    temp2.setJustify(TEXT_RIGHT);
    temp2.setPrecision(PRECISION);
    
    label label1("Temp 1:",NUM_SIZE);
    label1.setLocation(10,BASELINE1);
    label1.setJustify(TEXT_LEFT);
    label1.draw();
  } 
  printTimer.start();
}

void loop() {

  if (printTimer.ding()) {
    temp1.setValue(tempSensor.tempF());
    temp1.draw();
    /*
    Serial.print("Temp C  : ");
    Serial.println(tempSensor.tempC());
    Serial.print("Temp F  : ");
    Serial.println(tempSensor.tempF());
    Serial.println();
    */
    printTimer.stepTime();
  }
}

