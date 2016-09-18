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

#define BUTTON_PIN 12
#define ANALOG_PIN A0

#define SOUND_PIN 3
#define WAVE_DURATION 1 // How long 'till the next wave. Pitch, kinda'
#define WAVE_ON 0.1     // How long is the wave ON Smaller than duration.

#define NOTE_TIME 20     // How long to hold a note.

#define MIN_COUNT 0
#define MAX_COUNT 9 // Number of notes 'till we repeat.

blinker beep(SOUND_PIN,WAVE_ON,WAVE_DURATION,true);
timeObj timer(NOTE_TIME);
int count;
mapper  toneMap(MIN_COUNT,MAX_COUNT,.5,2);
float analogVal;
mapper  analogToCountMap(0,670,.5,2);

void setup() {
  
  //Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH); // Pull ups on.
  count = 0;
}

void loop() {
  idle();
  beep.setBlink(!digitalRead(BUTTON_PIN));
  analogVal = analogRead(ANALOG_PIN);
  //Serial.println(analogVal);
  if (timer.ding()) {
    timer.stepTime();
    //beep.setTimes(WAVE_ON,toneMap.Map(count));
    beep.setTimes(WAVE_ON,analogToCountMap.Map(analogVal));
    count++;
    if (count>MAX_COUNT) count = MIN_COUNT;
  }
}
