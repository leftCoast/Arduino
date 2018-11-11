#include <Adafruit_NeoPixel.h>

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

#include <chainPixels.h>
#include <liteLine.h>
#include <neoPixel.h>

#define   NUM_LEDS    120
#define   LED_PIN     3
#define   DELAY_TIME  2
#define   LOW_BLEND   1   // %blue to black
#define   HI_BLEND    75  // %black to white

#define   MAX_COLOR_INDEX 2000
#define   MAX_CHANGE_TIME 50
#define   MAX_NUM_CHANGES 25

neoPixel      lightLine(NUM_LEDS,LED_PIN);
colorMultiMap lightMapper;
timeObj       changeTimer;
colorObj      colorChoice;

void setup() {
  
  lightLine.begin();
  lightLine.setAll(&black);
  lightLine.show();
  lightMapper.addColor(0,&black);
  lightMapper.addColor(400,&red);
  lightMapper.addColor(1900,&yellow);
  lightMapper.addColor(MAX_COLOR_INDEX,&green);
  for (int i=0;i<NUM_LEDS;i++) {
    colorChoice = pickColor();
    lightLine.setPixelColor(i,&colorChoice);
  }
  lightLine.show();
  changeTimer.setTime(random(0,MAX_CHANGE_TIME));
}


colorObj pickColor(void) {
  
    colorObj  randColor;
    
    randColor = lightMapper.Map(random(0,2000));
    randColor.blend(&black,random(0,95));
    randColor.blend(&black,50);
    return randColor;
}


void doChange(void) {

  colorChoice = pickColor();
  lightLine.setPixelColor(random(0,NUM_LEDS),&colorChoice);
}


void loop() {

  int numChanges;
  
  if (changeTimer.ding()) {
    numChanges = random(1,MAX_NUM_CHANGES);
    for (int i=0;i<numChanges;i++) {
      doChange();
    }
    lightLine.show();
    changeTimer.setTime(random(0,MAX_CHANGE_TIME));
  }
}
