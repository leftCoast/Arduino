#include "blinker.h"
#include "idlers.h"


blinker  firstLED(13,1000,2000);
blinker  secondLED(16,330,2000);
timeObj  midTimer(1330);

void setup() {
   
   firstLED.setOnOff(true);   // Fire up your first LED
   while(!midTimer.ding()) idle();   // Wait for it to finish it's blink cycle..
   secondLED.setOnOff(true);  // Fire up your second LED.
}

void loop() {
   idle();   // This runs your LEDs in the background. And anything else that's in the background as well.
   // Do whatever else you want. Just NOT delay();
}
