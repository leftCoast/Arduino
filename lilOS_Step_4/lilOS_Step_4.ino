// *************************************************************************
//
// Setting up a lilOS handheld device. STEP 4 Custom OS.
//
// Fourth step is to setup your custom OS for your hardware. We need..
// • The touch screen that can run inder lilOS.
// • the SD card adapter.
// • The beeper. (Optional)
//
// As before, all these examples we'll be using our little handheld with an
// Adafruit PN 1947 for display and SD card slot.
// 
// Documentation, it's all in the book :
// https://github.com/leftCoast/LC_libraryDocs/blob/main/LC_libraries.pdf
// *************************************************************************

#include <SD.h>
#include <adafruit_1947.h>
#include <LC_SPI.h>
#include "ourOS.h"

#define  DISPLAY_CS  10       // These are the pins my hand held is using.
#define  SD_CS       4        // You will need to replace these with the

// A quick function to show an error in a uniform way. (optional)
void error(const char* inMsg) {
   
   delay(2000);                  // If the first thing we do fails, we'll need this.
   Serial.print("ERROR : ");     // Label.
   Serial.println(inMsg);        // Message.
}


// A quick function to halt the processor because the hardware is not
// going to work. In a uniform way as well. (Also optional)
void halt(const char* function) {

   delay(2000);                           // If the first thing we do fails, we'll need this.
   Serial.print("HALTING PROCESS IN : "); // Tell 'em so they don't wonder.
   Serial.print(function);                // And tell 'em where we stopped.
   Serial.println("()");                  // Make it look right as well.
   while(1);                              // Lock down!
}


// See if we can get everything running..
void setup() {

   Serial.begin(9600);                          // Serial port for debugging.
   screen = new adafruit_1947(DISPLAY_CS,-1);   // We create our screen object.
   if (!screen) {                               // Didn't get a screen?
      error("No screen.");                      // Tell the user.
      halt(__func__);                           // Halt the program and tell the user where.
   }                                            //
   if (!screen->begin()) {                      // Have a go at initialising the screen.
      error("screen begin() failed.");          // Failed? Tell the user.
      halt(__func__);                           // Halt the program and tell the user where.
   }                                            //
   screen->fillScreen(&black);                  // Have a go at making the sreen.. black.   
   if (!SD.begin(SD_CS)) {                      // Have a go at initializing the SD drive.
      error("SD.begin() failed.");              // Failed? Tell the user.
      halt(__func__);                           // Halt the program and tell the user where.
   }                                            //                                         //
   ourEventMgr.begin();                         // Kickstart our event manager.
   ourOS.begin();                              // Fire up our OS sevices.
}

// Loop() now has the idle() to run everything **AND** ourOS.loop() to run the Apps.
void loop() {
  
  idle();         // To get clicks and do updates, you MUST call idle().
  ourOS.loop();   // ourOS gets a kick to pass on to the current panel.
}
