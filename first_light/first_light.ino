#include <blinker.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <RCReciver.h>
#include <servo.h>
#include <timeObj.h>
#include <colorObj.h>

#include <PinChangeInt.h>

//#include <SoftwareSerial.h>


#include "liteLine.h"
#include "RCReciver.h"

#include <Adafruit_NeoPixel.h>




// ******************************************
#define DEF_FLASH_TIME 100


class glowLite : 
public liteLine {

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

: 
liteLine(inLength)
{ 
   lites = inLites; 
}


glowLite::~glowLite(void) {  
}

void glowLite::setMult(float inMult) { 
   mult = inMult; 
}

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


// ******************* start the "real" code here.  ******************* 


#define NUM_LEDS 8
#define HALF_NUM_LEDS 4
#define LED_PIN 3
#define WAIT 33 
#define LINE_LEN 6

#define MAX_RC       1900
#define MID_RC       1500
#define MIN_RC       1100

#define THROTTLE_PIN 4
#define ROLL_PIN     5
#define PITCH_PIN    6
#define YAW_PIN      8

#define MIN_ROLL        50    // Percent value drop at full roll
#define MIN_ROLL_CUTOFF 47
#define MAX_ROLL_CUTOFF 53

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

reciverPin throttle(THROTTLE_PIN);
reciverPin roll(ROLL_PIN);
reciverPin yaw(YAW_PIN);
reciverPin pitch(PITCH_PIN);

mapper pinToPercent(MIN_RC,MAX_RC,0,100);

multiMap pinToAmount;
//pinToAmount.addPoint(MIN_RC,0);
//pinToAmount.addPoint(MID_RC,1);
//pinToAmount.addPoint(MAX_RC,0);

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


void clearLights(void) { 
   setStick(BLACK); 
}


void setStick(colorObj color) {

   for(int i=0;i<NUM_LEDS;i++) {
      theLights.setPixelColor(i, color.getRed(), color.getGreen(), color.getBlue());
   }
   theLights.show();
}


void loop(void) {

   /*
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
    */
   if (timer.ding()) {


      colorMapper pitchMapper(RED,GREEN);
      colorObj pitchColor(pitchMapper.Map(pinToPercent.Map(pitch.pinResult())));

      colorObj black(BLACK);
      colorMapper throttleMapper(&black,&pitchColor);

      colorObj pitchNThrottleColor(throttleMapper.Map(pinToPercent.Map(throttle.pinResult())));

      float yawVal = pinToPercent.Map(yaw.pinResult());
      colorMapper rollMapper(&pitchNThrottleColor,&black);
      colorObj darkSide(BLACK);
      float rollVal = pinToPercent.Map(roll.pinResult());
      rollVal = (rollVal+yawVal)/2;
      if ((rollVal<50 && rollVal>MIN_ROLL_CUTOFF)||(rollVal>50 && rollVal<MAX_ROLL_CUTOFF)) {
         for(int i=0;i<NUM_LEDS;i++) {
            theLights.setPixelColor(i, pitchNThrottleColor.getRed(),pitchNThrottleColor.getGreen(),pitchNThrottleColor.getBlue());
         }
      } else if (rollVal>50) {
         mapper rollAmount(50,100,MIN_ROLL,100);
         darkSide = rollMapper.Map(rollAmount.Map(rollVal));
          for(int i=0;i<HALF_NUM_LEDS;i++) {
            theLights.setPixelColor(i, darkSide.getRed(),darkSide.getGreen(),darkSide.getBlue());
         }
        
         for (int i=HALF_NUM_LEDS;i<NUM_LEDS;i++) {
           theLights.setPixelColor(i, pitchNThrottleColor.getRed(),pitchNThrottleColor.getGreen(),pitchNThrottleColor.getBlue());
         }
      } else {
         mapper rollAmount(50,0,MIN_ROLL,100);
         darkSide = rollMapper.Map(rollAmount.Map(rollVal));
         for(int i=0;i<HALF_NUM_LEDS;i++) {
            theLights.setPixelColor(i, pitchNThrottleColor.getRed(),pitchNThrottleColor.getGreen(),pitchNThrottleColor.getBlue());
         }
         
         for (int i=HALF_NUM_LEDS;i<NUM_LEDS;i++) {
           theLights.setPixelColor(i, darkSide.getRed(),darkSide.getGreen(),darkSide.getBlue());
         }   
      }
     
       
      theLights.show();
      timer.stepTime();
   }
}




