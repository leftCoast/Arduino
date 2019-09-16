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

#include "notes.h"

#define BUTTON_PIN 12
#define ANALOG_PIN A0

#define SOUND_PIN1 23///[ '0023//5
#define SOUND_PIN2 4
#define SOUND_PIN3 3

//#define NOTE_TIME 250     // How long to hold a note.

#define MIN_COUNT 0
#define MAX_COUNT 12 // Number of notes 'till we repeat.

voice voice1(SOUND_PIN1,true);
//voice voice2(SOUND_PIN2,true);
//voice voice3(SOUND_PIN3,true);


*** Look at newbeep for the tone() stuff for teensy. Also the info to change to stadard freq. values. ***


int count;

float ringTone[] =          { E5,   D5,   F4,   G4,   C5,   B5,   D4,   E4,   B5,   A5,   C4,   E4,   A5, 0 };
int ringToneDurations[] = { 250,  250,  500,  500,  250,  250,  500,  500,  250,  250,  500,  500,  1000, 1000 };

void setup() {

  count = MIN_COUNT;
}

void playNote(void) {

  voice1.setUpSound(ringTone[count]);
  voice1.timedPlay((ringToneDurations[count]/2));
  count++;
  if (count > MAX_COUNT) {
    count = MIN_COUNT;
    delay(1000);
  }
}

void hammer(float freq,float duration) {

  float onTime;
  
  onTime = freq/2.0;
  timeObj onTimer(onTime);
  timeObj freqTime(freq);
  timeObj durationTime(duration);
  durationTime.start();
  Serial.print("Freq : ");
  Serial.print(freq);
  Serial.print("\tonTime ");
  Serial.print(onTime);
  Serial.println();
  while(!durationTime.ding()) {
    onTimer.start();
    freqTime.start();
    digitalWrite(SOUND_PIN1,LOW);
    while(!freqTime.ding()) {
      if (onTimer.ding()) {
        digitalWrite(SOUND_PIN1,HIGH);
      }
    }
  } 
}


void loop() {

  idle();
  if (!voice1.isPlaying()) {
    playNote();
  }
}
