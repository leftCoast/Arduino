// *************************************************************************
//
// Setting up a lilOS handheld device. STEP 1 hardware.
//
// First step is to get the hardware running. We need..
// • A touch screen that can run inder lilOS.
// • A SD card adapter. Typcially mounted on the screen but, need one.
// • Piezo beepet.
// • If the screen is a TFT a pin number to control the backlight. (Optioinal)
//
// This is all about getting the wiring setup, working and documented.
//
// For this example we'll be using our little handheld with an
// Adafruit PN 1947 for display and SD card slot.
//
// The initial system folder can be found in your..
// Arduino/linbraries/LC_lilOS/extras/For SD card/
// Folder.
//
// Go drag the system folder out of there an onto your SD card. Then
// reinstll it into your hardware.
// 
// *************************************************************************

#include <SD.h>
#include <adafruit_1947.h>
#include <LC_SPI.h>


#define  DISPLAY_CS  10       // These are the pins my hand held is using.
#define  SD_CS       4        // You will need to replace these with the
#define  BEEP_PIN    23       // Ones you are using on your device.
#define  B_LIGHT_PIN 25       // 
#define  B_LIGHT_ON  255      // Ours is a value from 0..255
#define  B_LIGHT_OFF 0        // And anywhere between.

// A quick function to show an error in a uniform way. (optional)
void error(const char* inMsg) {
   
   delay(2000);                  // If the first thing we do fails, we'll need this.
   Serial.print("ERROR : ");     // Label.
   Serial.println(inMsg);        // Message.
}


// A quick function to halt the processor because the hardware is not
// going to work. In a uniform way as well. (Also optional)
void halt(char* function) {

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
   screen->fillScreen(&green);                  // Have a go at making the sreen.. Green.
   analogWrite(B_LIGHT_PIN,B_LIGHT_OFF);        // Try turing out the backlight.
   delay(2000);                                 // Hold it off for a couple seconds.
   analogWrite(B_LIGHT_PIN,B_LIGHT_ON);         // Turn it back on.
   if (!SD.begin(SD_CS)) {                      // Have a go at initializing the SD drive.
      error("SD.begin() failed.");              // Failed? Tell the user.
      halt(__func__);                           // Halt the program and tell the user where.
   }                                            //
   tone(BEEP_PIN,300,100);                      // Lastly see if the thing can make a beep sound.
   Serial.println("If the screen is SOLID green. It blinked to black for a couple");
   Serial.println("seconds, and you heard a beep. Then everything should be ok.");
}


// Nonthing to do here as yet.
void loop() { }
