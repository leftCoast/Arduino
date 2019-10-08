
#include <Adafruit_GFX.h> 
#include <gfxfont.h>
#include <Adafruit_ILI9341.h>
#include <adafruit_1947_Obj.h>  // ADAFRUIT_1947
#include <SD.h>

#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <timeObj.h>
#include <displayObj.h>
#include <bmpPipe.h>
#include <screen.h>

#include <litlOS.h>

#define TFT_CS  10
#define TFT_RST -1    // I think I remember this is not used.
#define SD_CS   4

litlOS ourOS;

void setup() {
  
  // First bring the screen online.
  if (!initScreen(ADAFRUIT_1947,TFT_CS,TFT_RST,PORTRAIT)) {
    Serial.println("No screen! Shooting one's self in foot now.");
    while(true);
  }

  // Need to fire up the SD card reader?
  // Good place to do that. Along with other
  // fun pieces of hardware you'll need to
  // make life meaningfull.
  
}


void loop() {     // In loop(), all we do is..
  idle();         // Give the idlers a kick.
  ourOS.loop();   // Give the OS a kick.
}
