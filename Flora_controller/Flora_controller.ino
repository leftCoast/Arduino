#include <Adafruit_GFX.h>
#include <adafruit_1947_Obj.h>
#include "Fonts/FreeSans9pt7b.h"
//#include "Fonts/Serif12.h"


#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>

#include <drawObj.h>
#include <screen.h>
#include <litlOS.h>
#include "floraOS.h"


#define SD_CS   4

void setup() {
  // First bring the screen online.
  if (!initScreen(ADAFRUIT_1947,ADA_1947_SHIELD_CS,PORTRAIT)) {
    Serial.println("NO SCREEN!");
    Serial.flush();
    while(true);
  }
  screen->fillScreen(&black);

  if (!SD.begin(SD_CS)) {                   // With icons, we now MUST have an SD card.
    Serial.println("NO SD CARD!");
    Serial.flush();
    screen->setCursor(10,10);
    screen->setTextColor(&white);
    screen->setTextSize(2);
    screen->drawText("No SD card.");
    while(true);
  }
  
  ourEventMgr.begin();                      // Kickstart our event manager.
                                            // Hardware and services are up and running.
  ourOS.begin();                            // Boot OS manager.
  //ourOS.beep();
}

void loop() {     // During loop..
  idle();         // Idlers get their time.
  ourOS.loop();   // ourOS gets a kick to pass on to the current panel.
}
