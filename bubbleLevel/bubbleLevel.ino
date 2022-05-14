#include <SD.h>
#include <adafruit_1947_Obj.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <idlers.h>
#include <screen.h>
#include <label.h>
#include <mapper.h>
#include <runningAvg.h>

#include <triDBase.h>
#include <triDVector.h>
#include <arrayList.h>
#include <facetList.h>

#include "bubbleTools.h"
#include "UI.h"

#define BUBBLE_MS    10
#define NUM_DATA_AVG 10

runningAvg        smootherX(NUM_DATA_AVG);
runningAvg        smootherY(NUM_DATA_AVG);
runningAvg        smootherZ(NUM_DATA_AVG);
Adafruit_BNO055*  bno = new Adafruit_BNO055(55, 0x28); 
timeObj           bubbleTimer(BUBBLE_MS);



void setup() {
   
   Serial.begin(115200);                                          // Fire up serial for debugging.
   //Serial.println("We begin..");
   if (!initScreen(ADAFRUIT_1947,ADA_1947_SHIELD_CS,PORTRAIT)) {  // Init screen.
      Serial.println("NO SCREEN!");                               // Screen init failed. Tell user.
      Serial.flush();                                             // Make sure the message gets out.
      while(true);                                                // Lock the process here.
   }
   if (!SD.begin(ADA_1947_SHIELD_SDCS)) {                         // With icons, we now MUST have an SD card.
      Serial.println("NO SD CARD!");                              // Tell user we have no SD card.
      Serial.flush();                                             // Make sure the message gets out.
      while(true);                                                // Lock the process here.
   }
   if (!bno->begin()) {
      Serial.print("No BNO055 detected");
   } else {
      bno->setExtCrystalUse(true);
   }
   backColor.setColor(&black);
   screen->fillScreen(&backColor);                                // Lets set the screen to the back color.
   ourEventMgr.begin();                                           // Startup our event manager.
   
   setupModel();                                                  // This fires up the 3D rendering stuff. BUT seeing
   theGrid = new grid(BOUND_CX,BOUND_CY);
   viewList.addObj(theGrid);
   setupUI();
   ourSAngleBtn->setCallback(setAngleClk);
   ourCAngleBtn->setCallback(clearAngleClk);
   clearAngleClk();
}


void setAngleClk(void) {
   
   sensors_event_t   event;

   bno->getEvent(&event);
   offsetX = -event.orientation.y;
   offsetY = -event.orientation.z;
   offsetZ = -event.orientation.x;
}


void clearAngleClk(void) {

   offsetX = 0;
   offsetY = 0;
   offsetZ = 0;
}


bool angleChange(void) {

   if (new_angle.xRad!=my_angle.xRad) return true;
   if (new_angle.yRad!=my_angle.yRad) return true;
   if (new_angle.zRad!=my_angle.zRad) return true;
   return false;
}


void checkBubble(void) {

   sensors_event_t   event;
   
   if (bubbleTimer.ding()) {
      bno->getEvent(&event);
      new_angle.xRad = degToRad(smootherX.addData(-event.orientation.y - offsetX));
      new_angle.yRad = degToRad(smootherY.addData(-event.orientation.z - offsetY));
      new_angle.zRad = degToRad(smootherZ.addData(-event.orientation.x - offsetZ));
      if (angleChange()) {
         renderMan->setObjAngle(&new_angle);
         my_angle = new_angle;
         bubbleTimer.start();
      }
   }
}


void loop() {

   idle();
   checkBubble();
}
