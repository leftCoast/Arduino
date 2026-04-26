//#include <adafruit_1947.h>
#include <MSP3526_T.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>

#include <drawObj.h>
#include <lilOS.h>
#include "handheldOS.h"

// For 1947
//#define DSP_CS     10
//#define SD_CS      4
//#define DSP_RST    -1

// For MSP3526_T
#define DSP_CS    10
#define SD_CS     4
#define DSP_RST   26


void bootError(const char* errStr) {
    
   screen->fillScreen(&black);   // Fill the screen black.
   screen->setCursor(10,10);     // Move cursor to the top left.
   screen->setTextColor(&white); // Drawing in white..
   screen->setTextSize(2);       // Big enough to notice.
   screen->drawText(errStr);     // Draw the error message.
   analogWrite(SCREEN_PIN,255);  // Bring up the screen.
   while(1);                     // Lock down.
}


// Setup, get the hardware running then fire up the UI & OS.
void setup() {

   bool haveScreen;
   
   analogWrite(SCREEN_PIN,0);                                     // Turn off backlight.
   Serial.begin(115200);
   //pinMode(VIBE_PIN,OUTPUT);
   //digitalWrite(VIBE_PIN,LOW);
   haveScreen = false;
   //screen = (displayObj*) new adafruit_1947(DSP_CS,DSP_RST);
   //screen = (displayObj*) new adafruit_1947();
   screen =  (displayObj*) new MSP3526_T(DSP_CS,DSP_RST);
   if (screen) {
       if (screen->begin()) {
         screen->setRotation(PORTRAIT);
         haveScreen = true;
      }
   }
   if (!haveScreen) {
      Serial.println("NO SCREEN!");                               // Send an error out the serial port.
      while(true);                                                // Lock processor here forever.
   }
   if (!SD.begin(SD_CS)) {                                        // With icons, we now MUST have an SD card.
      Serial.println("NO SD CARD!");                              // Send an error out the serial port.
      bootError("No SD card.");                                   // Since we have a display, display the error.
      //analogWrite(SCREEN_PIN,255);                                // Turn on backlight.
   }
   ourEventMgr.begin();                                           // Kickstart our event manager.
   ourOS.begin();                                                 // Fire up our OS sevices.
}


// During loop..
void loop() {
      
  idle();         // Idlers get their time.
  ourOS.loop();   // ourOS gets a kick to pass on to the current panel.
}
