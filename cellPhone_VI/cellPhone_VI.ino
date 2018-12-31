
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Adafruit_ILI9341.h>
#include <adafruit_1947_Obj.h>  // ADAFRUIT_1947
#include <SD.h>

#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <timeObj.h>
#include <quickCom.h>
#include <displayObj.h>
#include <bmpPipe.h>
#include <screen.h>

#include "litlOS.h"

#define TFT_CS  10
#define TFT_RST -1    // I think I remember this is not used.
#define SD_CS   4

qCMaster  ourComObj;  // Object used to comunicate with the FONA controller.
litlOS*   ourOS;      // Hopefully this'll be used to manage the panels.

void setup() {
  
  if (!initScreen(ADAFRUIT_1947,TFT_CS,TFT_RST,PORTRAIT)) {
    while(true);
  }
  if (!SD.begin(SD_CS)) {
    while(true);
  }  
  ourOS = new litlOS();
  ourOS->begin();
}


void loop() {
  idle();
  ourOS->loop();
}
