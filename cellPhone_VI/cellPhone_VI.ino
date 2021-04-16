#define DEBUGGGING
#include <debug.h>
   
#include <Adafruit_GFX.h> 
#include <gfxfont.h>
#include <adafruit_1947_Obj.h>  // ADAFRUIT_1947
#include <SD.h>

#include <drawObj.h>
#include <scrKeyboard.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <lineObj.h>
#include <mapper.h>
#include <multiMap.h>
#include <timeObj.h>
#include <quickCom.h>
#include <baseGraphics.h>
#include <displayObj.h>
#include <bmpKeyboard.h>
#include <screen.h>
#include <lilOS.h>
//#include <cellCommon.h>
#include <eventMgr.h>


#include "icons.h"
#include "cellOS.h"
#include "cellManager.h"
#include "src/contacts/contactPanel.h"

#define TFT_CS  10
#define TFT_RST -1    // I think I remember this is not used.
#define SD_CS   4


void bootError(char* errStr) {
   
   screen->fillScreen(&black);   // Fill the screen black.
   screen->setCursor(10,10);     // Move cursor to the top left.
   screen->setTextColor(&white); // Drawing in white..
   screen->setTextSize(2);       // Big enought to notice.
   screen->drawText(errStr);     // Draw the error message.
   analogWrite(SCREEN_PIN,0);    // Bring up the screen.
   while(1);                     // Lock down.
}


void setup() {

  // Turn off backlight.
  analogWrite(SCREEN_PIN,0); 
                                    
  // Give it a bit for the hardware to power up.
  delay(100); 
                                                   
  // Now, bring the screen online.
  if (!initScreen(ADAFRUIT_1947,TFT_CS,TFT_RST,PORTRAIT)) {
    Serial.println("Can't bring the display online.");
    while(true);
  }
  
  // Next is to bring the FONA hardware online.
  ourCellManager.begin(9600);             // Fire up comunications.
  if (ourCellManager.readErr()!=NO_ERR) { // Did the poor thing fire up?
    Serial.println("Can't bring FONA comunication online.");
    bootError("No FONA Coms.");
    while(true);
  }

  // Bring the diskdrive online.
  if (!SD.begin(SD_CS)) {
    Serial.println("Can't bring diskdrive online.");
    bootError("No SD Card.");
    while(true);
  }
  
  addrStarter kicker;                       // Once the disk is online we can..
  kicker.begin(CONTACT_FILE_PATH,false);    // Boot our little black book. false = normal. true? Delete and reset the phonebook.
                                            // kicker should auto-destruct at the end of setup(). As it should.
                                            
  ourEventMgr.begin();                      // Kickstart our event manager.
                                            // Hardware and services are up and running.
  ourOS.begin();                            // Boot OS manager.
  
}


void loop() {     // During loop..
  idle();         // Idlers get their time.
  ourOS.loop();   // ourOS gets a kick to pass on to the current panel.
}
