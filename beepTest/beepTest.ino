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

#define SOUND_PIN 5

#define NOTE_TIME 250     // How long to hold a note.

#define MIN_COUNT 1
#define MAX_COUNT 40 // Number of notes 'till we repeat.

voice voice1(SOUND_PIN,true);
timeObj timer(NOTE_TIME);
int count;

void setup() {

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  count = 1;
}

void playNote(void) {

  if (count>=1 && count <=9) { voice1.setUpSound(F4); voice1.timedPlay(240);}
  if (count>=10 && count <= 19) { voice1.setUpSound(E4); voice1.timedPlay(240);}
  if (count>=20 && count <= 29) { voice1.setUpSound(D4); voice1.timedPlay(240);}

  count++;
  if (count > MAX_COUNT) count = MIN_COUNT;
}


void loop() {

  idle();
  if (timer.ding()) {
    timer.stepTime();
    playNote();
  }
}


