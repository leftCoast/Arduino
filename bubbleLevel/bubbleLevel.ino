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

#define BUBBLE_MS    250


Adafruit_BNO055   bno = Adafruit_BNO055(55, 0x28); 
timeObj           bubbleTimer(BUBBLE_MS);
timeObj           readTimer(100);
float             offsetX;
float             offsetY;
float             offsetZ;

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
   if (!bno.begin()) {
      Serial.print("No BNO055 detected");
   } else {
      bno.setExtCrystalUse(true);
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


// Get rotation by radians..
triDRotation getRotation(void) {

   triDVector     gravVect;
   triDVector     gravXVect;
   triDVector     gravYVect;
   triDVector     zVector(0,0,1);
   triDRotation   result;
   
   imu::Vector<3> Grav = bno.getVector(Adafruit_BNO055::VECTOR_GRAVITY);
   gravVect.setVector(Grav.y(),Grav.x(),Grav.z());
   
   gravVect.printVector();
   gravVect.normalize();
   gravVect.invert();
   gravVect.printVector();
   
   gravXVect.setVector(0,gravVect.getY(),gravVect.getZ());
   gravYVect.setVector(gravVect.getX(),0,gravVect.getZ());
   result.yRad = gravYVect.angleBetween(&zVector);
   result.xRad = gravXVect.angleBetween(&zVector);
   result.zRad = 0;
   
   Serial.print("xRad : ");Serial.print(result.xRad);Serial.print("   Deg : ");Serial.println(radToDeg(result.xRad));
   Serial.print("yRad : ");Serial.print(result.yRad);Serial.print("   Deg : ");Serial.println(radToDeg(result.yRad));
   Serial.println("-----------");
   return result;
}


void setAngleClk(void) {
   
   triDRotation ourRotation;
   
   ourRotation = getRotation();
   offsetX = -ourRotation.xRad;
   offsetY = -ourRotation.yRad;
   offsetZ = -ourRotation.zRad;
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

   triDRotation rot;
      
   if (readTimer.ding()) {
      rot = getRotation();
      rotationAngleX->setValue(radToDeg(rot.xRad));
      rotationAngleY->setValue(radToDeg(rot.yRad));
      rotationAngleZ->setValue(radToDeg(rot.zRad));

      // Offset * transpose
      new_angle.xRad = rot.xRad - offsetX;
      new_angle.yRad = rot.yRad - offsetY;
      new_angle.zRad = rot.zRad - offsetZ;
      
      if (angleChange()) {
         renderMan->setObjAngle(&new_angle);
         
         modelAngleX->setValue(radToDeg(new_angle.xRad));
         modelAngleY->setValue(radToDeg(new_angle.yRad));
         modelAngleZ->setValue(radToDeg(new_angle.zRad));
         
         my_angle = new_angle;
         readTimer.start();
      }
   }
}

float    degrees = -90;
timeObj  scanTimer(20);
bool     neg = false;
float    yVal;
void backAndForth(void) {

   if (scanTimer.ding()) {
      new_angle.xRad = degToRad(degrees);
      if (neg) {yVal = -degrees;} else {yVal = degrees;}
      new_angle.yRad = degToRad(yVal);
      new_angle.zRad = 0; //degToRad(degrees);
      renderMan->setObjAngle(&new_angle);
      if (neg) {
         if (degrees>-90) {
            degrees--;
         } else {
            neg = false;
            degrees++;
         }
      } else {
         if (degrees<90) {
            degrees++;
         } else {
            neg = true;
            degrees--;
         }
      }
      scanTimer.start();
   }
}


void loop() {
   
   idle();
   backAndForth();
   //checkBubble();
   
}
