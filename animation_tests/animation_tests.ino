#include <Adafruit_GFX.h>
#include <gfxfont.h>

#include <Adafruit_ILI9341.h>

//#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
//#include <PulseOut.h>
#include <runningAvg.h>
//#include <servo.h>
#include <timeObj.h>

//#include <adafruit_1431_Obj.h> // ADAFRUIT_1431
#include <adafruit_1947_Obj.h>  // ADAFRUIT_1947
//#include <bmpObj.h>
#include <displayObj.h>
#include <drawObj.h>
#include <label.h>
#include <lineObj.h>
#include <screen.h>

#include "sinwave.h"

sinwave sinDisp(20,20,200,7);
runningAvg amplitude(50);
runningAvg freq(50);
word midline = 30;

void setup() {

  //Serial.begin(9600);
  
  if (!initScreen(ADAFRUIT_1947)) {
    while(true); // Kill the process.
  }
  screen->fillScreen(&white);
 
  sinDisp.setFreq(10);
  sinDisp.setColors(&black,&white);
  viewList.addObj(&sinDisp);
}


void loop() {
  
  int newFreq;
  int newAmp;
  float newFAmp;
  screen->fillRect(19,midline-25,201,50,&white);
  idle();
  newFreq = random(15,40);
  newAmp = random(9,50);
  sinDisp.setFreq(freq.addData(newFreq));
  newFAmp = amplitude.addData(newAmp);
  newAmp = round(newFAmp);
  sinDisp.setLocation(20,midline-(newAmp/2));
  sinDisp.setSize(200,newAmp);
}
