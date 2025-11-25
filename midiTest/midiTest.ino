
#include <toneObj.h>

#include <autoPOT.h>
#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <mechButton.h>
#include <multiMap.h>
#include <resizeBuff.h>
#include <runningAvg.h>
#include <serialStr.h>
#include <squareWave.h>
#include <strTools.h>
#include <textBuff.h>
#include <timeObj.h>

#include <SD.h>
#include <SDTools.h>
#include <adafruit_1947.h>
#include <MIDINotes.h>


#define DSP_CS       10
#define SD_CS        4
#define BEEP_PIN     23


//https://www.music.mcgill.ca/~ich/classes/mumt306/StandardMIDIfileformat.html

toneObj   theVoice(BEEP_PIN);
MIDItune  theTune;

void setup() {

   bool haveScreen;

   Serial.begin(57600);
   Serial.println("Hello!");
   haveScreen = false;
   screen = (displayObj*) new adafruit_1947(DSP_CS, -1);
   if (screen) {
      if (screen->begin()) {
         screen->setRotation(PORTRAIT);
         haveScreen = true;
      }
   }
   if (!haveScreen) {
      Serial.println("NO SCREEN!");
      Serial.flush();
      while (true);
   }
   if (!SD.begin(SD_CS)) {
      Serial.println("NO SD CARD!");
      Serial.flush();
      while (true);
   }
   
   theTune.createTune("tsttune.mid");
   theTune.adjustSpeed(2);
   theTune.startTune(&theVoice);
   //Serial.println("Calling decode");
   //decodeFile("fith2.mid");
   //Serial.println(0x82,BIN);
   //Serial.println(0x68,BIN);
   //Serial.println(0b10101000,DEC);

}




void loop() { idle(); }
