#include <Adafruit_GFX.h> 
#include <gfxfont.h>
#include <Adafruit_ILI9341.h>
#include <adafruit_1947_Obj.h>  // ADAFRUIT_1947
#include <SD.h>

#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <mechButton.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <runningAvg.h>
#include <timeObj.h>

#include <screen.h>

#include "contactList.h"

#define TFT_CS  10
#define TFT_RST -1    // I think I remember this is not used.
#define SD_CS   4

#define CONTACT_FILE_PATH "/SYSTEM/CONTACTS"

contactList*  ourContacts;
blockFile*    mFile;

void setup() {

    // First bring the screen online.
  if (!initScreen(ADAFRUIT_1947,TFT_CS,TFT_RST,PORTRAIT)) {
    while(true);
  }
  
  if (!SD.begin(SD_CS)) {               // Bring the diskdrive online.
    while(true);
  }
  mFile = new blockFile(CONTACT_FILE_PATH);
  ourContacts = new contactList(mFile);
  Serial.println("Not crashed yet.");Serial.flush();
}


void loop() {
  idle();

  
}
