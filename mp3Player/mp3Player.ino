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
#include <scrollingList.h>
#include <pushPotGUI.h>
#include <soundCard.h>

#include "songList.h"
#include "controlPanel.h"


// ************************************
// ************** main() **************
// ************************************
//
// Quick apology about the code here. It was originally
// tossed together, as fast as possible, as a test case
// for building a scrolling library. Specifically the
// pushPotGUI version. Uggh!
// 
// Seeing that the screen had .mp3 playing hardware
// attached to it, it morphed into an .mp3 player interface.
// This made the testing more fun. And, at the same time,
// Adafruit contaced me to test their .mp3 player code that
// I'd put a patch in for them..
//
// One thing led to another and here we are. As of this
// writing, (1/9/2019) this stuff works. But it is a complete
// and utter bailing-wired-together hack!
//
// But, just by accadent, I wrote in this cool navy-white-yellow-red
// interface that Julie likes. Maybe I'll used that for my
// cell phone?



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
songList*       ourList;
songListItem*   playing;
controlPanel*   ourController;

mechButton      ourClicker(POT_BTN);
mapper          potToVol(0,1024,0,60);
timeObj         volumeDelay(10);
bool            runVolume;
bool            randomPlay;
colorObj        screenBColor;
colorObj        textUHColor;
colorObj        textHColor;
colorObj        textActiveColor;
runningAvg      potSmoother(25);


void setup() {

 
  screenBColor = colorObj(LC_NAVY);
  textUHColor    = colorObj(LC_WHITE);
  textHColor   = colorObj(LC_YELLOW);
  textActiveColor  = colorObj(LC_RED);
  
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
  randomPlay = true;
  playing = NULL;
  
  /*
  if (paper.openPipe(DISP_BG_FILE)) {
    Serial.println(F("Background .bmp file OPENED."));
  } else {
    Serial.println(F("Background .bmp file FAIL."));
  }
*/
  Serial.println("Building songList");
  Serial.flush();
  ourList = new songList(10,24,118,100);
  viewList.addObj(ourList);
  SDCleaner();
  fillList("/");
  
  //Serial.println("Building toggleBtn");
  //Serial.flush();
  //ourToggler = new toggleBtn(107,6,12,12);
  //viewList.addObj(ourToggler);
  
  Serial.println("Building controlPanel");
  Serial.flush();
  ourController = new controlPanel(10,7,90,10);
  viewList.addObj(ourController);
  Serial.print("songListItems cost : ");Serial.print(sizeof(songListItem)+13);Serial.println(" Bytes");
  setControlPtr(ourList);
}



bool validFilename(char* fileName) {

    char* strPtr;
    byte  strLen;
  
    if(fileName[0]=='_') {                   // Starts with a '_'
      strPtr = strstr(fileName,".MP3");
      if (strPtr) {
        strLen = strlen(fileName);
        strLen = strLen-4;
        if(strPtr==&(fileName[strLen])) {  // Ends with a .mp3
          return false;                    // One of apple's files, kill it.
        }
      }
    }
    if(fileName[0]=='~') {                // Starts with a '~'
        return false;                     // Seen these too. kill them.
    }
    strPtr = strstr(fileName,".TRA");     // Has a .tra in it..
    if (strPtr) {
      return false;                       // Kill those too.
    }
    return true;                          // Found nothing, let it pass.
  }

  
void SDCleaner(void) {

// FSEVEN~1/
// TRASHE~1/
// SPOTLI~1/

  File  wd;
  File  entry;
  char  fileName[15];
  bool  done;
   
  wd = SD.open("/");
  if (wd) {
    wd.rewindDirectory();
    done = false;
    do {
      entry = wd.openNextFile();
      if (entry) {
        strcpy(fileName,entry.name());
        entry.close();                      // Enough of you.
        if (!validFilename(fileName)) {
          SD.remove(fileName);              // One of apple's files, kill it.
        }
      } else {
        done = true;
      }
    } while (!done);
    wd.close();
  }
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


void selectAnother() {

  int           numSongs;
  int           choice;
  songListItem* ourChoice;
  
  numSongs = ourList->numObjInList();
  Serial.print("Song count : ");Serial.println(numSongs);
  choice = random(0,numSongs-1);
  Serial.print("Our chosen index : ");Serial.println(choice);
  ourChoice = (songListItem*)ourList->getObj(choice);
  if(ourChoice) {
    ourChoice->doAction();
    //setFocusPtr(ourChoice);           // Set this item as the one who has focus.  
    ourList->showItem(ourChoice);     // Do we need to scroll to show this item? Do so.
  } else {
    Serial.println("Got a NULL!");
  }
  
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
    if (playing) {
      playing->setNeedRefresh();
      playing = NULL;
    }
    if (randomPlay) {
      selectAnother();
      Serial.println("they want to select another");
      Serial.flush();
    } else {
      runVolume = false;
      setControlPtr(ourList);
    }
  }
   if (ourClicker.clicked()) {
    player->command(fullStop);
    runVolume = false;
    randomPlay = false;
    if (playing) {
      playing->setNeedRefresh();
      playing = NULL;
    }
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
