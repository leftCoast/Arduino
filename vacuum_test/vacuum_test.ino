
#include "timeObj.h"
#include "mapper.h"

#define vacummPin A0

timeObj timer(250);

mapper data2voltMapper(0, 1024, 0.0049, 5);
mapper volt2psiMapper(0.2,4.6,-16.7,2.2);
mapper psi2hgMapper(-14.734624,2.2,30,-4.4792455);
//mapper hg2servoMapper(30,0,-100,100);



void setup() { 
  Serial.begin(9600);
  timer.start();
}


float readInHg(void) {

  int sensorValue;
  float volts;
  float psi;

  sensorValue = analogRead(vacummPin);
  volts = data2voltMapper.Map(sensorValue);
  psi = volt2psiMapper.Map(volts);
  return psi2hgMapper.Map(psi);
}

float oldReading;

void loop() {

    float inHg;
    
    if (timer.ding()) {
      timer.stepTime();
      inHg = readInHg();
      Serial.print("inHg : ");
      Serial.print(inHg);
      Serial.print("      leak inHg/sec : ");
      Serial.println((oldReading-inHg)*4);
      oldReading = inHg;
    }
 
}

























