//#include <SD.h>
#include <adafruit_1947.h>
#include <strTools.h>
#include <lilParser.h>
//#include <SDTools.h>
#include <filePath.h>


#define DSP_CS      10    // We need to fire up the display because it's on the SPI bus.
#define SD_CS        4     // SD chip select pin number. Change to suit your setup.

// **************** SDVolume ****************





// ****************  mySD  ****************


bool mySD::begin(uint8_t CSPin) {

  bool  success;

  cdPin = CSPin;
  success = false;
  if (root.isOpen()) {
    root.close();
  }
  if (card.init(SPI_HALF_SPEED) {
    if (volume.init(card) {
      if (root.openRoot(volume)) {
        success = true;
      }
  }
  return success;
}


void mySD::end() { root.close(); }



void setup() {

  Serial.begin(9600);
  screen = (displayObj*)new adafruit_1947(DSP_CS,-1);
  if (!screen) {                                      // If we can't get the screen running..
    Serial.println("NO SCREEN!");                     // Send an error out the serial port.
    Serial.flush();                                   // Make sure it goes out!
    while (true);                                     // Lock processor here forever.
  }                                                   //
  screen->begin();                                    //
  screen->fillScreen(&black);                         // Looks like we have a screen.
                                                      //
  if (!SD.begin(SD_CS)) {                             // If we can not initialze a SD drive.
    Serial.println("No SD Drive!");                   // Tell the user.
    while (1);                                        // Just stop here.
  }
