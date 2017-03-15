#include "fListItem.h"
#include <soundCard.h>

extern  soundCard   thePlayer;
extern  bool        fileLoaded;
extern  bool        doneEditing;


void  editTime(void) {

  // You know, the dial thing.
}


fListItem::fListItem(char* fileName,bmpPipe* inBackdrop) {

  ourText = new bmpLabel(F_LIST_TXT_X,1,F_LIST_W-F_LIST_TXT_X,F_LIST_H-2,fileName,inBackdrop);
  if (ourText) {
    ourText->setParent((drawObj*)this);
  }
  choosen = false;
}


fListItem::~fListItem(void) {

  if (ourText) {
    delete(ourText);
  }
}


// Show we are the choosen one! Or not..
void fListItem::highlite(bool onOff) {

  if (onOff) {
    ((viewMgr*)parentObj)->theTouched = (drawObj*) this;
  }
  choosen = onOff;
  needRefresh = true; 
}


// For starters, this will be to play the file.
void fListItem::doAction(void) {

  if (ourText) {
    thePlayer.setSoundfile(ourText->buff);
    fileLoaded = true;
  } else {
    fileLoaded = false;
  }
  doneEditing = true;
}


void fListItem::drawSelf() {

  rect  ourRect;

  ourRect.x = scrX();
  ourRect.y = scrY();
  ourRect.width = width;
  ourRect.height = height;
  if (ourText) {
    ourText->backdrop->setSourceRect(ourRect);
    ourText->backdrop->drawBitmap(ourRect.x,ourRect.y);
    ourText->drawSelf();
  }
  if (choosen) {
    screen->drawRect(ourRect.x,ourRect.y,width,height,&black);
  }
}

