#include "dice.h"
#include <tapSensor.h>
#include <squareWave.h>
#include <timeObj.h>
#include <mapper.h>

#define  THINK_MS 1000
#define  SHOW_MS  5000
#define  TAP_MIN  20


//Adafruit_8x8matrix1   matrix1;


enum        states { waiting, thinking, showing };
states      ourState;
tapSensor   ourTapSensor(3);
dice        ourDie[2];


void setup(void) {
   
   timeObj  serialTimeout(5000);
   
  Serial.begin(57600);
  while(!Serial&&!serialTimeout.ding());
  Serial.println("Hello?");
  ourDie[0].begin(0x70);
  ourDie[0].clearDie();
  ourDie[1].begin(0x71);
  ourDie[1].clearDie();
  ourTapSensor.begin();
  ourState = waiting;
}



void loop(void) {

   int  num;
   
   idle();
   switch(ourState) {
      case waiting   :
         if (ourTapSensor.getTapVal()>TAP_MIN) {
            ourDie[0].doFuzz(THINK_MS);
            ourDie[1].doFuzz(THINK_MS);
            ourState = thinking;
         }
      break;
      case thinking  :
         if (ourTapSensor.getTapVal()>TAP_MIN) {
            ourDie[0].doFuzz(THINK_MS);
            ourDie[1].doFuzz(THINK_MS);
         } else if (ourDie[0].isWaiting()) {
            num = random(1,7);
            ourDie[0].showNum(num,SHOW_MS);
            ourDie[1].showNum(num,SHOW_MS);
            ourState = showing;
         }
      break;
      case showing   :
         if (ourTapSensor.getTapVal()>TAP_MIN) {
            ourDie[0].doFuzz(THINK_MS);
            ourDie[1].doFuzz(THINK_MS);
            ourState = thinking;
         } else if (ourDie[0].isWaiting()) {
            ourState = waiting;
         }
      break;
   } 
}
