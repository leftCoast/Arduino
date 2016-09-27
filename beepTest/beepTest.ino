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
#define WAVE_DURATION C4 // How long 'till the next wave. Pitch, kinda'
#define WAVE_ON 0.075     // How long is the wave ON Smaller than duration.

#define NOTE_TIME 250     // How long to hold a note.

#define MIN_COUNT 1
#define MAX_COUNT 15 // Number of notes 'till we repeat.

blinker beep(SOUND_PIN, WAVE_ON, WAVE_DURATION, true);
timeObj timer(NOTE_TIME);
int count;
boolean playing;

void setup() {

  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH); // Pull ups on.
  count = 1;
  beep.setBlink(false);
  playing = false;
}

void playNote(void) {

  switch (count) {
    case 1 : beep.setTimes(WAVE_ON, C3); break;
    case 2 : beep.setTimes(WAVE_ON, D3); break;
    case 3 : beep.setTimes(WAVE_ON, E3); break;
    case 4 : beep.setTimes(WAVE_ON, F3); break;
    case 5 : beep.setTimes(WAVE_ON, G3); break;
    case 6 : beep.setTimes(WAVE_ON, A3); break;
    case 7 : beep.setTimes(WAVE_ON, B3); break;
    case 8 : beep.setTimes(WAVE_ON, C4); break;
    case 9 : beep.setTimes(WAVE_ON, D4); break;
    case 10 : beep.setTimes(WAVE_ON, E4); break;
    case 11 : beep.setTimes(WAVE_ON, F4); break;
    case 12 : beep.setTimes(WAVE_ON, G4); break;
    case 13 : beep.setTimes(WAVE_ON, A4); break;
    case 14 : beep.setTimes(WAVE_ON, B4); break;
    case 15 : beep.setTimes(WAVE_ON, C5); break;
  }
  count++;
  if (count > MAX_COUNT) count = MIN_COUNT;
}


void loop() {
  idle();
  boolean pressed = !digitalRead(BUTTON_PIN);
  if (pressed) {
    if (!playing) {
      beep.setBlink(true);
      playNote();
      timer.start();
      playing = true;
    } else {
      if (timer.ding()) {
        timer.stepTime();
        playNote();
      }
    }
  } else {
    if (playing) {
      beep.setBlink(false);
      playing = false;
    }
  }
}


