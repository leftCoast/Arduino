#include "cellOS.h"
#include "src/phone/phone.h"

#include <resizeBuff.h>
#include <keystroke.h>
#include <scrKeyboard.h>
#include <lineObj.h>

#include "contactPanel.h"

blockFile*      mFile;
contactList*    ourBlackBook;
keyboard*       ourKeyboard = NULL;
contact*        currContact = NULL;

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
// ******************  contCloseBtn  *******************
// *****************************************************

        
contCloseBtn::contCloseBtn(contactPanel* ourPanel)
  : closeBtn(CLOSE_X,CLOSE_Y) { mPanel = ourPanel; }

contCloseBtn::~contCloseBtn(void) {  }

void contCloseBtn::doAction(void) { mPanel->close(); }


// *****************************************************
// *******************  contNewBtn  ********************
// *****************************************************


contNewBtn::contNewBtn(PNList* ourList)
  : newBtn(NEW_X,NEW_Y) { mList = ourList; }

    
contNewBtn::~contNewBtn(void) {  }


void contNewBtn::doAction(void) {

  contact*  ourContact;

  ourContact = ourBlackBook->findByPN(NEW_PN);
  if (!ourContact) {
    ourContact = ourBlackBook->findOrAddContact(NEW_PN);
    if (ourContact) {
      mList->addContact(ourContact,true);
    }
  }
}


// *****************************************************
// ******************  contTextBtn  ********************
// *****************************************************


contTextBtn::contTextBtn(void)
  : textBtn(TEXT_X,TEXT_Y) {  }

  
contTextBtn::~contTextBtn(void) {  }


void contTextBtn::doAction(void) { }



// *****************************************************
// ******************  contCallBtn  ********************
// *****************************************************


contCallBtn::contCallBtn(void)
  : callBtn(CALL_X,CALL_Y) {  }

  
contCallBtn::~contCallBtn(void) {  }


void contCallBtn::doAction(void) {

  pleaseCall = currContact;
  nextPanel = phoneApp;
}



// *****************************************************
// ******************  contTrashBtn  *******************
// *****************************************************


contTrashBtn::contTrashBtn(void)
  : trashBtn(TRASH_X,TRASH_Y) {  }

  
contTrashBtn::~contTrashBtn(void) { }


void contTrashBtn::doAction(void) { }



// *****************************************************
// *******************  PNEditField  *******************
// *****************************************************

                         
PNEditField::PNEditField (rect* inRect,char* inText,PNListItem* ourListItem)
  : drawGroup(inRect,true) {

  rect  bRect;
  rect  tRect;

  mOurListItem = ourListItem;
  
  bRect.x = 0;
  bRect.y = 0;
  bRect.width = inRect->width;
  bRect.height = inRect->height;

  tRect = bRect;
  tRect.insetRect(PN_EITEM_INSET);
  
  mEditBase = new colorRect(&bRect,&backColor);
  mEditBase->setColor(&backColor);
  addObj(mEditBase);
  mEditField = new editField(&tRect,inText,1);
  mEditField->setColors(&textColor,&backColor);
  addObj(mEditField);
}


PNEditField::~PNEditField(void) {  }


void PNEditField::drawSelf(void) { /*screen->drawRect(this,&white);*/ }


void PNEditField::setFocus(bool setLoose) {

  if (setLoose) {
    mOurListItem->startedEdit();
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
    mOurListItem->finishEdit(this);
  }
  mEditField->setFocus(setLoose);
}


void PNEditField::doAction(void) { setFocusPtr(this); }


// Not including the \0. You may need to add one.
int PNEditField::getNumChars(void) { return mEditField->getNumChars(); }


// You better have added the (1) for the \0.
void PNEditField::getText(char* inBuff) { mEditField->getText(inBuff); }



// *****************************************************
// *******************  PNListItem  ********************
// *****************************************************


PNListItem::PNListItem(PNList* ourList,contact* inContact)
  : drawGroup(PN_ITEM_X1,PN_ITEM_Y,PN_ITEM_W,PN_ITEM_H) {

  lineObj*  aDivider;
  rect      aRect;

  mList    = ourList;
  mContact = inContact;

  aDivider = new lineObj(PN_ITEM_LX,PN_ITEM_LY,PN_ITEM_LX2,PN_ITEM_LY,&redButtonColor);
  addObj(aDivider);
  
  aRect.setRect(PN_EITEM_LX,PN_EITEM_LY,PN_EITEM_LW,PN_EITEM_H);
  pNumEditField = new PNEditField(&aRect,mContact->mPN,this);
  addObj(pNumEditField);

  
  aRect.setRect(PN_EITEM_SX,PN_EITEM_SY,PN_EITEM_SW,PN_EITEM_H);
  nickEditField = new PNEditField(&aRect,mContact->mNickName,this);
  addObj(nickEditField);

  
  aRect.setRect(PN_EITEM_SX2,PN_EITEM_SY,PN_EITEM_SW,PN_EITEM_H);
  fNameEditField = new PNEditField(&aRect,mContact->mFirstName,this);
  addObj(fNameEditField);

  
  aRect.setRect(PN_EITEM_SX3,PN_EITEM_SY,PN_EITEM_SW,PN_EITEM_H);
  lNameEditField = new PNEditField(&aRect,mContact->mLastName,this);
  addObj(lNameEditField);

  
  aRect.setRect(PN_EITEM_LX2,PN_EITEM_LY,PN_EITEM_LW,PN_EITEM_H);
  compEditField = new PNEditField(&aRect,mContact->mCompanyName,this);
  addObj(compEditField);
}


PNListItem::~PNListItem(void) {  }


void PNListItem::draw(void) {

  if (mList->isVisible(this)) {
      drawGroup::draw();
  }
  needRefresh = false;
}


void PNListItem::drawSelf(void) {  }


void PNListItem::startedEdit(void) { currContact = mContact; }


void PNListItem::finishEdit(PNEditField* theField) {

  char* buff;
  int   numChars;

  currContact = NULL; 
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


contact* PNListItem::getContact(void) { return mContact; }



// *****************************************************
// *********************  PNList  **********************
// *****************************************************


PNList::PNList(int x,int y,int width,int height)
  : drawList(x,y,width,height) {  }

  
PNList::~PNList(void) {  }

void PNList::drawSelf(void) {  /*screen->drawRect(this,&red);*/ }


void PNList::addContact(contact* contactPtr,bool showContact) {

  PNListItem*   itemPtr;
  
  if (contactPtr) {
    itemPtr = new PNListItem(this,contactPtr);
    if (itemPtr) {
      addObj(itemPtr);
      if (showContact) {
        showItem(itemPtr);
      }
    }
  } 
}

  
void PNList::fillList(void) {

  int           numAddr;
  contact*      contactPtr;
  
  numAddr = ourBlackBook->getCount();
  for (int i=0;i<numAddr;i++) {
    contactPtr = (contact*)ourBlackBook->getByIndex(i);
    addContact(contactPtr);
  }
}



// *****************************************************
// ******************  contactPanel  *******************
// *****************************************************


contactPanel::contactPanel(void)
  : panel(contactApp,false) {  }


contactPanel::~contactPanel(void) {  }

           
void contactPanel::setup(void) {

  if (ourKeyboard==NULL) {
    ourKeyboard = new keyboard(NULL);
  }
  mPNList = new PNList(PNLIST_X,PNLIST_Y,PNLIST_W,PNLIST_H);  // Whip up the list of contacts.
  mPNList->fillList();                                        // Fill it with goodies.
  addObj(mPNList);                                            // Pass it to the a group.

  contCloseBtn* ourCloseButton = new contCloseBtn(this);
  addObj(ourCloseButton);

  contNewBtn* ourNewButton = new contNewBtn(mPNList);
  addObj(ourNewButton);

  contTextBtn*  ourTextBtn = new contTextBtn();
  addObj(ourTextBtn);
  
  contCallBtn* ourCallBtn = new contCallBtn();
  addObj(ourCallBtn);

  contTrashBtn* ourTrashBtn = new contTrashBtn();
  addObj(ourTrashBtn);
}


void contactPanel::loop(void) {  }


void contactPanel::drawSelf(void) { 

  screen->fillScreen(&backColor);
  screen->fillRect(0,0,width,MENU_BAR_H,&menuBarColor);
}


void contactPanel::closing(void) {

  ourBlackBook->saveToFile();
  if (ourKeyboard) {
    delete ourKeyboard;
    ourKeyboard = NULL;
  }
}
