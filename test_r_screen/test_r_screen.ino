#include <Adafruit_GFX.h>
#include <gfxfont.h>

#include <Adafruit_ILI9341.h>

//#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
//#include <PulseOut.h>
#include <runningAvg.h>
//#include <servo.h>
#include <timeObj.h>

//#include <adafruit_1431_Obj.h> // ADAFRUIT_1431
#include <adafruit_1947_Obj.h>  // ADAFRUIT_1947
#include <displayObj.h>
#include <drawObj.h>
#include <label.h>
#include <lineObj.h>
#include <screen.h>

label hello("Hello world!");

void setup() {

  if (!initScreen(ADAFRUIT_1947)) {
    while(true); // Kill the process.
  }
  screen->fillScreen(&green);
  viewList.addObj(&hello);
}


void loop() {
  idle();

}
