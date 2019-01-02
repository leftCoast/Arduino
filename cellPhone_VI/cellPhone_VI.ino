
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

#include <cellCommon.h>

#include "icons.h"
#include "litlOS.h"

#define TFT_CS  10
#define TFT_RST -1    // I think I remember this is not used.
#define SD_CS   4



extern qCMaster    ourComObj;          // Object used to comunicate with the FONA controller.
extern litlOS*     ourOS;              // Hopefully this'll be used to manage the panels.
extern comStates   comState;           // State of comunications.
extern danceCards  currCard;           // Who has current control of comunications.

void setup() {

  analogWrite(SCREEN_PIN,0);    // Turn off backlight.
  
  // First bring the screen online.
  if (!initScreen(ADAFRUIT_1947,TFT_CS,TFT_RST,PORTRAIT)) {
    while(true);
  }

  // Next is to bring the comuication hardware online.
  comState = offline;                   // Not online yet.
  ourComObj.begin(9600);                // Fire up comunications.
  if (ourComObj.readErr()!=NO_ERR) {    // Did the poor thing fire up?
    while(true);
  }
  comState = standby;                   // Communications standing by!
  currCard = noOne;                     // Free to dance with any and all.
  
  if (!SD.begin(SD_CS)) {               // Bring the diskdrive online.
    while(true);
  }
   
  ourOS = new litlOS();                
  ourOS->begin();                       // OS manager, online.
}


void loop() {
  idle();
  ourOS->loop();
}
