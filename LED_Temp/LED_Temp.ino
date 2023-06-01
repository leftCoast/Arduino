#include <idlers.h>
#include <timeObj.h>
#include <runningAvg.h>
#include <LM35.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

Adafruit_8x8matrix matrixL;// = Adafruit_8x8matrix();
Adafruit_8x8matrix matrixR;

timeObj tempTimer(1000);    // Gives us a reading every second.
LM35 tempSensor(A0);        // Choose whatever analog pin you like.

runningAvg  smoother(20);

void setup() {
  
  matrixL.begin(0x70);              // pass in the address
  matrixL.setRotation(2);
  matrixL.setTextSize(1);
  matrixL.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
  matrixL.setTextColor(LED_ON);
  matrixL.setBrightness(15);

  matrixR.begin(0x71);              // pass in the address
  matrixR.setRotation(2);
  matrixR.setTextSize(1);
  matrixR.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
  matrixR.setTextColor(LED_ON);
  matrixR.setBrightness(15);

  tempTimer.start();
}


void writeStr(char* inStr) {
  
  matrixL.clear();
  matrixL.setCursor(0,0);
  matrixL.print(inStr);
  
  matrixR.clear();
  matrixR.setCursor(-8,0);
  matrixR.print(inStr);
  
  matrixL.writeDisplay();
  matrixR.writeDisplay();
}


void loop() {
  
  char tempStr[10];
  int tempF;
  
  idle();
  if (tempTimer.ding()) {
    tempF = round(smoother.addData(tempSensor.tempF()));
    if (tempF<100) {
      snprintf (tempStr,3,"%dd",tempF);
      strcat(tempStr,"F");
      writeStr(tempStr);
    } else {
      writeStr("+++");
    }
    tempTimer.stepTime();
  }
}
