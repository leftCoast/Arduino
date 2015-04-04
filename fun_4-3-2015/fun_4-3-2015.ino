#include <Adafruit_NeoPixel.h>

#include <chainPixels.h>
#include <liteLine.h>
#include <neoPixel.h>

#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <runningAvg.h>
#include <servo.h>
#include <timeObj.h>

#define BEEP_ON_MS        .1
#define BEEP_MIN_PEROID   .3
#define BEEP_MAX_PERIOD  4.55

blinker aBlinker(13,BEEP_ON_MS,1,false);
mapper aToBeep(0,1023,BEEP_MIN_PEROID,BEEP_MAX_PERIOD);


timeObj  readTimer(10);
timeObj  writeTimer(1000);

#define SENS1_SWITCH_PIN  1
#define SENS2_SWITCH_PIN  2

neoPixel lightStick(8,3);

void setup() {
  //Serial.begin(9600);
  lightStick.begin();
  aBlinker.setBlink(true);  // This starts the blinking..
  pinMode(SENS1_SWITCH_PIN, INPUT);
  digitalWrite(SENS1_SWITCH_PIN,HIGH);          // Pullups on!
  pinMode(SENS2_SWITCH_PIN, INPUT);
  digitalWrite(SENS2_SWITCH_PIN,HIGH);          // Pullups on!
}


void setPixels(colorObj* color) {
  
  for (int i=0;i<8;i++) {
    lightStick.setPixelColor(i,color);
  }
  lightStick.show();
}


void loop() {
  float periodMs;
  
  idle();
  while(!digitalRead(SENS1_SWITCH_PIN)) {
    digitalWrite(13,HIGH);
    setPixels(&green);
    delay(20);
  }
  setPixels(&black);
  
  while(!digitalRead(SENS2_SWITCH_PIN)) {
    digitalWrite(13,HIGH);
    setPixels(&blue);
    delay(20);
  }
  setPixels(&black);
  
  
  digitalWrite(13,LOW);
  
  if (readTimer.ding()) {
    periodMs = aToBeep.Map(analogRead(0));
    aBlinker.setTimes(BEEP_ON_MS,periodMs);
    readTimer.stepTime();
    /*
    if (writeTimer.ding()) {
      Serial.println(periodMs);
      writeTimer.stepTime();
    }
    */
  }
}
