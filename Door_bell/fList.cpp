#include "fList.h"
#include <soundCard.h>
#include <Fonts/FreeSerif9pt7b.h>

extern  bmpPipe     paper;
extern  soundCard   thePlayer;
extern  bool        fileLoaded;
extern  bool        doneEditing;
extern  char        currentSong[];
extern  void        writeParamFile();
extern  void        readParamFile();

        colorObj  pencil;      
extern  void    fillFileList(const char* workingDir);
        fList   fileList;
        
void startEditor(void) {

  rect sourceRect;

  sourceRect.x = 0;
  sourceRect.y = 0;
  sourceRect.width = 128;
  sourceRect.height = 128;
  paper.setSourceRect(sourceRect);
  paper.drawBitmap(0,0);
  pencil.setColor(&white);
  pencil.blend(&black,70);
  readParamFile();
  fillFileList("/");
  fileList.setupMapper();
  fileList.savedHighlight = -1;
}


void buttonClick(void) { fileList.buttonClick(); }

void potVal(int aVal) { fileList.potVal(aVal); }


// ******************************
// fList stuff
// ******************************



fList::fList(void) 
  : drawList(FL_X,FL_Y,FL_W,FL_H) { savedHighlight = -1; }


void  fList::setHighlite(int index) {

  fListItem*  trace;
  int         i;
    
  trace = (fListItem*)theList;
  i = 0;
  while(trace) {
    trace->highlite(i==index);
    if (i==index) { 
      makeVisable(trace);
    }
    i++;
    trace = (fListItem*)trace->drawObj::next;  
  }
}


void fList::potVal(int aVal) {

  int i;
  
  i = round(listMapper.Map(aVal));
  if (i!=savedHighlight) {
    setHighlite(i);
    savedHighlight = i;
  }
}


void fList::buttonClick() {

  fListItem*  trace;
  
  trace = (fListItem*)theList;
  while (trace) {
    if (trace->choosen) {
      trace->doAction();
      return;
    } else {
      trace = (fListItem*)trace->drawObj::next;  
    }
  }
}
 

void fList::setupMapper(void) { listMapper.setValues(0,1023,0,numObjInList()-1); }


void fList::scroll(int ydist) {

  fListItem*  trace;
  int x;
  int y;
  
  trace = (fListItem*)theList;
  while (trace) {
      x = trace->x;
      y = trace->y;
      y = y + ydist;
      trace->setLocation(x,y);
      trace = (fListItem*)trace->drawObj::next;  
    }
  }



void fList::makeVisable(fListItem* theItem) {

  if (theItem->y<0) {
    scroll(-theItem->y);
  } else {
    while(!visible(theItem)) {
      scroll(-FLI_H);
    }
  }
}


boolean  fList::visible(fListItem* theItem) {

  return (theItem->y>=0 && theItem->y<=NUM_FLI*FLI_H-1);
}

void fList::drawSelf(void) { /*screen->drawRect(x,y,width,height,&red);*/ }

// ******************************
// fListItem stuff
// ******************************
#define TXT_X 40
#define TXT_Y 1
#define TXT_W 80
#define TXT_H FLI_H-2

#define MK_X  0
#define MK_Y  FLI_H/2-3
#define MK_W  4
#define MK_H  2

#define DBS_X 26
#define DBS_Y 0
#define DBS_W 8
#define DBS_H 8

fListItem::fListItem(char* fileName) 
  :drawGroup(FLI_X,FLI_Y,FLI_W,FLI_H) {
    
  mText = new bmpLabel(TXT_X,TXT_Y,TXT_W,TXT_H,fileName,&paper);
  mText->setFastDraw(true);
  mText->setColors(&pencil);
  addObj(mText);

  mSelector = new DBSelector(mText);
  addObj(mSelector);
  
  mMarker = new marker(MK_X,MK_Y,MK_W,MK_H);
  addObj(mMarker);
  choosen = false;
}


fListItem::~fListItem(void) { }


// Make us the choosen one! Or not..
void fListItem::highlite(bool onOff) {

  if (choosen!=onOff) {
    if (onOff) {
      mMarker->setColor(&red);
    } else {
      mMarker->setColor(&pencil);
    }
    choosen = onOff;
  }
}


// For starters, this will be to play the file.
void fListItem::doAction(void) {

  thePlayer.setSoundfile(mText->buff);
  fileLoaded = true;
  doneEditing = true;
}

void  fListItem::setGoupRefresh(void) {

  drawObj* trace;

  trace = theList;
  while(trace) {
    trace->setNeedRefresh();
    trace = (drawObj*)trace->next;
  } 
}


boolean fListItem::wantRefresh(void) {
  
  if (fileList.visible(this)) {
    if (needRefresh) setGoupRefresh();
    return(drawGroup::wantRefresh());
  } else {
    return false;
  }
}


// ******************************
// marker stuff
// ******************************


marker::marker(int x,int y, word width, word height) : drawObj(x,y,width,height) {

  mColor.setColor(&pencil);
}

void marker::setColor(colorObj* aColor) { 
  
  mColor.setColor(aColor);
  needRefresh = true;
}

void marker::drawSelf(void) { screen->fillRect(x,y,width,height,&mColor); }




DBSelector::DBSelector(label* inLabel)
    : bmpDrawObj(DBS_X,DBS_Y,DBS_W,DBS_H,&paper) {
      mText = inLabel;
    }


void DBSelector::select(void) {

  strcpy(currentSong,mText->buff);
  writeParamFile();
  needRefresh = true;
}


void DBSelector::drawSelf(void) {
  
  char* CSname;
  
  bmpDrawObj::drawSelf();                                 // Clear it out.
  screen->drawRect(x,y,width,height,&pencil);
  CSname = &(currentSong[1]);
  if (!strcmp(CSname,mText->buff)) {
    screen->drawLine(x,y,x+width-1,y+height-1,&pencil);
    screen->drawLine(x,y+height-1,x+width-1,y,&pencil);
  }
}


