#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Adafruit_ILI9341.h>

#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <runningAvg.h>
#include <timeObj.h>

#include <adafruit_1947_Obj.h>  // ADAFRUIT_1947
#include <displayObj.h>
#include <drawObj.h>
#include <label.h>
#include <lineObj.h>
#include <screen.h>

#include "editField.h"


#define TFT_CS  10
#define TFT_RST -1  // I think I remember this is not used.

editField hello("Hello world, Nigel");

void setup() {
  colorObj aColor;

  Serial.begin(9600);
  while(!Serial);
  if (!initScreen(ADAFRUIT_1947,TFT_CS,TFT_RST,PORTRAIT)) {
    while(true); // Kill the process.
  }
  viewList.addObj(&hello);
}

  
void loop() {
  idle();

}
