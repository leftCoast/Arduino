#include <SD.h>
#include <adafruit_1947.h>
#include <idlers.h>
//#include <label.h>

#define DSP_CS      10
#define SD_CS       4


#ifdef DARDUINO_AVR_MEGA2560
mega = true;
#pragma message "Mega!"
#endif

void setup() {
  
  Serial.begin(57600);                                  //  Fire up serial for debugging.
  screen = (displayObj*) new adafruit_1947(DSP_CS,-1);  // Create our screen object.
  if (screen) {                                         // We got one? Cool!
    if (screen->begin()) {                              // Can we successfully call begin on the screen?
      screen->setRotation(PORTRAIT);                    // Ok, set portrait. Good for handhelds.
      if (SD.begin(ADA_1947_SHIELD_SDCS)) {             // With icons etc. we MUST have an SD card.
        //ourEventMgr.begin();                          // Startup our event manager.
        setupScreen();                                  // Populate the screen.
        return;                                         // Everything fired up successfuly. Lets go!
      } else {                                          //
        Serial.println("NO SD CARD!");                  // Tell user we have no SD card.
        while(true) delay(10);                          // Lock the process here.
      }                                                 //
    }                                                   //
  }                                                     //
  Serial.println("NO SCREEN!");                         // Send an error out the serial port.
  while(true)delay(10);                                 // Lock processor here forever.
}

void setupScreen(void) {

  //label*  HelloText;

  screen->fillScreen(&black);
  /*
  HelloText = new label("Hello world!");
  if (HelloText) {
    HelloText->setColors(&yellow);
    HelloText->setLocation(10,10);
    viewList.addObj(HelloText);
  } else {
    Serial.println("No label!");
  }
  */
}


void loop() { idle(); }        // Idlers get their time.
