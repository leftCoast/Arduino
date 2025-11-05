//#include <teensy_1947.h>
#include <adafruit_1947.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>

#include <drawObj.h>
#include <lilOS.h>
#include "handheldOS.h"

#define DSP_CS      10
#define SD_CS       4


void bootError(const char* errStr) {
    
   screen->fillScreen(&black);   // Fill the screen black.
   screen->setCursor(10,10);     // Move cursor to the top left.
   screen->setTextColor(&white); // Drawing in white..
   screen->setTextSize(2);       // Big enought to notice.
   screen->drawText(errStr);     // Draw the error message.
   analogWrite(SCREEN_PIN,255);  // Bring up the screen.
   while(1);                     // Lock down.
}


// Setup, get the hardware running then fire up the UI & OS.
void setup() {

   bool haveScreen;

  Serial.begin(9600);
   haveScreen = false;
   analogWrite(SCREEN_PIN,0);                                     // Turn off backlight.
   screen = (displayObj*) new adafruit_1947(DSP_CS,-1);
   //screen = (displayObj*) new teensy_1947(DSP_CS,-1);
   if (screen) {
      if (screen->begin()) {
         screen->setRotation(PORTRAIT);
         haveScreen = true;
      }
   }
   if (!haveScreen) {
      Serial.println("NO SCREEN!");                               // Send an error out the serial port.
      Serial.flush();                                             // Make sure it goes out!
      while(true);                                                // Lock processor here forever.
   }
   if (!SD.begin(SD_CS)) {                                        // With icons, we now MUST have an SD card.
      Serial.println("NO SD CARD!");                              // Send an error out the serial port.
      Serial.flush();                                             // Make sure it goes out!
      bootError("No SD card.");                                   // Since we have a display, display the error.
      analogWrite(SCREEN_PIN,255);                                // Turn on backlight.
   }
   
   // If we get here, looks like we have hardware running.
   ourEventMgr.begin();                                           // Kickstart our event manager.
   ourOS.begin();                                               // Fire up our OS sevices.
   //nextPanel = navTestApp;
}


// During loop..
void loop() {
      
  idle();         // Idlers get their time.
  ourOS.loop();   // ourOS gets a kick to pass on to the current panel.
}
