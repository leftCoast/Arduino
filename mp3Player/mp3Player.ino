#include <Adafruit_VS1053.h>
#include <adafruit_1431_Obj.h>
#include <LC_SPI.h>

#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <runningAvg.h>
#include <timeObj.h>
#include <mechButton.h>

#include <bmpPipe.h>
#include <bmpLabel.h>
#include <displayObj.h>
#include <drawObj.h>
#include <label.h>
#include <lineObj.h>
#include <screen.h>

#include <soundCard.h>

#include "scrollingList.h"
#include "pushPotGUI.h"
#include "songList.h"
#include "controlPanel.h"


// ************************************
// ************** main() **************
// ************************************



#define SOUND_CS    20
#define SOUND_SDCS  21
#define SOUND_DRQ   1
#define SOUND_RST   15

#define OLED_CS     10
#define OLED_RST    6
#define OLED_SDCS   -1    // Not wired

#define POT_BTN     4
#define POT_ANLG    A0    // A13 was used in the original.

#define DB_BTN      5

#define DISP_BG_FILE  "/dbos/paper.bmp"
#define BOOT_FILE  "/dbos/boot.mp3"

//bmpPipe         paper;

soundCard*      player;
bool            runVolume;
songList*       ourList;
controlPanel*   ourController;
toggleBtn*      ourToggler;
mechButton      ourClicker(POT_BTN);
mapper          potToVol(0,1024,0,60);
timeObj         volumeDelay(50);
colorObj        screenBColor;
colorObj        textUHColor;
colorObj        textHColor;
runningAvg      potSmoother(10);


void setup() {

  screenBColor = colorObj(LC_NAVY);
  textUHColor    = colorObj(LC_WHITE);
  textHColor   = colorObj(LC_YELLOW);
  
  if (initScreen(ADAFRUIT_1431, OLED_CS, OLED_RST, INV_PORTRAIT)) {
    screen->fillScreen(&screenBColor);
  } else {
    Serial.println(F("Init screen card FAIL."));
  }

  if (SD.begin(SOUND_SDCS)) {
    
  } else {
    Serial.println(F("File system FAIL."));
  }

  player = new soundCard(soundCard_BREAKOUT,SOUND_CS,SOUND_DRQ,SOUND_RST);
  player->begin();
  runVolume = false;
  /*
  if (paper.openPipe(DISP_BG_FILE)) {
    Serial.println(F("Background .bmp file OPENED."));
  } else {
    Serial.println(F("Background .bmp file FAIL."));
  }
*/

  ourController = new controlPanel(10,7,90,10);
  viewList.addObj(ourController);
  
  ourToggler = new toggleBtn(107,6,12,12);
  viewList.addObj(ourToggler);
  
  ourList = new songList(10,24,118,100);
  viewList.addObj(ourList);
  fillList("/");
  
  setControlPtr(ourList);
}


void fillList(const char* workingDir) {

  File        wd;
  File        entry;
  bool        done;
  songListItem*  newItem;

  wd = SD.open(workingDir);
  if (wd) {
    wd.rewindDirectory();
    done = false;
    do {
      entry = wd.openNextFile();
      if (entry) {
        if (!entry.isDirectory()) {
          if (strstr(entry.name(),".MP3")) {
            newItem = new songListItem(ourList,entry.name());
            ourList->addObj(newItem);
          }
        }
        entry.close();
      } else {
        done = true;
      }
    } while (!done);
    wd.close();
  } else {
    //lastFileError = F_FOF_ERR;  // Sadly, instead of returning a NULL, it just crashes.
  }
  ourList->resetPositions();
}


void doRunVolume(void) {
  int vol;
  int potNum;
  
  potNum = analogRead(POT_ANLG);
  vol = potToVol.Map(potNum);
  if (volumeDelay.ding()) {
    volumeDelay.start();
    player->setVolume(vol);
  }
  if (!player->isPlaying()) {
    runVolume = false;
    setControlPtr(ourList);
  }
   if (ourClicker.clicked()) {
    player->command(fullStop);
    runVolume = false;
    while(ourClicker.clicked());
    setControlPtr(ourList);
  }
}


void loop() {
    
  idle();
  if (runVolume) {
    doRunVolume();
  } else {
    if (ourClicker.clicked()) {
      while(ourClicker.clicked());
      buttonClick();
    }
    potVal(analogRead(POT_ANLG));
  } 
}
