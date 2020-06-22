#include "multiMap.h"

#define STEP   1

multiMap aFunction;
float indexx;

void setup() {

   indexx = 0;
   
   aFunction.addPoint(0,0);
   aFunction.addPoint(10,25);
   aFunction.addPoint(100,25);
   aFunction.addPoint(110,0);
   Serial.println(aFunction.Map(5));Serial.flush();
}

void loop() {
   float Fx;
   float integral;
   delay(25);
   Fx = aFunction.Map(indexx);
   integral = aFunction.integrate(0,indexx);
   Serial.print(Fx);Serial.print(" ");
   Serial.println(integral/100);Serial.flush();
   indexx = indexx+STEP;
   if (indexx>110) indexx = 0;
   
}
