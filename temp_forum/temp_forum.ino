
#include "blinker.h"
#include "idlers.h"


blinker  firstLED(13,2500,9600);
blinker  secondLED(12,2500,9600);
timeObj  midTimer(7100);          // 2500 to 7100

void setup() {
  
   
   firstLED.setOnOff(true);   // Fire up your first LED
   while(!midTimer.ding()) idle();   // Wait for it to finish it's blink cycle..
   secondLED.setOnOff(true);  // Fire up your second LED.
}

void loop() {
   idle();   // This runs your LEDs in the background. And anything else that's in the background as well.
  
   
   // Do whatever else you want. Just NOT delay();
}
