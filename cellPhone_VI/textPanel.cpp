#include "cellOS.h"
#include "textPanel.h"
#include <cellCommon.h>
#include <cellManager.h>

#define TW_X    10
#define TW_Y    MENU_BAR_H
#define TW_W    240 - (2*TW_X)
#define TW_H    340 - MENU_BAR_H - ROW_1 - 10

#define ET_X    TW_X
#define ET_Y    195
#define ET_W    TW_W
#define ET_H    15

#define ML_X    TW_X
#define ML_Y    TW_Y

contact* currentContact;        


// *****************************************************
// ******************  textPanel  *******************
// *****************************************************


textPanel::textPanel(void)
  : panel(textApp,noEvents) { mKeyboard = NULL; }


textPanel::~textPanel(void) { if (mKeyboard) delete mKeyboard; }

           
void textPanel::setup(void) {

  rect  aRect;

  if (currentContact) {
    mKeyboard = new keyboard(NULL);
    aRect.setRect(ET_X,ET_Y,ET_W,ET_H);
    cellEditField* ourEditField  = new cellEditField(&aRect,"",mKeyboard);
    if (ourEditField) { 
      setFocusPtr(ourEditField);
      addObj(ourEditField);
    }
    menuBar* ourMenuBar = new menuBar((panel*)this);
    addObj(ourMenuBar);
    contactsBtn* ourContactsBtn = new contactsBtn(CLOSE_X+40,CLOSE_Y);
    ourMenuBar->addObj(ourContactsBtn);
    label* nickname = new label(CLOSE_X+80,7,100,22,currentContact->mNickName,1);
    nickname->setJustify(TEXT_CENTER);
    nickname->setColors(&textColor,&menuBarColor);
    ourMenuBar->addObj(nickname);
  } else {
    nextPanel = contactApp;
  }
}


void textPanel::loop(void) { }


void textPanel::drawSelf(void) {

  rect  userRect(0,24,240,320-24);

  screen->fillRect(&userRect,&backColor);
}


void textPanel::closing(void) { }
