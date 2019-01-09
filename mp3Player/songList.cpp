#include "songList.h"
#include "controlPanel.h"

#include <soundCard.h>


extern soundCard* player;
extern bool           runVolume;
extern bool           randomPlay;
extern songListItem*  playing;
extern toggleBtn*     ourToggler;
extern colorObj       screenBColor;
extern colorObj       textUHColor;
extern colorObj       textHColor;
extern colorObj       textActiveColor;

// ************************************
// ********** songListItem ************
// ************************************


songListItem::songListItem(drawList* myList,char* name)
  : label(0,0,78,12,name,1) { mList = myList; }


songListItem::~songListItem(void) {  }


void songListItem::drawSelf(void) {

  char  buff[20];
  
  int offst = 2;
  getText(buff);
  if (mList->isVisible(this)) {
    if (playing == this) {
      setColors(&textActiveColor);
    } else if (focus) {
      setColors(&textHColor);
    } else {
      setColors(&textUHColor);
    }
    screen->fillRect(x,y,width,height,&screenBColor);
    x=x+offst;
    y=y+offst;
      label::drawSelf();
    x=x-offst;
    y=y-offst;
  }
}


void songListItem::doAction(void) {

  char* songFile[20];

  getText((char*)songFile);
  player->setSoundfile((char*)songFile);
  player->command(play);
  playing = this;
  //setControlPtr(NULL);  // I don't think this should be here..
  runVolume = true;
  needRefresh = true;
}






// ************************************
// ************* songList *************
// ************************************


songList::songList(int x, int y, word width,word height)
  : scrollingList(x,y,width,height,dSOpenTop) {

  potSmoother = new runningAvg(5);
  potToList   = new mapper(0,1024,0,100);
}


songList::~songList(void) {

  if (potSmoother) delete potSmoother;
  if (potToList) delete potToList;
}


void  songList::reset(void) { needRefresh = true; }


void songList::offList(void) {
  
  setControlPtr(ourToggler);
}


void songList::doPotVal(int aVal) {

  float pcnt;
  
  pcnt = potToList->Map(aVal);
  setScrollValue(pcnt);
}
