
#include <SSD_13XX.h>
#include <Adafruit_VS1053.h>
#include <adafruit_1431_Obj.h>
#include <LC_SPI.h>
//#include <SSD_13XX_Obj.h>

#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <runningAvg.h>
#include <timeObj.h>

#include <bmpPipe.h>
#include <bmpLabel.h>
#include <displayObj.h>
#include <drawObj.h>
#include <label.h>
#include <lineObj.h>
#include <screen.h>

#include <soundCard.h>

#define SOUND_CS    20
#define SOUND_SDCS  21
#define SOUND_DRQ   1
#define SOUND_RST   15

#define OLED_CS     10
#define OLED_RST    6
#define OLED_SDCS   -1    // Not wired

#define POT_BTN     4
#define POT_ANLG    A13

#define DB_BTN      3


#define DISP_BG_FILE  "/dbos/paper.bmp"
bmpPipe paper;
bmpLabel label1(20, 6, 100, 10, "File name 1", &paper);
soundCard thePlayer(soundCard_BREAKOUT, SOUND_CS, SOUND_DRQ, SOUND_RST);
byte vol = 20;
void setup() {

  Serial.begin(9600); while (!Serial);
  Serial.println("Serial online.");

if (!initScreen(ADAFRUIT_376, OLED_CS, OLED_RST, INV_PORTRAIT)) {
    Serial.println("Init screen card fail.");
    //while(true); // Kill the process.
  } else {
    screen->fillScreen(&blue);
    Serial.println("Screen\'s online.");
  }

  
  Serial.println("Starting file system.");
  if (!SD.begin(SOUND_SDCS)) {
    Serial.println("SD.begin() fail.");
  } else {
    Serial.println("SD.begin() success.");
  }

  

  
  Serial.println("Testing Sound card.");
  if (thePlayer.begin()) {
    Serial.println("Sound card OK?");
    if (thePlayer.setSoundfile("Corvette.mp3")) {
      Serial.println("Happy with Corvette?");
      if (thePlayer.command(play)) {
        Serial.println("Playing Corvette!");
        thePlayer.setVolume(vol);
      } else {
        Serial.println("Couldn't play Corvette.");
        Serial.print(F("command(play) failed with error# ")); Serial.println((int)thePlayer.getLastError());
      }
    } else {
      Serial.println("Couldn't open Corvette.");
    }
  } else {
    Serial.println("Sound card fail!");
  }

}


void loop() {
  idle();

}
