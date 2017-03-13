
#include <SSD_13XX.h>
#include <Adafruit_VS1053.h>
#include <LC_SPI.h>
#include <SSD_13XX_Obj.h>

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

#define OLED_CS     10
#define OLED_RST    6
#define OLED_SDCS   -1    // Not wired

#define POT_BTN     4
#define POT_ANLG    A13

#define DB_BTN      3


#define DISP_BG_FILE  "/dbos/paper.bmp"
bmpPipe paper;
bmpLabel label1(20,6,100,10,"File name 1",&paper);
soundCard thePlayer(soundCard_BREAKOUT,SOUND_CS,SOUND_DRQ);

void setup() {

  Serial.begin(9600); while(!Serial);
  Serial.println("Serial online.");
/*
  if (!initScreen(SUMO_TOY_SSD_13XX,OLED_CS,OLED_RST,INV_PORTRAIT)) {
    Serial.println("Init screen card fail.");
    //while(true); // Kill the process.
  } else {
    screen->fillScreen(&blue);
    Serial.println("Screen\'s online.");
  }
  delay(1000); // Lets see it..

  Serial.println("Starting file system.");
  if (!SD.begin(SOUND_SDCS)) {
    Serial.println("SD.begin() fail.");
  } else {
    Serial.println("SD.begin() success.");
    Serial.println("Retesting screen by painting it yellow.");
    screen->fillScreen(&yellow);
  }

  Serial.println("Testing bitmaps.");
  if (paper.openPipe(DISP_BG_FILE)) {
    Serial.println("Opened pipe.");
    paper.drawBitmap(0,0);
    point pos = label1.getCorner(topLeftPt);
    for (int i=0;i<10;i++) {
      label1.setLocation(pos.x,pos.y+(i*12));
      label1.drawSelf();
    }
  } else {
    Serial.println("Failed to open pipe.");
  }

  */
  Serial.println("Testing Sound card.");
  if (thePlayer.begin()) {
    Serial.println("Sound card OK?");
    if (thePlayer.setSoundfile("Corvette.mp3")) {
      Serial.println("Happy with Corvette?");
      //thePlayer.command(play);
    } else {
      Serial.println("Couldn't open Corvette.");
    }
  } else {
    Serial.println("Sound card fail!");
  }

  screen->fillScreen(&white);
}


void loop() {
  idle();

}
