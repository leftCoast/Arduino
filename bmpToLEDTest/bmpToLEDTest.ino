
#include <SD.h>
#include <adafruit_1431_Obj.h>
#include <neoPixel.h>
#include <bmpImage.h>
#include <imgNeoPixel.h>
#include <screen.h>
#include <timeObj.h>

#define NUM_LEDS     150
#define NUM_LINES    400
#define MS_PER_LINE  50
#define PIXEL_PIN    10
#define SD_CS        4
#define FILE_NAME    "/newPic.bmp"

#define OLED_CS      7
#define OLED_RST     8


neoPixel lites(NUM_LEDS, PIXEL_PIN);
bmpImage       thePicFile;
imgNeoPixel*   theNeoPic;
timeObj        lineTimer(MS_PER_LINE);
RGBpack        theLine[NUM_LEDS];
int            lineNum;
bool           goUp;

void setup() {

   Serial.begin(57600);
   lites.begin();
   lites.setAll(&black);
   lites.show();
   initScreen(ADAFRUIT_1431,OLED_CS,OLED_RST,INV_PORTRAIT);   // Have to fire up the screen because it has the SD drive on it.
   screen->fillScreen(&black);
   if (!SD.begin(SD_CS)) {
      Serial.println("No SD Card?");
   }
   if (thePicFile.openDocFile(FILE_NAME)) {
      theNeoPic = new imgNeoPixel(&lites,&thePicFile);
   } else {
      Serial.println("No File?");
   } 
   lineNum = 0;
   goUp = true;
   theNeoPic->setupOffscreen(); // If you have the RAM, let it really rip!
}


// Wipe up and down..
void incrementCount(void) {

   if (goUp) {
      lineNum++;
      if (lineNum>NUM_LINES) {
         lineNum=NUM_LINES;
         goUp = false;
        }
   } else {
      lineNum--;
      if (lineNum<0) {
         lineNum=0;
         goUp = true;
      }
   }
}


void loop(void) {

   if (lineTimer.ding()) {
      lineTimer.stepTime();            // Reset our line timer.
      theNeoPic->setLine(lineNum);     // Read a line into the neoPixels.
      lites.show();                    // Show it.
      incrementCount();                // Set up for next line.
   }
}
