#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <runningAvg.h>
#include <timeObj.h>

#include <Adafruit_NeoPixel.h>

#include <chainPixels.h>
#include <liteLine.h>
#include <neoPixel.h>

#include <colorObj.h>

#include <sonarObj.h>
#include <idlers.h>

#define PING_PIN  2
//sonarObj sonar(PING_PIN);      // ONLY PIN 2 or 3 CAN BE USED FOR SONAR.
float distance = -1;    // Flag, it should never be negative.
mapper   distMapper(0, 300, 0, 100);
colorObj startColor(RED);
colorObj endColor(GREEN);
neoPixel theStick(8, 4);
colorMapper distm2colotMapper;
runningAvg smoother(15);
timeObj  timer(10);

void setup() {

  //Serial.begin(9600);
  //sonar.hookup();         // Assembles the idler into the idler list.
  theStick.begin();
  endColor = endColor.blend(&black, 70);
  distm2colotMapper.setColors(&startColor, &endColor);
  timer.start();
}

float ping(void) {

  long duration;

  pinMode(PING_PIN, OUTPUT);
  digitalWrite(PING_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(PING_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(PING_PIN, LOW);

  pinMode(PING_PIN, INPUT);
  duration = pulseIn(PING_PIN, HIGH);
  return duration / 29 / 2;
}


void loop() {

  float temp;
  colorObj result;
  float normalized;

  idle();                           // Lets all idlers do their things.
  if (timer.ding()) {
    temp = ping();
    //temp = sonar.distanceCm();        // Grab the distance.
    temp = smoother.addData(temp);    // doing the running avarage thing.
    //Serial.print("distance cm : "); Serial.println(distance);   // Show it.
    normalized = distMapper.Map(temp);
    //Serial.print("Normalized : "); Serial.println(normalized);   // Show it.
    result = distm2colotMapper.Map(normalized);
    //result.printRGB();
    for (int i = 0; i < 8; i++) {
      theStick.setPixelColor(i, &result);
    }
    theStick.show();
    timer.stepTime();
  }
}


