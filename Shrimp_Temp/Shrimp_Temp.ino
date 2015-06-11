

#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>

#include <colorObj.h>
#include <drawObj.h>
#include <label.h>
#include <screenObj.h>

#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <timeObj.h>

#include "LM35.h"

#define NUM_SIZE   2
#define LEFT_EDGE  110
#define BASELINE1  40
#define BASELINE2  80
#define PRECISION  1
#define TEXT_COLOR  GREEN
#define BACK_COLOR  BLACK

timeObj tempTimer(200);
LM35 shrimpSensor(A4);
LM35 seaSensor(A5);

label shrimpLabel("-000.0",NUM_SIZE);
label seaLabel("-000.0",NUM_SIZE);

void setup() {
    
  //Serial.begin(9600);
  if (initScreen(INV_LANDSCAPE)) {
    screen->fillScreen(BACK_COLOR);

    shrimpLabel.setLocation(LEFT_EDGE,BASELINE1);
    shrimpLabel.setJustify(TEXT_RIGHT);
    shrimpLabel.setPrecision(PRECISION);
    shrimpLabel.setColors(TEXT_COLOR,BACK_COLOR);
    
    seaLabel.setLocation(LEFT_EDGE,BASELINE2);
    seaLabel.setJustify(TEXT_RIGHT);
    seaLabel.setPrecision(PRECISION);
    seaLabel.setColors(TEXT_COLOR,BACK_COLOR);
    
    label label1("Shrimp :       Deg F",NUM_SIZE);
    
    label1.setColors(TEXT_COLOR,BACK_COLOR);
    label1.setLocation(10,BASELINE1);
    label1.setJustify(TEXT_LEFT);
    
    label1.draw();
    
    label1.setValue("Sea    :       Deg F");
    label1.setLocation(10,BASELINE2);
    label1.draw();

  } 
  tempTimer.start();
}

void loop() {

  if (tempTimer.ding()) {
  
    shrimpLabel.setValue(shrimpSensor.tempF());
    shrimpLabel.draw();
   
    seaLabel.setValue(seaSensor.tempF());
    seaLabel.draw();

    tempTimer.stepTime();
  }
}

