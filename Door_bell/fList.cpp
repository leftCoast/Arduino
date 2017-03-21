#include "fList.h"
#include <soundCard.h>
#include <Fonts/FreeSerif9pt7b.h>

extern  bmpPipe     paper;
extern  soundCard   thePlayer;
extern  bool        fileLoaded;
extern  bool        doneEditing;
extern  bool        settingVol;
extern  char        currentSong[];
extern  void        writeParamFile();
extern  void        readParamFile();

colorObj  pencil;
extern  void    fillFileList(const char* workingDir);
timeObj potTimer;
mapper  listMapper;
fList   fileList;


// How the outside starts us up.      
void startEditor(void) { fileList.reset(); }


// Re-Blank the screen with the paper bitmap.
void drawBackground(void) {

    rect sourceRect;
   
    sourceRect.x = 0;
    sourceRect.y = 0;
    sourceRect.width = 128;
    sourceRect.height = 128;
    paper.setSourceRect(sourceRect);
    paper.drawBitmap(0, 0);
  }

// ******************************
// fList stuff
// ******************************


fList::fList(void) : drawList(FL_X, FL_Y, FL_W, FL_H) { }


// From an unknown state we need to set up our world.
void  fList::reset(void) {

  // Redraw the background of the screen.
  drawBackground();

  // Set up our favorite colors.
  pencil.setColor(&white);
  pencil.blend(&black, 70);

  // Who's saved in params? And, grab the current list of files.
  readParamFile();
  fillFileList("/");

  // Make ourselves the controller. Being top dog is good.
  setControlPtr(this);
  setFocusPtr(NULL);
  setNeedRefresh();
}


// This sets the focus on a list item.
// So that clicking the button will cause it to grab control.
void  fList::chooseItem(int index) {

  drawObj*  item;

  item = getObj(index);
  if (item) {
    setFocusPtr(item);
    makeVisable((fListItem*)item);
  }
}


void fList::doPotVal(int aVal) {

  int i;

  if (potTimer.ding()) {              // Ok, IF the timer is up.
    i = round(listMapper.Map(aVal));  // Map out an item index.
    chooseItem(i);                    // Set focus on that item.
    potTimer.start();
  }
}


// We've been handed control. Deal with it!
void fList::gainControl(void) {

  listMapper.setValues(1023,0, 0, numObjInList() - 1);
  potTimer.setTime(POT_MS);
}


// Scroll 'em!
void fList::scroll(int ydist) {

  fListItem*  trace;
  int x;
  int y;

  trace = (fListItem*)theList;
  while (trace) {
    x = trace->x;
    y = trace->y;
    y = y + ydist;
    trace->setLocation(x, y);
    trace = (fListItem*)trace->drawObj::next;
  }
}


// We want a certan item to be visable. Scroll 'till it is.
void fList::makeVisable(fListItem* theItem) {

  int distance;
  int jumps;
  int itemMax;

  itemMax = theItem->y+theItem->height;
  if (theItem->y < 0) {
    scroll(-theItem->y);
  } else if (itemMax>FL_H) {
    distance = itemMax - FL_H;
    jumps = distance/theItem->height;
    if ((jumps*theItem->height)<distance) {   // Round off or whatever caused it to be short?
      jumps++;                                // Fine, kick in another.
    }
    scroll(-jumps*theItem->height);
  }
}


// Check to see if a certan fListItem is visible or not.
boolean  fList::visible(fListItem* theItem) {

  return (theItem->y >= 0 && theItem->y <= NUM_FLI * FLI_H - 1);
}

// Kind of a hack, but.. Go find who has the current song, set their redraw flag.
// 'Cause they are going to loose it in a second.
// NOTE: Scary shit pointer casting here. ALL items MUST be fListItem pointers!
void  fList::currentSongRefresh() {

  drawObj*   trace;
  
  trace = theList;
  while(trace) {
    if (!strcmp(((fListItem*)trace)->mText->buff,currentSong)) { // Out on a limb here! Blind Casting up into.. 
      trace->setNeedRefresh();
      return;
    } else {
      trace = (drawObj*)trace->next;
    }
  }
}



// ******************************
// fListItem stuff
// ******************************
#define TXT_X 40
#define TXT_Y 1
#define TXT_W 80
#define TXT_H FLI_H-2

#define MK_X  0
#define MK_Y  0
#define MK_W  9
#define MK_H  9

#define PF_X 14
#define PF_Y 0
#define PF_W 8
#define PF_H 8

#define DBS_X 26
#define DBS_Y 0
#define DBS_W 8
#define DBS_H 8


fListItem::fListItem(char* fileName)
  : drawGroup(FLI_X, FLI_Y, FLI_W, FLI_H) {

  // Set up the bits..
  mText = new bmpLabel(TXT_X, TXT_Y, TXT_W, TXT_H, fileName, &paper);
  mText->setFastDraw(false);
  mText->setColors(&pencil);
  mSelector = new DBSelector(mText);
  mMarker = new marker(MK_X, MK_Y, MK_W, MK_H);
  mPlayFile = new playFile(mText);
  
  // Add them in the correct order.
  // NOTE: Let the list delete them.
  addObj(mText);
  addObj(mSelector);
  addObj(mPlayFile);
  addObj(mMarker);
}


fListItem::~fListItem(void) { }


// Focus is on us. Look good!
void fListItem::setFocus(bool onOff) {

  drawObj::setFocus(onOff);       // We let the original do its thing. That should set our refresh flag.
  if (onOff) {
    mMarker->setColor(&red);
  } else {
    mMarker->setColor(&pencil);
  }
}


void fListItem::gainControl(void) {

  listMapper.setValues(0, 1023, 0, 2);
  potTimer.setTime(POT_MS);
  mMarker->pointIn = false;              // Release parent control.
  needRefresh = true;
}


// Button clicked while we were the focus. Take control!!
void fListItem::doAction(void) { setControlPtr(this); }


void fListItem::doPotVal(int aVal) {

  int i;

  if (potTimer.ding()) {                // Ok, IF the timer is up.
    i = round(listMapper.Map(aVal));  // Map out an item index.
    switch (i) {
      case 2 : setFocusPtr(mMarker); break;
      case 1 : setFocusPtr(mPlayFile); break;
      case 0 : setFocusPtr(mSelector); break;
    }
    potTimer.start();
  }
}


// Many of us may be offscreen. So its a -leetle- bit different.
boolean fListItem::wantRefresh(void) {

  if (fileList.visible(this)) {         // Are we actually visable?
    return (drawGroup::wantRefresh());  // We'll let the original code do its thing.
  } else {
    return false;                       // We were offscreen. I knew it!
  }
}


// ******************************
// marker stuff
// ******************************


marker::marker(int x, int y, word width, word height) 
  : bmpDrawObj(x, y, width, height,&paper) {
    
    mColor.setColor(&pencil);
    pointIn = true;
  }


marker::~marker(void) { }


void marker::setColor(colorObj* aColor) {

  mColor.setColor(aColor);
  needRefresh = true;
}


void marker::drawSelf(void) {

    colorObj  aColor;
    
    bmpDrawObj::drawSelf();        // Clear it out.    
    if (pointIn) {                 // Parent control.
      aColor.setColor(&mColor);
      screen->fillRect(0,3,7,2, &aColor);
      screen->fillRect(5,2,2,4, &aColor);
      screen->fillRect(5,1,1,6, &aColor);
      aColor.blend(&white,25);
      screen->drawPixel(4,0, &aColor);
      screen->drawPixel(4,7, &aColor);
      screen->fillRect(7,3,2,2, &aColor);
    } else {                       // Our control!
      if (focus) {
        aColor.setColor(&red);
      } else {
        aColor.setColor(&pencil);
      }
      screen->fillRect(1,3,7,2, &aColor);
      screen->fillRect(2,2,2,4, &aColor);
      screen->fillRect(3,1,1,6, &aColor);
      aColor.blend(&white,25);
      screen->drawPixel(4,0, &aColor);
      screen->drawPixel(4,7, &aColor);
      screen->fillRect(0,3,2,2, &aColor);
    }
  }


// User is looking to select a different list Item.
// Pass the control back to the list.
void marker::doAction(void) {
  
    setControlPtr(&fileList);
    pointIn = true;
    needRefresh = true;
  }



// ******************************
// playFile stuff
// ******************************


playFile::playFile(label* inLabel) : bmpDrawObj(PF_X, PF_Y, PF_W, PF_H, &paper) {
  mText = inLabel;
}


playFile::~playFile(void) { }

  
void playFile::doAction(void) {

    thePlayer.setSoundfile(mText->buff);
    if (!strcmp(mText->buff,currentSong)) { 
      settingVol = true;
    }
    fileLoaded = true;
    doneEditing = true;
}


void playFile::drawSelf(void) {

  colorObj  aColor;

  bmpDrawObj::drawSelf();     // Clear it out.
  if (focus) {
    aColor.setColor(&red);
  } else {
    aColor.setColor(&pencil);
  }
  screen->drawVLine(x,y,height,&aColor);
  screen->drawLine(x,y,x+width-1,height/2,&aColor);
  screen->drawLine(x,y+height-1,x+width-1,height/2,&aColor);
}



// ******************************
// DBSelector stuff
// ******************************


DBSelector::DBSelector(label* inLabel)
  : bmpDrawObj(DBS_X, DBS_Y, DBS_W, DBS_H, &paper) {
  mText = inLabel;
}


DBSelector::~DBSelector(void) { }


void DBSelector::doAction(void) {

  fileList.currentSongRefresh();
  strcpy(currentSong, mText->buff);
  writeParamFile();
  needRefresh = true;
}


void DBSelector::drawSelf(void) {

  colorObj  aColor;
  
  bmpDrawObj::drawSelf();       // Clear it out.
  if (focus) {
    aColor.setColor(&red);
  } else {
    aColor.setColor(&pencil);
  }
  screen->drawRect(x, y, width, height, &aColor);
  if (!strcmp(currentSong, mText->buff)) {
    screen->drawLine(x, y, x + width - 1, y + height - 1, &aColor);
    screen->drawLine(x, y + height - 1, x + width - 1, y, &aColor);
  }
}




