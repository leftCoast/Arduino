#include "fList.h"
#include <soundCard.h>


extern  bmpPipe     paper;
extern  soundCard   thePlayer;
extern  bool        fileLoaded;
extern  bool        doneEditing;

        
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
  //screen->fillScreen(&black);
  fillFileList("/");
  fileList.setupMapper();
}

void buttonClick(void) { fileList.buttonClick(); }

void potVal(int aVal) { fileList.potVal(aVal); }


// ******************************
// fList stuff
// ******************************



fList::fList(void) 
  : drawList(FL_X,FL_Y,FL_W,FL_H) {  }


void  fList::setHighlite(int index) {

  fListItem*  trace;
  int         i;
    
  trace = (fListItem*)theList;
  i = 0;
  while(trace) {
    trace->highlite(i==index);
    i++;
    trace = (fListItem*)trace->next;  
  }
}


void fList::potVal(int aVal) {

  int i;
  
  i = round(listMapper.Map(aVal));
  setHighlite(i);
}


void fList::buttonClick() {

  fListItem*  trace;
  
  trace = (fListItem*)theList;
  while (trace) {
    if (trace->choosen) {
      trace->doAction();
      return;
    } else {
      trace = (fListItem*)trace->next;  
    }
  }
}
 

void fList::setupMapper(void) { listMapper.setValues(0,1023,0,numObjInList()-1); }


boolean  fList::visible(fListItem* theItem) {

  return (theItem->y>=0 && theItem->y<=NUM_FLI*FLI_H-1);
}

void fList::drawSelf(void) { /*screen->drawRect(x,y,width,height,&red);*/ }

// ******************************
// fListItem stuff
// ******************************


fListItem::fListItem(char* fileName) 
  :bmpLabel(FLI_X,FLI_Y,FLI_W,FLI_H,fileName,&paper) {

  setFastDraw(false);
  choosen = false;
}


fListItem::~fListItem(void) { }


// Make us the choosen one! Or not..
void fListItem::highlite(bool onOff) {

  if (choosen!=onOff) {
    choosen = onOff;
    needRefresh = true;
  }
}

boolean fListItem::wantRefresh(void) {

  return needRefresh;
}


// For starters, this will be to play the file.
void fListItem::doAction(void) {

  thePlayer.setSoundfile(buff);
  fileLoaded = true;
  doneEditing = true;
}


void fListItem::drawSelf() {
  
  if (fileList.visible(this)) {
    if (choosen) {
      setColors(&red);
    } else {
      setColors(&blue);
    }
    bmpLabel::drawSelf();
  }
}

