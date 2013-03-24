#include "miniSSC.h"

mapper SSCMapper(miniSSCMinVal,miniSSCMaxVal,0,254);

miniSSC::miniSSC() {

  byte i;
  buff[0] = 255;                // 255 is the flag that a new packet is coming.
  for(i=0;i<NUM_SERVOS;i++)
    lastVal[i] = 255;           // 255 is invalid so this is a good flag..
}


void miniSSC::initSSC(bool fast) {

  if (fast)
    Serial.begin(9600);
  else
    Serial.begin(2400);
}


void miniSSC::setServo(byte servoNum,float inVal) {

  buff[1] = servoNum;                                    // Slam the servo num into the output buffer
  buff[2] = byte(SSCMapper.Map(inVal));                  // Scale the inVal to a byte and pop it in there..
  if (servoNum<NUM_SERVOS && buff[2] != lastVal[servoNum]) {    // Now before spending time to write it out, sanity check all this.
    Serial.flush();                                      // Maybe we need to wait 'till its done?
    Serial.write(buff,3);                                // Everything's ok, write the buffer out.
    lastVal[servoNum] = buff[2];                         // update our last position for this servo.
  }
}

