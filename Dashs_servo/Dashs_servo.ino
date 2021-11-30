#include <blinker.h>
#include <mapper.h>


blinker servo(13,1.5,15);
mapper  servoMapper(1,2,-100,100);


void setup() {
   servo.setOnOff(true);

}


void loop() {

   float inValue;
   
   idle();
   servo.setPulse(servoMapper.map(inValue));
}
