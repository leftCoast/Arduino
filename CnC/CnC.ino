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

// *******************************************
// *****   light ring stuff for display  *****

#define NUM_LEDS 12
 
neoPixel ring(NUM_LEDS,3);
colorObj black(BLACK);
// *******************************************

enum state { running, calibStick, calibPetals };
state currState = running;

multiMap rollMapper;
multiMap pitchMapper;
multiMap yawMapper;
mapper throttleMapper(0,MIN_PULSE,1023,MAX_PULSE);

int rollVal;
int pitchVal;
int yawVal;
int throttleVal;

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

   Serial.begin(9600);
   setupMappers();
   ring.begin();
   currState = running;
    for (int i=0;i<=NUM_LEDS;i++) {
       ring.setPixelColor(i,&black);
   }
   ring.show();
}



void runLoop(void) {
   
   rollVal = analogRead(ROLL_PIN);
   pitchVal = analogRead(PITCH_PIN);
   yawVal = analogRead(YAW_PIN);
   throttleVal = analogRead(THROTTLE_PIN);
    Serial.print("sensor = " );                       
  Serial.println(rollVal);  
   colorObj green(GREEN);
   colorObj red(RED);
   mapper roll2light(0,1024,TRANSPARENT,OPAQUE);
   colorObj result = green.blend(&red,roll2light.Map(rollVal));
   for (int i=0;i<=NUM_LEDS;i++) {
       ring.setPixelColor(i,&result);
   }
   ring.show();
}


void loop() {

   idle();
   //runLoop();
   switch (currState) {
      case running : runLoop(); break;
      //calibStick : calStickLoop(); break;
      //calibPetals : calPetalsLoop(); break;
   }
}

