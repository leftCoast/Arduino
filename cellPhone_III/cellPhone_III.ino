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

#include "litlOS.h"
#include "editField.h"
#include "keyboard.h"

#define TFT_CS  10
#define TFT_RST -1  // I think I remember this is not used.

editField hello(1,180,300,16,"Well?");
keyboard* ourKeyboard;

void setup() {
  colorObj aColor;

  Serial.begin(115200);
  while(!Serial);
  if (!initScreen(ADAFRUIT_1947,TFT_CS,TFT_RST,PORTRAIT)) {
    while(true); // Kill the process.
  }
  aColor = ourOS.getColor(SYS_PANEL_COLOR);
  screen->fillScreen(&aColor);
  viewList.addObj(&hello);
  ourKeyboard = new keyboard(&hello);
}

  
void loop() {
  idle();

}
