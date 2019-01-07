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

class playObj : public idler {

  public:
            playObj(byte boardSetup,byte inCsPin,byte inDrqPin,byte inResetPin=-1);

            bool  begin(void);
            bool  setSoundfile(char* inFilePath);
            bool  command(action inCommand);
            bool  isPlaying(void);
            void  setVolume(byte volume);

            timeObj timer;
};


playObj::playObj(byte boardSetup,byte inCsPin,byte inDrqPin,byte inResetPin) {
  timer.setTime(5000);
}
bool playObj::begin(void) { return true; }
bool playObj::setSoundfile(char* inFilePath) { return true; }
bool playObj::command(action inCommand) { 
  timer.start();
  return true;
}
bool playObj::isPlaying(void) { return timer.ding(); }
void playObj::setVolume(byte volume) { }

// ************************************
// ************* songList *************
// ************************************

//mapper  potToScroll(0,1024,0,100);

class songList :  public scrollingList,
                  public pushPotGUI {

  public:
          songList(int x, int y, word width,word height);
  virtual ~songList(void);

  virtual void        doPotVal(int aVal);
  virtual void        reset(void);

          runningAvg* potSmoother;
          mapper*     potToScroll;
};


songList::songList(int x, int y, word width,word height)
  : scrollingList(x,y,width,height,dialScroll) {

  potSmoother = new runningAvg(5);
  potToScroll = new mapper(0,1024,0,100);
}


songList::~songList(void) {

  if (potSmoother) delete potSmoother;
  if (potToScroll) delete potToScroll;
}


 void  songList::reset(void) { needRefresh = true; }

void songList::doPotVal(int aVal) {

  float pcnt;
  
  pcnt = potToScroll->Map(aVal);
  setScrollValue(pcnt);
}



// ************************************
// ************ fListItem *************
// ************************************


soundCard*  player;
//playObj*    player;
bool        runVolume;

class fListItem : public label {

public:
          fListItem(drawList* myList,char* name);
  virtual ~fListItem(void);

  virtual void drawSelf(void);
  virtual void doAction(void);
  
          drawList* mList;
};


fListItem::fListItem(drawList* myList,char* name)
  : label(0,0,78,12,name,1) { mList = myList; }


fListItem::~fListItem(void) {  }


void fListItem::drawSelf(void) {

  char  buff[20];
  
  int offst = 2;
  getText(buff);
  if (mList->isVisible(this)) {
    Serial.print((int)this);Serial.println(" Drawing.");
    if (focus) {
      setColors(&red);
    } else {
      setColors(&black);
    }
    screen->fillRect(x,y,width,height,&green);
    //screen->drawRect(x,y,width,height,&blue);
    x=x+offst;
    y=y+offst;
      label::drawSelf();
    x=x-offst;
    y=y-offst;
  }
}


void fListItem::doAction(void) {

  char* songFile[20];

  getText((char*)songFile);
  player->setSoundfile((char*)songFile);
  player->command(play);
  setControlPtr(NULL);
  runVolume = true;
}



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
songList*   ourList;
mechButton  ourClicker(POT_BTN);
mapper      potToVol(0,1024,0,60);
timeObj     volumeDelay(25);

void setup() {

  Serial.println(F("Starting screen."));
  if (initScreen(ADAFRUIT_1431, OLED_CS, OLED_RST, INV_PORTRAIT)) {
    screen->fillScreen(&white);
    Serial.println(F("Screen\'s ONLINE."));
  } else {
    Serial.println(F("Init screen card FAIL."));
  }

  Serial.println(F("Starting file system."));
  if (SD.begin(SOUND_SDCS)) {
    Serial.println(F("File system ONLINE."));
  } else {
    Serial.println(F("File system FAIL."));
  }

  screen->fillScreen(&green);

  player = new soundCard(soundCard_BREAKOUT,SOUND_CS,SOUND_DRQ,SOUND_RST);
  //player = new playObj(soundCard_BREAKOUT,SOUND_CS,SOUND_DRQ,SOUND_RST);
  player->begin();
  //player->setSoundfile(BOOT_FILE);
  //player->command(play);
  //player->setVolume(0);
  runVolume = false;
  /*
  if (paper.openPipe(DISP_BG_FILE)) {
    Serial.println(F("Background .bmp file OPENED."));
  } else {
    Serial.println(F("Background .bmp file FAIL."));
  }
*/
  Serial.print("Size of color obj. : ");Serial.println(sizeof(colorObj));
  Serial.print("Size of RGBpack. : ");Serial.println(sizeof(RGBpack));

  fListItem*  newItem;
  ourList = new songList(20,4,100,120);
  ourList->setNeedRefresh();
  viewList.addObj(ourList);
 
  fillList("/");
  setControlPtr(ourList);
}


void fillList(const char* workingDir) {

  File        wd;
  File        entry;
  bool        done;
  fListItem*  newItem;

  wd = SD.open(workingDir);
  if (wd) {
    wd.rewindDirectory();
    done = false;
    do {
      entry = wd.openNextFile();
      if (entry) {
        if (!entry.isDirectory()) {
          if (strstr(entry.name(),".MP3")) {
            newItem = new fListItem(ourList,entry.name());
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
    player->setVolume(vol);
    volumeDelay.start();
  }
  if (!player->isPlaying()) {
    runVolume = false;
    setControlPtr(ourList);
  }
   if (ourClicker.clicked()) {
    player->command(pause);
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
      buttonClick();
      while(ourClicker.clicked());
    }
    potVal(analogRead(POT_ANLG));
  } 
}
