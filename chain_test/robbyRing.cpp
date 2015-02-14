#include "robbyRing.h"

#define DARK_BLEND     95
#define LIGHT_BLEND    20
#define PERCENT_RED    15
#define PERCENT_GREEN  5

#define MAX_MS       3000
#define SLICE_MS     25

#define STARTUP_MS   8000

boolean        sharedInit = false;
colorMapper    lightMapper;
mapper         startMapper(0, STARTUP_MS, 100, 0);
mapper         trigMapper(0, 0, 0, 0);
unsigned long  startMS;


// To set up the shared robbyRing resources. If we are cloned.
void ringSetup(void) {

  //Serial.println("Calling shared init.");
  
  colorObj lightBlue = blue.blend(&red, PERCENT_RED);
  lightBlue = lightBlue.blend(&green, 5);

  colorObj darkBlue = blue.blend(&black, DARK_BLEND);
  lightBlue = lightBlue.blend(&black, LIGHT_BLEND);

  lightMapper.setColors(&darkBlue, &lightBlue);
  trigMapper.setValues(0, MAX_MS, 0, 2 * 3.1416);

  startMS = millis();
  sharedInit = true;
}


robbyRing::robbyRing(word  numLEDs)
  : pixelGroup(numLEDs) {

  currentSlice; currentSlice = 0;
  lightTimer.setTime(SLICE_MS);
  lightTimer.start();
  ourInit = false;
}


robbyRing::~robbyRing(void) { }


float robbyRing::calculatePercent(void) {

  float percent;
  float value;

  if (currentSlice >= MAX_MS) {
    currentSlice = 0;
  }
  value = trigMapper.Map(currentSlice);
  percent = (sin(value) + 1) * 50;         // sin() ranges from -1 -> 1 Adding 1 gives 0 -> 2 x 50 gives.. 0 -> 100
  currentSlice = currentSlice + SLICE_MS;
  //Serial.print("currentSlice : ");Serial.print(currentSlice);Serial.print("  Percent : ");Serial.println(percent);
  return percent;
}



void robbyRing::draw(void) {

  float percent;
  colorObj result;

  if (!sharedInit) {
    ringSetup();
  } else if (!ourInit) {
    //Serial.println("Calling local init.");
    setPixels(&black);
    ourInit = true;
  } else if (lightTimer.ding()) {
    //Serial.println("Drawing something.");
    percent = calculatePercent();
    result = lightMapper.Map(percent);
    result = result.blend(&black, startMapper.Map(millis() - startMS));
    setPixels(&result);
    lightTimer.stepTime();
  }
}
