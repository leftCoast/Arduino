#include "cellOS.h"

#include <resizeBuff.h>
#include <keystroke.h>
#include <keyboard.h>

#include "contactPanel.h"

blockFile*      mFile;
contactList*    ourBlackBook;
keyboard*       ourKeyboard = NULL;

// *****************************************************
// *******************  addrStarter  *******************
// *****************************************************


addrStarter::addrStarter(void) {  }
addrStarter::~addrStarter(void) {  }

void addrStarter::begin(char* filePath) {

  mFile = new blockFile(filePath);
  if (mFile) {
    ourBlackBook = new contactList(mFile);
  }
}



// *****************************************************
// *******************  PNEditField  *******************
// *****************************************************

                         
PNEditField::PNEditField (rect* bRect,rect* tRect,char* inText)
  : drawGroup(bRect) {

  mEditBase = new colorRect(bRect,&backColor);
  mEditField = new editField(tRect,inText,1);
  addObj(mEditBase);
  addObj(mEditField);
}


PNEditField::~PNEditField(void) {  }


void PNEditField::setFocus(bool setLoose) {

  if (setLoose) {
    mEditField->setColors(&textSelectColor,&editFieldBColor);
    mEditBase->setColor(&editFieldBColor);
    if (ourKeyboard) {
      ourKeyboard->setEditField(mEditField);
    }
  } else {
    mEditField->setColors(&textColor,&backColor);
    mEditBase->setColor(&backColor);
    if (ourKeyboard) {
      if (ourKeyboard->mEditField==mEditField) {
        ourKeyboard->setEditField(NULL);
      }
    }
  }
}


// Not including the \0. You may need to add one.
int PNEditField::getNumChars(void) { return mEditField->getNumChars(); }


// You better have added the (1) for the \0.
void PNEditField::getText(char* inBuff) { mEditField->getText(inBuff); }



// *****************************************************
// *******************  PNListItem  ********************
// *****************************************************


PNListItem::PNListItem(contact* inContact)
  : drawGroup(PN_ITEM_X1,PN_ITEM_Y,PN_ITEM_W,PN_ITEM_H) {

  rect  bRect;
  rect  tRect;
  
  mContact = inContact;

  bRect.setRect(PN_EITEM_LX,PN_EITEM_LY,PN_EITEM_LW,PN_EITEM_H);
  tRect = bRect;
  tRect.insetRect(PN_EITEM_INSET);
  tRect.x++;
  pNumEditField = new PNEditField(&bRect,&tRect,mContact->mPN);
  addObj(pNumEditField);

  
  bRect.setRect(PN_EITEM_SX,PN_EITEM_SY,PN_EITEM_SW,PN_EITEM_H);
  tRect = bRect;
  tRect.insetRect(PN_EITEM_INSET);
  tRect.x++;
  nickEditField = new PNEditField(&bRect,&tRect,mContact->mNickName);
  addObj(nickEditField);

  
  bRect.setRect(PN_EITEM_SX2,PN_EITEM_SY,PN_EITEM_SW,PN_EITEM_H);
  tRect = bRect;
  tRect.insetRect(PN_EITEM_INSET);
  tRect.x++;
  fNameEditField = new PNEditField(&bRect,&tRect,mContact->mFirstName);
  addObj(fNameEditField);

  
  bRect.setRect(PN_EITEM_SX3,PN_EITEM_SY,PN_EITEM_SW,PN_EITEM_H);
  tRect = bRect;
  tRect.insetRect(PN_EITEM_INSET);
  tRect.x++;
  lNameEditField = new PNEditField(&bRect,&tRect,mContact->mLastName);
  addObj(lNameEditField);

  
  bRect.setRect(PN_EITEM_LX2,PN_EITEM_LY,PN_EITEM_LW,PN_EITEM_H);
  tRect = bRect;
  tRect.insetRect(PN_EITEM_INSET);
  tRect.x++;
  compEditField = new PNEditField(&bRect,&tRect,mContact->mCompanyName);
  addObj(compEditField);
}


PNListItem::~PNListItem(void) {  }


void PNListItem::finishEdit(PNEditField* theField) {

  char* buff;
  int   numChars;
  
  buff = NULL;
  if(theField==pNumEditField) {
    numChars = pNumEditField->getNumChars() + 1;
    if (resizeBuff(numChars,(uint8_t**)&buff)) {
      pNumEditField->getText(buff);
      if (strcmp(buff,mContact->mPN)) {
        mContact->setPN(buff); 
      }
    }
  } else if(theField==nickEditField) {
    numChars = nickEditField->getNumChars() + 1;
    if (resizeBuff(numChars,(uint8_t**)&buff)) {
      nickEditField->getText(buff);
      if (strcmp(buff,mContact->mNickName)) {
        mContact->setNickName(buff); 
      }
    }
  } else if(theField==fNameEditField) {
    numChars = fNameEditField->getNumChars() + 1;
    if (resizeBuff(numChars,(uint8_t**)&buff)) {
      fNameEditField->getText(buff);
      if (strcmp(buff,mContact->mFirstName)) {
        mContact->setFirstName(buff); 
      }
    }
  } else if(theField==lNameEditField) {
    numChars = lNameEditField->getNumChars() + 1;
    if (resizeBuff(numChars,(uint8_t**)&buff)) {
      lNameEditField->getText(buff);
      if (strcmp(buff,mContact->mLastName)) {
        mContact->setLastName(buff); 
      }
    }
  } else if(theField==compEditField) {
    numChars = compEditField->getNumChars() + 1;
    if (resizeBuff(numChars,(uint8_t**)&buff)) {
      compEditField->getText(buff);
      if (strcmp(buff,mContact->mCompanyName)) {
        mContact->setCompanyName(buff); 
      }
    }
  }
  resizeBuff(0,(uint8_t**)&buff);
}



// *****************************************************
// *********************  PNList  **********************
// *****************************************************


PNList::PNList(int x,int y,int width,int height)
  : drawList(x,y,width,height) {  }

  
PNList::~PNList(void) {  }


void PNList::fillList(void) {

  int           numAddr;
  contact*      contactPtr;
  PNListItem*   itemPtr;
  
  numAddr = ourBlackBook->getCount();
  for (int i=0;i<numAddr;i++) {
    contactPtr = (contact*)ourBlackBook->getByIndex(i);
    if (contactPtr) {
      itemPtr = new PNListItem(contactPtr);
      if (itemPtr) {
        addObj(itemPtr);
      }
    }
  }
}



// *****************************************************
// ******************  contactPanel  *******************
// *****************************************************


contactPanel::contactPanel(void)
  : panel(contactApp,false) {
  if (ourKeyboard==NULL) {
    ourKeyboard = new keyboard(NULL);
  }
}


contactPanel::~contactPanel(void) {

  if (ourKeyboard) {
    delete ourKeyboard;
    ourKeyboard = NULL;
  }
}

           
void contactPanel::setup(void) {

  mPNList = new PNList(PNLIST_X,PNLIST_Y,PNLIST_W,PNLIST_H);  // Whip up the list of contacts.
  mPNList->fillList();                                        // Fill it with goodies.
  addObj(mPNList);                                            // Pass it to the a group.
}


void contactPanel::loop(void) {  }


void contactPanel::drawSelf(void) { screen->fillScreen(&backColor); }


void contactPanel::close(void) {  }


void contactPanel::closing(void) {

  ourBlackBook->saveToFile(); 
}
