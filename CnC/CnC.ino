#include <colorObj.h>
#include <liteLine.h>
#include <neoPixel.h>

#include <Adafruit_NeoPixel.h>

#include <blinker.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <servo.h>
#include <timeObj.h>

#define MIN_PULSE 1000
#define MID_PULSE 1500
#define MAX_PULSE 2000

#define ROLL_PIN     A0
#define PITCH_PIN    A1
#define YAW_PIN      A2
#define THROTTLE_PIN A3
#define AUX_PIN      A4


#define IDLE_STATE   HIGH
#define ACTIVE_STATE LOW
#define TIMING_PULSE 200         // microseconds.
#define OUT_PIN      0
#define REFRESH_TIME 22          // miliseconds  mili NOT micro!

#define NUM_CHANNELS 5
#define THROTTLE     0
#define AILERON      1
#define ELEVAOTOR    2
#define RUDDER       3
#define AUX          4

int     channels[NUM_CHANNELS]; // Also in microseconds.


// *******************************************
// *****   light ring stuff for display  *****

#define NUM_LEDS 12

neoPixel ring(NUM_LEDS,3);
colorObj black(BLACK);
// *******************************************

enum state { 
  running, calibStick, calibPetals };
state currState = running;

multiMap rollMapper;
multiMap pitchMapper;
multiMap yawMapper;
mapper throttleMapper(0,MIN_PULSE,1023,MAX_PULSE);
mapper auxMapper(0,MIN_PULSE,1023,MAX_PULSE);

timeObj outTimer(REFRESH_TIME);

void setupMappers(void) {

  rollMapper.addPoint(0,MIN_PULSE);
  rollMapper.addPoint(512,MID_PULSE);
  rollMapper.addPoint(1023,MAX_PULSE);

  pitchMapper.addPoint(0,MIN_PULSE);
  pitchMapper.addPoint(512,MID_PULSE);
  pitchMapper.addPoint(1023,MAX_PULSE);

  yawMapper.addPoint(0,MIN_PULSE);
  yawMapper.addPoint(512,MID_PULSE);
  yawMapper.addPoint(1023,MAX_PULSE);
}


void setup(void) {

  //Serial.begin(9600);
  setupMappers();
  for (int i=0;i<NUM_CHANNELS;i++) { 
    channels[i] = MID_PULSE;
  }
  pinMode(OUT_PIN, OUTPUT);     
  digitalWrite(OUT_PIN, IDLE_STATE);

  ring.begin();
  for (int i=0;i<=NUM_LEDS;i++) {
    ring.setPixelColor(i,&black);
  }
  ring.show();
  outTimer.start();
  currState = running;
}


void readValues(void) {

  channels[THROTTLE] = throttleMapper.Map(analogRead(THROTTLE_PIN));
  channels[AILERON] = rollMapper.Map(analogRead(ROLL_PIN));
  channels[ELEVAOTOR] = pitchMapper.Map(analogRead(PITCH_PIN));
  channels[RUDDER] = yawMapper.Map(analogRead(YAW_PIN));
  channels[AUX] = auxMapper.Map(analogRead(AUX_PIN));
}


void sendPulse(int* pulseList,byte numChannels) {

  for (int i=0;i<numChannels;i++) {
    digitalWrite(OUT_PIN, ACTIVE_STATE);     // Wake up!
    delayMicroseconds(TIMING_PULSE);               
    digitalWrite(OUT_PIN, IDLE_STATE);       // Now listen!
    delayMicroseconds(pulseList[i]);         // time this!
  }
  digitalWrite(OUT_PIN, ACTIVE_STATE);       // end pulse.
  delayMicroseconds(TIMING_PULSE);               
  digitalWrite(OUT_PIN, IDLE_STATE);
}


void runLoop(void) {

  if (outTimer.ding()) {
    sendPulse(channels,NUM_CHANNELS);
    readValues();
    colorObj green(GREEN);
    green = green.blend(&black,20);
    colorObj red(RED);
    red = red.blend(&black,20);
    mapper roll2light(MIN_PULSE,MAX_PULSE,TRANSPARENT,OPAQUE);
    colorObj result = green.blend(&red,roll2light.Map(channels[AILERON]));
    for (int i=0;i<=NUM_LEDS;i++) {
      ring.setPixelColor(i,&result);
    }
    ring.show();
    outTimer.stepTime();
  }
}


void loop() {

  idle();
  switch (currState) {
    case running : runLoop(); break;
    //calibStick : calStickLoop(); break;
    //calibPetals : calPetalsLoop(); break;
  }
}


