
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
#include <neoPixel.h>

#define NUM_LEDS 12
 
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


// Shows the change, calls delay etc.
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
    
   if ((inNum<5) || (inNum>=60)) return 11;
   switch(inNum) {
         case 5: return 0;
         case 10: return 1;
         case 15: return 2;
         case 20: return 3;
         case 25: return 4;
         case 30: return 5;
         case 35: return 6;
         case 40: return 7;
         case 45: return 8;
         case 50: return 9;
         case 55: return 10;
      }
      inNum--;
      return sixtyToLED(inNum);
}


    
 void drawTime() {
  
    setColor(background);   // Clear Dial
    ring.setPixelColor(11,&twelveColor);   // Hilight the 12 point
    ring.setPixelColor(hours-1,&hourColor);
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
         case 5: blink(0); break;
         case 10: blink(1); break;
         case 15: blink(2); break;
         case 20: blink(3); break;
         case 25: blink(4); break;
         case 30: blink(5); break;
         case 35: blink(6); break;
         case 40: blink(7); break;
         case 45: blink(8); break;
         case 50: blink(9); break;
         case 55: blink(10); break;
         case 60: blink(11); break;
      }
   }
}
