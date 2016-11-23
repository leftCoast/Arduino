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

#define COLOR_STEP  0.001
#define MAX_COLOR_INDEX 4
#define COLOR_STEP_MS   10

neoPixel theLight(1,3);
colorMultiMap theColors;
float colorIndex;


void setup() {
  theLight.begin();
  theLight.setPixelColor(0,&black);
  theLight.show();

  theColors.addColor(0,&green);
  theColors.addColor(1,&red);
  theColors.addColor(3,&blue);
  theColors.addColor(4,&green);

  colorIndex = 0;
}


void loop() {

  colorObj  aColor;
  aColor = theColors.Map(colorIndex);
  theLight.setPixelColor(0,&aColor);
  theLight.show();
  colorIndex = colorIndex + COLOR_STEP;
  if (colorIndex>MAX_COLOR_INDEX) colorIndex = 0;
  delay(COLOR_STEP_MS);
}
