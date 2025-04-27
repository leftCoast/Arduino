
#include "GPxxx.h"

GPSInStr inPort;
GPVTG    vOverGound;   

void setup() {
   Serial.begin(9600);
   delay(100);
   Serial1.begin(9600);
   delay(100);
   inPort.addHandler(&vOverGound);
}


void loop() {
   /*
   char  aChar;

   if (Serial1.available()) {    // If the GPS sent a char..
      aChar = Serial1.read();    // Read and save it.
      Serial.print(aChar);       // Echo it back to the serial monitor.
   }
   */
   idle();
}
