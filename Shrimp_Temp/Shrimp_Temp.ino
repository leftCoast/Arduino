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
#include <runningAvg.h>

#include "LM35.h"

#define NUM_SIZE   2
#define LEFT_EDGE  110
#define BASELINE1  40
#define BASELINE2  80
#define PRECISION  1

timeObj printTimer(100);
LM35 shrimpSensor(A4);
LM35 seaSensor(A5);

label shrimpLabel("-000.0",NUM_SIZE);
label seaLabel("-000.0",NUM_SIZE);

runningAvg  shrimpSmooth(5);
runningAvg  seaSmooth(5);

void setup() {
  
  TSPoint inDest;
  
  //Serial.begin(9600);
  if (initScreen(INV_LANDSCAPE)) {
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
    shrimpLabel.setLocation(LEFT_EDGE,BASELINE1);
    shrimpLabel.setJustify(TEXT_RIGHT);
    shrimpLabel.setPrecision(PRECISION);

    seaLabel.setLocation(LEFT_EDGE,BASELINE2);
    seaLabel.setJustify(TEXT_RIGHT);
    seaLabel.setPrecision(PRECISION);
    
    label label1("Shrimp :       Deg F",NUM_SIZE);
 
    label1.setLocation(10,BASELINE1);
    label1.setJustify(TEXT_LEFT);
    label1.draw();
    
    label1.setValue("Sea    :       Deg F");
    label1.setLocation(10,BASELINE2);
    label1.draw();
    
    /*
    label1.setValue("Deg. F");
    label1.setLocation(180,BASELINE1);
    label1.draw();
    */
  } 
  printTimer.start();
}

void loop() {

  float shrimpTemp;
  float seaTemp;
  
  if (printTimer.ding()) {
    shrimpTemp = shrimpSmooth.addData(shrimpSensor.tempF());
    shrimpLabel.setValue(shrimpTemp);
    shrimpLabel.draw();
    
    seaTemp = shrimpSmooth.addData(seaSensor.tempF());
    seaLabel.setValue(seaTemp);
    seaLabel.draw();
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

