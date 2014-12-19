#include <SoftwareSerial.h>
#include <blinker.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <servo.h>
#include <timeObj.h>
#include "colorObj.h"
#include "liteLine.h"

#include <Adafruit_NeoPixel.h>



      
// ******************************************
#define DEF_FLASH_TIME 100

/*
class flash public idler, timeObj {
   
   public:
   
   flash(Adafruit_NeoPixel* inLites);
   ~flash(void);
   
   virtual void trigger(int index,float inMs);
   virtual void idle(void) {
      
      
   private :
 
   Adafruit_NeoPixel* theLites;
};


 flash::flash(Adafruit_NeoPixel* inLites) { theLites = inLites; }
 
 flash::~flash(void) {  }
 
   
 void flash::trigger(int index,float inMs) {
    
    index = inIndex;
    setTime(inMs);
 }
    
    
 void flash::idle(void) {
    
      if (!ding()) {
         theLights.setPixelColor(index,255,255,255);
         theLights.show();
      }
 }
 */  
   
   
   
#define NUM_LEDS 8
#define LED_PIN 3
#define WAIT 20 
#define LINE_LEN 6

Adafruit_NeoPixel theLights(NUM_LEDS,LED_PIN,NEO_GRB + NEO_KHZ800);

colorObj aColor;
colorObj bColor;

monoColorLine  offLites(LINE_LEN);
multiColorLine lites(LINE_LEN);

timeObj timer(WAIT);
int     liteIndex = 0;

void setup(void) {
   
   //Serial.begin(9600);
   offLites.setColor(BLACK);
      
   //lites.setColor(0,0,1,10);
   //lites.setColor(0,0,1,9);
   //lites.setColor(0,0,2,8);
   //lites.setColor(0,0,3,7);
   lites.setColor(0,0,5,6);
   lites.setColor(0,0,8,5);
   lites.setColor(0,0,10,4);
   lites.setColor(0,0,27,3);
   lites.setColor(0,0,30,2); 
   lites.setColor(10,10,35,1);
   lites.setColor(20,20,50,0);
   
   theLights.begin();
   timer.start();
   clearLights();
}


void clearLights(void) { setStick(BLACK); }
   
   
void swashStick(colorObj startColor, colorObj endColor) {

   colorMapper aCMapper(&startColor,&endColor);
   for (int i=0;i<101;i++) {
      setStick(aCMapper.Map(i));
      delay(WAIT);
   } 
}


void setStick(colorObj color) {

   for(int i=0;i<NUM_LEDS;i++) {
      theLights.setPixelColor(i, color.getRed(), color.getGreen(), color.getBlue());
   }
   theLights.show();
}


void loop(void) {
   
  if (timer.ding()) {
     offLites.setLights(&theLights,liteIndex,false );
     liteIndex++;
     if (liteIndex>=NUM_LEDS+LINE_LEN) {
        liteIndex = 0;
     }
     lites.setLights(&theLights,liteIndex,false);
     timer.stepTime();
  }  
}



