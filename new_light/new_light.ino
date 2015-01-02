
#include <Adafruit_NeoPixel.h>

#include <liteLine.h>
#include <blinker.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <servo.h>
#include <timeObj.h>
#include <colorObj.h>

#define NUM_LEDS 12

class neoPixel : public Adafruit_NeoPixel {
   
      public:

  // Constructor: number of LEDs, pin number, LED type. See the ADAfruit_NeoPixel.h for info on these things.
  neoPixel(uint16_t n, uint8_t p=6, uint8_t t=NEO_GRB + NEO_KHZ800);
  ~neoPixel();
  void setPixelColor(uint16_t n,colorObj* inColor);
  colorObj getPixelColor(uint16_t n);
};


 neoPixel::neoPixel(uint16_t n, uint8_t p, uint8_t t)
   :  Adafruit_NeoPixel(n,p,t) {  }
   
 neoPixel::~neoPixel() {  }


void neoPixel::setPixelColor(uint16_t n,colorObj* inColor) { 
    
    Adafruit_NeoPixel::setPixelColor(n,inColor->getRed(),inColor->getGreen(),inColor->getBlue());
 }
  
  
colorObj neoPixel::getPixelColor(uint16_t n) {
   
   uint32_t packColor;
   uint8_t*    colorPtr;
   
   packColor = Adafruit_NeoPixel::getPixelColor(n);
   colorPtr = (uint8_t*) &packColor;
   colorObj result(colorPtr[2],colorPtr[1],colorPtr[0]);
   return result;
}


neoPixel ring(NUM_LEDS,3);

//colorObj black(BLACK);
colorObj background(2,2,2);
colorObj secColor(10, 60, 40);
colorObj minuteColor(60,20,20);
colorObj hourColor(20,40,00);
colorObj twelveColor(20,20,20);

//mapper sixtyLEDmapper(5,60,);
//colorMapper foregroundDimmer(&black, &foreground);   
   

int hours = 7;
int minutes = 20;
int secs = 56;

timeObj secTimer(1000);

void setup(void) {
   
  ring.begin();
  setColor(background);
  ring.setPixelColor(0,&twelveColor);   // Hilight the 12 point
  ring.show();
  secTimer.start();
}

// Sets in memory, call Show() to see the change.
void setColor(colorObj inColor) {
   
   for (int i=0;i<=NUM_LEDS;i++) {
       ring.setPixelColor(i,&inColor);
   }
}


// Shows the change, cals delay etc.
void blink(int num) {
   
    colorObj background = ring.getPixelColor(num);
    colorObj resultColor;
    
    for (int i=50;i>1;i--) {
       resultColor = secColor .blend(&background,i*2);
       ring.setPixelColor(num,&resultColor);
       ring.show();
       delay(98);
    }
    ring.setPixelColor(num,&background);
    ring.show();
}
 
word sixtyToLED(int inNum) {
    
   if (inNum<0) {
      return 0;
   }
   switch(inNum) {
         case 60: return 0;
         case 5: return 1;
         case 10: return 2;
         case 15: return 3;
         case 20: return 4;
         case 25: return 5;
         case 30: return 6;
         case 35: return 7;
         case 40: return 8;
         case 45: return 9;
         case 50: return 10;
         case 55: return 11;
      }
      inNum--;
      return sixtyToLED(inNum);
}


    
 void drawTime() {
  
    setColor(background);   // Clear Dial
    ring.setPixelColor(0,&twelveColor);   // Hilight the 12 point
    ring.setPixelColor(hours,&hourColor);
    ring.setPixelColor(sixtyToLED(minutes),&minuteColor);
    ring.show();
 }
 
    
void loop(void) {
   
   if (secTimer.ding()) {
      secs++;
      if (secs>=61) {
         secs = 1;
         minutes++;
         if (minutes>=61){
            minutes = 1;
            hours++;
            if (hours>=13) {
               hours = 1;
            }
         }
         drawTime();
      }
      secTimer.stepTime();
      switch(secs) {
         case 60: blink(0); break;
         case 5: blink(1); break;
         case 10: blink(2); break;
         case 15: blink(3); break;
         case 20: blink(4); break;
         case 25: blink(5); break;
         case 30: blink(6); break;
         case 35: blink(7); break;
         case 40: blink(8); break;
         case 45: blink(9); break;
         case 50: blink(10); break;
         case 55: blink(11); break;
      }
   }
}
