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


#define BUBBLE_MS    20


mapper            rawToRadians(0,1023,0,2*PI);
runningAvg        smootherX(5);
runningAvg        smootherY(5);
Adafruit_BNO055*  bno = new Adafruit_BNO055(55, 0x28); 
timeObj           bubbleTimer(BUBBLE_MS);
float             offsetX;
float             offsetY;


void setup() {
   
   Serial.begin(115200);                                          // Fire up serial for debugging.
   Serial.println("We begin..");
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
   backColor.setColor(LC_OLIVE);
   backColor.blend(&green,20);
   screen->fillScreen(&backColor);                                    // Lets set the screen to the back color.
   setupModel();                                                  // This fires up the 3D rendering stuff. BUT seeing
   Serial.println("The end of the beginning.");
   offsetX = 0;
   offsetY = 0;
   
   theGrid = new grid(BOUND_CX,BOUND_CY);
   viewList.addObj(theGrid);
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
      new_angle.xRad = degToRad(smootherX.addData(-event.orientation.y - offsetY));
      new_angle.yRad = degToRad(smootherY.addData(-event.orientation.z - offsetY));
      new_angle.zRad = 0;
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
