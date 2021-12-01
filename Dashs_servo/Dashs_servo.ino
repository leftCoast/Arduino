#include <blinker.h>
#include <mapper.h>
#include <serialStr.h>


blinker     servo(13,1.5,15);
mapper      servoMapper(-100,100,1,2);
serialStr   ourReader;

void setup() {

   Serial.begin(57600);
   ourReader.setCallback(doServo);
   servo.setOnOff(true);

}

void doServo(const char* inStr) {
   
   float inValue;
   float msValue;
   Serial.println(inStr);
   inValue = atof(inStr);
   msValue = servoMapper.map(inValue);
   Serial.println(inValue);
   Serial.println(msValue);
   servo.setPulse(msValue);
}


void loop() { idle(); }
