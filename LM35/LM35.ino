#include <blinker.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <servo.h>
#include <timeObj.h>

mapper intToVolts(0,1023,0,5.0);
mapper voltsToTemp(0,1.5,0,150);

double centToDegF(double cent) { 
  return (cent * 1.8) + 32; 
}

timeObj printTimer(1000);

void setup() {
  Serial.begin(9600);
  printTimer.start();
}

void loop() {

  if (printTimer.ding()) {
    unsigned int count = analogRead(A0);
    double volts = intToVolts.Map(count);
    double tempC = voltsToTemp.Map(volts);
    double tempF = centToDegF(tempC);
    Serial.print("Count   : ");
    Serial.println(count);
    Serial.print("Volts   : ");
    Serial.println(volts);
    Serial.print("Temp C  : ");
    Serial.println(tempC);
    Serial.print("Temp F  : ");
    Serial.println(tempF);
    Serial.println();
    printTimer.stepTime();
  }
}

