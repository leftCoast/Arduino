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

#define   ANALOG_PIN  A0
#define   ANALOG_MIN  3.5
#define   ANALOG_MAX  8
#define   DELTA_SAMPLES  10
#define   SMOOTH_SAMPLES  10


neoPixel    lightLine(NUM_LEDS,LED_PIN);
timeObj     delayTimer(DELAY_TIME);
mapper      analogMapper(ANALOG_MIN,ANALOG_MAX,0,100);
colorMapper lightMapper;
runningAvg  deltas(DELTA_SAMPLES);
runningAvg  smoother(SMOOTH_SAMPLES);

void setup() {
  
  colorObj  aColor;
  colorObj  bColor;
  
  Serial.begin(57600);
  lightLine.begin();
  lightLine.setAll(&black);
  lightLine.show();
  delayTimer.start();
  aColor.setColor(&black);
  aColor.blend(&blue,LOW_BLEND);
  bColor.setColor(&green);
  bColor.blend(&black,HI_BLEND);
  lightMapper.setColors(&aColor,&bColor);
}



void loop() {

  int       inVal;
  float     percent;
  colorObj  nextColor;
  
  deltas.addData(analogRead(ANALOG_PIN));
  inVal = smoother.addData(deltas.getDelta());
  if (delayTimer.ding()) {
    Serial.println(log(inVal));
    percent = analogMapper.Map(log(inVal));
    nextColor = lightMapper.Map(percent);
    lightLine.shiftPixels();
    lightLine.setPixelColor(0,&nextColor);
    lightLine.show();
    delayTimer.stepTime();
  }
}
