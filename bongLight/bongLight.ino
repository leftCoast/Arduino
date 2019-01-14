         
#include <Adafruit_NeoPixel.h>

#include <liteLine.h>
#include <mapper.h>
#include <multiMap.h>
#include <timeObj.h>
#include <colorObj.h>
#include <neoPixel.h>

//#define CALIBRATE // Un-comment to run as a calibration program.


#define NUM_LEDS 12

#define MIN_TIME    25    // Fast
#define MAX_TIME    250   // Slow
#define BLUR_CUTOFF 50    // When we add a blur effect.

#define BACK_TIME   30    // ms between background color changes.
#define BACK_STEPS  200   // Number of steps for backgrond color wash.

#define INPUT_PIN A0

neoPixel ring(NUM_LEDS, 5);

colorObj background;
colorObj slowDot;
colorObj fastDot;
colorObj dotColor;
colorObj tail1;
colorObj tail2;
colorObj tail3;


//mapper inputToTimer(0, 675, MIN_TIME, MAX_TIME);   // history
//mapper inputToTimer(944, 949, MIN_TIME, MAX_TIME); // Uno bong
//mapper inputToTimer(940, 946, MIN_TIME, MAX_TIME);   // Trinket bong 1
mapper inputToTimer(944, 948, MIN_TIME, MAX_TIME);   // Trinket bong 2


mapper inputToLEDMapper(MIN_TIME, MAX_TIME, 0, 100);

colorMapper foregroundDimmer;

colorMultiMap backgroundDimmer;

timeObj drawTimer;
timeObj backTimer;

int     oldTime;
int     count;
int     backCount;


void setup(void) {

  oldTime = -1;
  count = 0;

  #ifdef CALIBRATE
  Serial.begin(9600);
  #endif
  
  ring.begin();

  background.setColor(&blue);
  background = background.mixColors(&black, 99);
  setRingColor(background);
  ring.show();
  backgroundDimmer.addColor(0, &background);
  backgroundDimmer.addColor(BACK_STEPS, &background);
  background.setColor(&blue);
  background = background.mixColors(&black, 80);
  backgroundDimmer.addColor(BACK_STEPS / 2, &background);
  backCount = 0;
  backTimer.setTime(BACK_TIME);


  slowDot.setColor(&blue);
  fastDot.setColor(&red);
  slowDot = slowDot.mixColors(&black, 80);
  fastDot = fastDot.mixColors(&yellow, 10);
  fastDot = fastDot.mixColors(&black, 75);

  foregroundDimmer.setColors(&fastDot, &slowDot);


  drawTimer.start();
}

// Sets in memory, call Show(); to see the change.
void setRingColor(colorObj inColor) {

  for (int i = 0; i <= NUM_LEDS; i++) {
    ring.setPixelColor(i, &inColor);
  }
}


boolean setBackgroundColor(void) {

  if (backTimer.ding()) {
    backTimer.stepTime();
    backCount++;
    if (backCount > BACK_STEPS)
      backCount = 0;
    background = backgroundDimmer.Map(backCount);
    return true;
  }
}


boolean setForegroundColor(void) {

  float newTime;
  float percent;

  newTime = analogRead(INPUT_PIN);
  newTime = inputToTimer.Map(newTime);
  if (drawTimer.ding()) {
    count++;
    if (count > 11) {
      count = 0;
    }
    if (newTime != oldTime) {
      drawTimer.setTime(newTime);
      percent = inputToLEDMapper.Map(newTime);
      dotColor = foregroundDimmer.Map(percent);
      if (newTime == MIN_TIME) dotColor.setColor(&white);
      tail1 = dotColor.mixColors(&background, 40);
      tail2 = dotColor.mixColors(&background, 60);
      tail3 = dotColor.mixColors(&background, 90);
      oldTime = newTime;
      return true;
    } else {
      drawTimer.stepTime();
    }
  }
  return false;
}


void drawRing(void) {

  setRingColor(background);
  if (oldTime < MAX_TIME) {
    ring.setPixelColor(count, &dotColor);
    if (oldTime < BLUR_CUTOFF) {
      if (count == 0) {
        ring.setPixelColor(NUM_LEDS - 1, &tail1);
        ring.setPixelColor(NUM_LEDS - 2, &tail2);
        ring.setPixelColor(NUM_LEDS - 3, &tail3);
      } else if (count == 1) {
        ring.setPixelColor(0, &tail1);
        ring.setPixelColor(NUM_LEDS - 1, &tail2);
        ring.setPixelColor(NUM_LEDS - 2, &tail3);
      } else if (count == 2) {
        ring.setPixelColor(1, &tail1);
        ring.setPixelColor(0, &tail2);
        ring.setPixelColor(NUM_LEDS - 1, &tail3);
      } else {
        ring.setPixelColor(count - 1, &tail1);
        ring.setPixelColor(count - 2, &tail2);
        ring.setPixelColor(count - 3, &tail3);
      }
    }
  }
  ring.show();
}

#ifdef CALIBRATE
void loop(void) {
  float reading = analogRead(INPUT_PIN);
  Serial.println(reading);
}
#endif

#ifndef CALIBRATE
void loop(void) {

  boolean refresh1;
  boolean refresh2;

  refresh1 = setBackgroundColor();
  refresh2 = setForegroundColor();
  if (refresh1 || refresh2) {
    drawRing();
  }
}
#endif
