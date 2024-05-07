#include "dice.h"
#include <tapSensor.h>

#define  THINK_MS 1000
#define  SHOW_MS  5000
#define  TAP_MIN  20



enum        states { waiting, thinking, showing };
states      ourState;
tapSensor   ourTapSensor(3);
dice        ourDie;


void setup(void) {
   
  Serial.begin(9600);
  ourDie.begin(0x70);
  ourDie.clearDie();
  ourTapSensor.begin();
  ourState = waiting;
}



void loop(void) {

   idle();
   switch(ourState) {
      case waiting   :
         if (ourTapSensor.getTapVal()>TAP_MIN) {
            ourDie.doFuzz(THINK_MS);
            ourState = thinking;
         }
      break;
      case thinking  :
         if (ourTapSensor.getTapVal()>TAP_MIN) {
            ourDie.doFuzz(THINK_MS);
         } else if (ourDie.isWaiting()) {
            ourDie.showNum(random(1,7),SHOW_MS);
            ourState = showing;
         }
      break;
      case showing   :
         if (ourDie.isWaiting()) {
            ourDie.clearDie();
            ourState = waiting;
         }
      break;
   }
}
