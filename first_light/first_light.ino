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


class glowLite : public liteLine {
   
   public:
   
   glowLite(Adafruit_NeoPixel* inLites,int inLength);
   ~glowLite(void);
   
   void setMult(float inMult);
   virtual colorObj calcColor(int index,int i);
   
   private:
   
   Adafruit_NeoPixel* lites;
   float              mult;
};
      


 glowLite::glowLite(Adafruit_NeoPixel* inLites,int inLength) 
 
   : liteLine(inLength)
   { lites = inLites; }
   
 
 glowLite::~glowLite(void) {  }
 
 void glowLite::setMult(float inMult) { mult = inMult; }
   
 colorObj glowLite::calcColor(int index,int i) {
   
   uint8_t* colorBuff;
   uint32_t colorValue;
   colorObj aColor;
   byte red = 0;
   byte green = 0;
   byte blue = 0;
   
   colorValue = lites->getPixelColor(index);
   colorBuff = (uint8_t*) &colorValue;
   
   blue = colorBuff[0];
   green = colorBuff[1]; 
   red = colorBuff[2];
   /*
   Serial.print("index : ");Serial.println(index);
   Serial.print("red : ");Serial.println(red);
   Serial.print("green : ");Serial.println(green);
   Serial.print("blue : ");Serial.println(blue);
   Serial.println("Mult : ");Serial.println(mult);
   Serial.println();
   */
   red = red * mult;
   green = green * mult;
   blue = blue * mult;
   /*
   Serial.print("red : ");Serial.println(red);
   Serial.print("green : ");Serial.println(green);
   Serial.print("blue : ");Serial.println(blue);
   Serial.println();
   */
   aColor.setColor(red,green,blue);
   return aColor;
 }
    
#define NUM_LEDS 8
#define LED_PIN 3
#define WAIT 20 
#define LINE_LEN 6

Adafruit_NeoPixel theLights(NUM_LEDS,LED_PIN,NEO_GRB + NEO_KHZ800);

colorObj aColor;
colorObj bColor;

monoColorLine  offLites(LINE_LEN);
multiColorLine lites(LINE_LEN);
glowLite Alen(&theLights,2);

timeObj timer(WAIT);
timeObj AlenTimer(750);

int     liteIndex = 0;
int      alenIndex = 3;

void setup(void) {
   
   //Serial.begin(9600);
   offLites.setColor(BLACK);

   lites.setColor(0,0,5,6);
   lites.setColor(0,0,8,5);
   lites.setColor(0,0,10,4);
   lites.setColor(0,0,27,3);
   lites.setColor(0,0,30,2); 
   lites.setColor(10,10,35,1);
   lites.setColor(20,20,50,0);
   
   theLights.begin();
   Alen.setMult(0);
   timer.start();
   AlenTimer.start();
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
     Alen.setLights(&theLights,alenIndex);
     timer.stepTime();
  }  
  if (AlenTimer.ding()) {
    alenIndex--;
    if (alenIndex<0) {
      alenIndex = NUM_LEDS-1;
    }
    AlenTimer.stepTime();
  } 
}



