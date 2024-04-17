#include <Adafruit_GFX.h>
#include <adafruit_1947_Obj.h>

#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>

#include <drawObj.h>
#include <screen.h>
#include <lilOS.h>
#include "handheldOS.h"


#define SD_CS        4

void bootError(const char* errStr) {
    
   screen->fillScreen(&black);   // Fill the screen black.
   screen->setCursor(10,10);     // Move cursor to the top left.
   screen->setTextColor(&white); // Drawing in white..
   screen->setTextSize(2);       // Big enought to notice.
   screen->drawText(errStr);     // Draw the error message.
   analogWrite(SCREEN_PIN,255);   // Bring up the screen.
   while(1);                     // Lock down.
}


// Setup, get the hardware running then fire up the UI & OS.
void setup() {

   //analogWrite(SCREEN_PIN,0);                                      // Turn off backlight.
  
   if (!initScreen(ADAFRUIT_1947,ADA_1947_SHIELD_CS,PORTRAIT)) {  // If we can't get the screen running..
      Serial.println("NO SCREEN!");                               // Send an error out the serial port.
      Serial.flush();                                             // Make sure it goes out!
      while(true);                                                // Lock processor here forever.
   }
   screen->fillScreen(&black);                                    // Looks like we have a screen, fill it with black.
   if (!SD.begin(SD_CS)) {                                        // With icons, we now MUST have an SD card.
      Serial.println("NO SD CARD!");                              // Send an error out the serial port.
      Serial.flush();                                             // Make sure it goes out!
      bootError("No SD card.");                                   // Since we have a display, display the error.
   }
   
   // If we get here, looks like we have hardware running.
   ourEventMgr.begin();                                           // Kickstart our event manager.
   ourOS.begin();                                                 // Fire up our OS sevices.
}


// During loop..
void loop() {
      
  idle();         // Idlers get their time.
  ourOS.loop();   // ourOS gets a kick to pass on to the current panel.
}
