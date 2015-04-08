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

#include "shakeNTinkle.h"
#include "sparkle.h"

#define MAX_SPARKLES  2
#define MIN_SHAKE     30
#define MIN_TINKLE    20

blinker aBlinker;

//timeObj  readTimer(10);
//timeObj  writeTimer(1000);
timeObj  lightTimer(5);

#define SENS1_SWITCH_PIN  2
#define SENS2_SWITCH_PIN  3

shakeNTinkle sensors(SENS1_SWITCH_PIN,SENS2_SWITCH_PIN);

neoPixel lightStick(8, 4);

sparkleList sparkles;

colorMultiMap  cMap;
int MapCount = 0;

void setup() {
  //Serial.begin(9600);
  lightStick.begin();
  //aBlinker.setBlink(true);  // This starts the blinking..
  sensors.begin();          // Sets up & starts the sensors.
  sparkles.hookup();        // Starts garbage collection on the sparkle list.
  
  cMap.addColor(0,&green);
  cMap.addColor(100,&blue);
  cMap.addColor(50,&magenta);
  cMap.addColor(150,&red);
  cMap.addColor(175,&yellow);
  cMap.addColor(200,&cyan);
  cMap.addColor(250,&green);
}


void setPixels(colorObj* color) {

  for (int i = 0; i < 8; i++) {
    lightStick.setPixelColor(i, color);
  }
  lightStick.show();
}


void createSparkle(float shake,float tinkle) {
  
  int      index;
  colorObj color;
  float    onTime;
  float dormantTime;
  float buildTime;
  float decayTime;
 
  if ((sparkles.numSparkles()<MAX_SPARKLES)&&(shake>MIN_SHAKE)) {
    index = random(0,8);
    color = white;
    onTime = 100;
    buildTime = 0;
    decayTime = 250;
    if (tinkle>MIN_TINKLE) {
      dormantTime = random(0,1000);
    } else {
      dormantTime = random(0,100);
    }
    sparkle* newSparkle = new sparkle(index,&color,onTime,dormantTime,buildTime,decayTime);
    sparkles.addToTop(newSparkle);
  }
}


void doSparkles(void) {
  
  int      index;
  colorObj backgroundColor;
  colorObj result;
  sparkle* sparklePtr;
  
  sparklePtr = sparkles.getSparkles();
  while(sparklePtr) {
    index = sparklePtr->getIndex();
    backgroundColor = lightStick.getPixelColor(index);
    result = sparklePtr->getColor(&backgroundColor);
    lightStick.setPixelColor(index, &result);
    sparklePtr = sparklePtr->getNext();
  }
  lightStick.show();
}


void loop() {
  float periodMs;

  idle();
  if (lightTimer.ding()) {
    createSparkle(sensors.getShake(),sensors.getTinkle());
    colorObj result = cMap.Map(MapCount++);
    result = result.blend(&black,90);
    setPixels(&result);
    if (MapCount>=250) MapCount=0;
    doSparkles();
    lightTimer.stepTime();
  }
}

