#include <Adafruit_GFX.h>
#include <gfxfont.h>

#include <Adafruit_ILI9341.h>

#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>

// Coose your hardware.
#include <adafruit_1431_Obj.h>    // ADAFRUIT_1431
//#include <adafruit_1947_Obj.h>  // ADAFRUIT_1947

#include <displayObj.h>
#include <drawObj.h>
#include <label.h>
#include <lineObj.h>
#include <screen.h>

label hello("Hello world!");

void setup() {

  if (!initScreen(ADAFRUIT_1431,PORTRAIT)) { // Select hardware choice goes here.        
    while(true); // No screen? Kill the process.
  }
  screen->fillScreen(&green);
  viewList.addObj(&hello);
}


void loop() {
  
  idle();
}
