#include <blinker.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <servo.h>
#include <timeObj.h>

#define OFF_TIME_MIN 200
#define OFF_TIME_MAX 250
#define ON_TIM_MIN  250
#define ON_TIME_MAX 500

blinker* bList[19];

blinker blinker1(A8,random(OFF_TIME_MIN,OFF_TIME_MAX),random(ON_TIM_MIN,ON_TIME_MAX),true);
blinker blinker2(A9,random(OFF_TIME_MIN,OFF_TIME_MAX),random(ON_TIM_MIN,ON_TIME_MAX),true);
blinker blinker3(A10,random(OFF_TIME_MIN,OFF_TIME_MAX),random(ON_TIM_MIN,ON_TIME_MAX),true);
blinker blinker4(A11,random(OFF_TIME_MIN,OFF_TIME_MAX),random(ON_TIM_MIN,ON_TIME_MAX),true);
blinker blinker5(A12,random(OFF_TIME_MIN,OFF_TIME_MAX),random(ON_TIM_MIN,ON_TIME_MAX),true);
blinker blinker6(A13,random(OFF_TIME_MIN,OFF_TIME_MAX),random(ON_TIM_MIN,ON_TIME_MAX),true);
blinker blinker7(A14,random(OFF_TIME_MIN,OFF_TIME_MAX),random(ON_TIM_MIN,ON_TIME_MAX),true);
blinker blinker8(A15,random(OFF_TIME_MIN,OFF_TIME_MAX),random(ON_TIM_MIN,ON_TIME_MAX),true);
blinker blinker9(2,random(OFF_TIME_MIN,OFF_TIME_MAX),random(ON_TIM_MIN,ON_TIME_MAX),true);
blinker blinker10(3,random(OFF_TIME_MIN,OFF_TIME_MAX),random(ON_TIM_MIN,ON_TIME_MAX),true);
blinker blinker11(5,random(OFF_TIME_MIN,OFF_TIME_MAX),random(ON_TIM_MIN,ON_TIME_MAX),true);
blinker blinker12(6,random(OFF_TIME_MIN,OFF_TIME_MAX),random(ON_TIM_MIN,ON_TIME_MAX),true); 
blinker blinker13(44,random(OFF_TIME_MIN,OFF_TIME_MAX),random(ON_TIM_MIN,ON_TIME_MAX),true);
blinker blinker14(45,random(OFF_TIME_MIN,OFF_TIME_MAX),random(ON_TIM_MIN,ON_TIME_MAX),true);
blinker blinker15(46,random(OFF_TIME_MIN,OFF_TIME_MAX),random(ON_TIM_MIN,ON_TIME_MAX),true);
blinker blinker16(10,random(OFF_TIME_MIN,OFF_TIME_MAX),random(ON_TIM_MIN,ON_TIME_MAX),true); 
blinker blinker17(9,random(OFF_TIME_MIN,OFF_TIME_MAX),random(ON_TIM_MIN,ON_TIME_MAX),true);
blinker blinker18(8,random(OFF_TIME_MIN,OFF_TIME_MAX),random(ON_TIM_MIN,ON_TIME_MAX),true);
blinker blinker19(7,random(OFF_TIME_MIN,OFF_TIME_MAX),random(ON_TIM_MIN,ON_TIME_MAX),true);




void setTimes() {

  bList[random(0,19)]->setTimes(random(OFF_TIME_MIN,OFF_TIME_MAX),random(ON_TIM_MIN,ON_TIME_MAX));
 
}


void setup() {

  bList[0] = &blinker1;
  bList[1] = &blinker2;
  bList[2] = &blinker3;
  bList[3] = &blinker4;
  bList[4] = &blinker5;
  bList[5] = &blinker6;
  bList[6] = &blinker7;
  bList[7] = &blinker8;
  bList[8] = &blinker9;
  bList[9] = &blinker10;
  bList[10] = &blinker11;
  bList[11] = &blinker12;
  bList[12] = &blinker13;
  bList[13] = &blinker14;
  bList[14] = &blinker15;
  bList[15] = &blinker16;
  bList[16] = &blinker17;
  bList[17] = &blinker18;
  bList[18] = &blinker19;

  for (int i=0;i<=18;i++) {
    bList[i]->setBlink(true);
  }
}

void pattern() {
  
   for (int i=0;i<=18;i++) {
      bList[i]->setLight(false);
   }
   delay(60);
   for (int i=0;i<=18;i++) {
      bList[i]->setLight(true);
      delay(60);
      bList[i]->setLight(false);
   }
   for (int i=0;i<=18;i++) {
    bList[i]->setBlink(true);
  }
}

void loop() {
  idle();
  if (random(1,5000)==12) setTimes();
  if (random(1,50000)==12) pattern();
}


