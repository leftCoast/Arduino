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


class fListItem : public label {

public:
          fListItem(drawList* myList,char* name);
  virtual ~fListItem(void);

  virtual void drawSelf(void);
  
          drawList* mList;
};


fListItem::fListItem(drawList* myList,char* name)
  : label(0,0,78,12,name,1) { mList = myList; }


fListItem::~fListItem(void) {  }

void fListItem::drawSelf(void) {

  char  buff[20];
  
  int offst = 2;
  getText(buff);
  Serial.print("Getting draw call fpr item : ");Serial.println(buff);
  if (mList->isVisible(this)) {
    Serial.print("Drawing item : ");Serial.println(buff);
    if (focus) {
      setColors(&red);
    } else {
      setColors(&black);
    }
    screen->fillRect(x,y,width,height,&green);
    x=x+offst;
    y=y+offst;
      label::drawSelf();
    x=x-offst;
    y=y-offst;
  }
  //delay(3000);
}


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

runningAvg      potSmoother(5);
mapper          potToScroll(0,1024,0,100);
//bmpPipe         paper;
scrollingList*  ourList;


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
  ourList = new scrollingList(20,24,126,60,dialScroll);
  ourList->setNeedRefresh();
  viewList.addObj(ourList);
  newItem = new fListItem(ourList,"First");
  ourList->addObj(newItem);
  newItem = new fListItem(ourList,"Second");
  ourList->addObj(newItem);
  newItem = new fListItem(ourList,"Third");
  ourList->addObj(newItem);
  Serial.print("OurList : ");Serial.println((int)ourList);
  Serial.print("viewList : ");Serial.println((int)&viewList);
  //fillList("/");
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




void loop() {
  
  int rawPrawn;
  float pcnt;

  idle();
  rawPrawn = analogRead(POT_ANLG);
  pcnt = potToScroll.Map(rawPrawn);
  ourList->setScrollValue(pcnt);
}
