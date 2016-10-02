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

#define SOUND_PIN1 5
#define SOUND_PIN2 4
#define SOUND_PIN3 3

#define NOTE_TIME 250     // How long to hold a note.

#define MIN_COUNT 1
#define MAX_COUNT 32 // Number of notes 'till we repeat.

voice voice1(SOUND_PIN1,true);
voice voice2(SOUND_PIN2,true);
voice voice3(SOUND_PIN3,true);

timeObj timer(NOTE_TIME);
int count;

void setup() {

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  count = 1;
}

void playNote(void) {

  if (count>=1 && count <=9) { 
    voice1.setUpSound(F5); 
    voice2.setUpSound(D5);
    voice3.setUpSound(A4); 
    voice1.timedPlay(240);
    voice2.timedPlay(240);
    voice3.timedPlay(240);
  }
  if (count>=10 && count <= 16) {
    voice1.setUpSound(E5);
    voice2.setUpSound(C5);
    voice3.setUpSound(A4);
    voice1.timedPlay(240);
    voice2.timedPlay(240);
    voice3.timedPlay(240);
  }
  if (count>=17 && count <= 25) {
    voice1.setUpSound(E5);
    voice2.setUpSound(C5);
    voice3.setUpSound(G4);
    voice1.timedPlay(240);
    voice2.timedPlay(240);
    voice3.timedPlay(240);
  }
  if (count>=26 && count <= 32) {
    voice1.setUpSound(D5);
    voice2.setUpSound(B4);
    voice2.setUpSound(G4);
    voice1.timedPlay(240);
    voice2.timedPlay(240);
    voice3.timedPlay(240);
  }
  /*
   if (count>=30 && count <= 39) {
    voice1.setUpSound(D5);
    voice2.setUpSound(B4);
    voice1.timedPlay(240);
    voice2.timedPlay(240);
  }
  */
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


