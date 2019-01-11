
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
#include <baseGraphics.h>
#include <displayObj.h>
#include <bmpPipe.h>
#include <screen.h>
#include <litlOS.h>

#include <cellCommon.h>

#include "icons.h"
#include "cellOS.h"
#include "cellManager.h"

#define TFT_CS  10
#define TFT_RST -1    // I think I remember this is not used.
#define SD_CS   4



void setup() {

  analogWrite(SCREEN_PIN,0);    // Turn off backlight.
  
  // First bring the screen online.
  if (!initScreen(ADAFRUIT_1947,TFT_CS,TFT_RST,PORTRAIT)) {
    while(true);
  }

  // Next is to bring the comuication hardware online.
  ourCellManager.begin(9600);             // Fire up comunications.
  if (ourCellManager.readErr()!=NO_ERR) { // Did the poor thing fire up?
    while(true);
  }
  
  if (!SD.begin(SD_CS)) {               // Bring the diskdrive online.
    while(true);
  }
   
  ourOS.begin();                       // Boot OS manager. 
}


void loop() {
  idle();
  ourOS.loop();
}
