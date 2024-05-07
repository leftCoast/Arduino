#include "dice.h"
#include <Wire.h>
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>
#include <idlers.h>



dice::dice(void)
   : Adafruit_8x8matrix(),
   idler() { ourState = prestart; }


dice::~dice(void) { }


void dice::begin(uint8_t ourI2CAddr) {

   Adafruit_8x8matrix::begin(ourI2CAddr);
   hookup();
   frameTimer.setTime(FRAME_MS,false);
   ourState = waiting; 
}


void dice::clearDie(void) {

   clear();
   writeDisplay();
}


void dice::clearSratch(void) {
   
   for (int i=0;i<8;i++) {
      scratch[i]=0;
   }  
}


void dice::orBMaps(const uint8_t* bmp1,const uint8_t* bmp2) {

   for (int i=0;i<8;i++) {
      scratch[i]=bmp1[i]|bmp2[i];
   }
}


void dice::drawBmp(const uint8_t* bmp) {

   if (ourState != prestart) {
      clear();
      drawBitmap(0, 0, bmp, 8, 8, LED_ON);
      writeDisplay();
   }
}


void dice::doFuzz(float fuzzMs) {

   if (ourState != prestart) {
      stateTimer.setTime(fuzzMs);
      frameTimer.start();
      ourState = fuzzing;
   }
}


void dice::showNum(int num,float showMs){

   if (num>0 && num<=6) {
      if (showMs>0) {
         stateTimer.setTime(showMs);
         ourState = showing;
      } else {
         ourState = waiting;
      }
      switch(num) {
         case 1 : drawBmp(bmpA); break;
         case 2 : drawBmp(bmpB); break;
         case 3 :
            clearSratch();
            orBMaps(bmpA,bmpB);
            drawBmp(scratch);
         break;
         case 4 :
            clearSratch();
            orBMaps(bmpB,bmpC);
            drawBmp(scratch);
         break;
         case 5 :
            clearSratch();
            orBMaps(bmpB,bmpC);
            orBMaps(scratch,bmpA);
            drawBmp(scratch);
         break;
         case 6 :
            clearSratch();
            orBMaps(bmpB,bmpC);
            orBMaps(scratch,bmpD);
            drawBmp(scratch);
         break;
         default :drawBmp(bmpE);
      }
   }
}


bool dice::isWaiting(void) { return ourState==waiting; }


void dice::idle(void) {

   switch(ourState) {
      case prestart  :
      case waiting   : break;
      case fuzzing   :
         if (frameTimer.ding()) {
            for (int i=0;i<8;i++) {
               scratch[i]=random(0,256);
            }
            drawBmp(scratch);
            if (stateTimer.ding()) {
               frameTimer.reset();
               ourState = waiting;
            } else {
               frameTimer.stepTime();
            }
         }
      break;
      case showing   :
         if (stateTimer.ding()) {
            clearDie();
            stateTimer.reset();
            ourState = waiting;
         }
      break;   
   }
}
