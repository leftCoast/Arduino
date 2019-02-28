#include "cellOS.h"
#include "src/phone/phone.h"

#include <resizeBuff.h>
#include <keystroke.h>
#include <scrKeyboard.h>
#include <lineObj.h>
#include <PNLabel.h>

#include "contactPanel.h"

#define PNLIST_X        10
#define PNLIST_Y        30
#define PNLIST_W        220


#define PN_ITEM_X1      0
#define PN_ITEM_Y       0   // Doesn't really matter, it scrolls.
#define PN_ITEM_W       PNLIST_W
#define PN_ITEM_H       42
#define PN_ITEM_LX      30
#define PN_ITEM_LX2     PNLIST_W - PN_ITEM_LX
#define PN_ITEM_LY      PN_ITEM_H - 2

#define PNLIST_H        PN_ITEM_H * 4

#define PN_EITEM_SW     68
#define PN_EITEM_LW     104
#define PN_EITEM_H      15

#define PN_EITEM_SX     4
#define PN_EITEM_SY     4
#define PN_EITEM_LX     4
#define PN_EITEM_LY     PN_EITEM_SY + PN_EITEM_H + 3

#define PN_EITEM_SG     4
#define PN_EITEM_LG     4

#define PN_EITEM_SX2    PN_EITEM_SX + PN_EITEM_SW + PN_EITEM_SG
#define PN_EITEM_SX3    PN_EITEM_SX2 + PN_EITEM_SW + PN_EITEM_SG
#define PN_EITEM_LX2    PN_EITEM_LX + PN_EITEM_LW + PN_EITEM_LG

#define PN_EITEM_INSET  3

#define CONT_BTN_RAD    3
#define CONT_BTN_SPC    20

#define CLOSE_X         0
#define CLOSE_Y         1
#define CLOSE_W         22
#define CLOSE_H         CLOSE_W

#define NEWBTN_X        CLOSE_X + CLOSE_W + CONT_BTN_SPC
#define NEWBTN_Y        CLOSE_Y
#define NEWBTN_W        CLOSE_W
#define NEWBTN_H        CLOSE_H
#define NEWBTN_PN       "5551212"

#define TEXT_X          NEWBTN_X + NEWBTN_W + CONT_BTN_SPC
#define TEXT_Y          CLOSE_Y
#define TEXT_W          CLOSE_W
#define TEXT_H          CLOSE_H

#define CALL_X          TEXT_X + TEXT_W + CONT_BTN_SPC
#define CALL_Y          CLOSE_Y
#define CALL_W          CLOSE_W
#define CALL_H          CLOSE_H

#define TRASH_X         CALL_X + CALL_W + CONT_BTN_SPC
#define TRASH_Y         CLOSE_Y
#define TRASH_W         CLOSE_W
#define TRASH_H         CLOSE_H

blockFile*      mFile;
contactList*    ourBlackBook;
keyboard*       ourKeyboard = NULL;
contact*        currContact = NULL;


void formatPN(label* numField);


// *****************************************************
// *********************  formatPN  ********************
// *****************************************************


void formatPN(label* numField) {
 
  PNLabel formatter(1,1,1,1,1);
  int     numBytes;
  char*   textBuff;
  
  textBuff = NULL;
  numBytes = numField->getNumChars() + 1;
  if (resizeBuff(numBytes,(uint8_t**)&textBuff)) {
    numField->getText(textBuff);
    formatter.setValue(textBuff);
    resizeBuff(0,(uint8_t**)&textBuff);
    numBytes = formatter.getNumChars() + 1;
    if (resizeBuff(numBytes,(uint8_t**)&textBuff)) {
      formatter.getText(textBuff);
      numField->setValue(textBuff);
      resizeBuff(0,(uint8_t**)&textBuff);
    }
  }
}



// *****************************************************
// *******************  addrStarter  *******************
// *****************************************************


addrStarter::addrStarter(void) {  }
addrStarter::~addrStarter(void) {  }
//13751 daybreak lane.
void addrStarter::begin(char* filePath,bool resetFile) {

  mFile = new blockFile(filePath);
  if (mFile) {
    if (resetFile) {
      contact* aContact;
      mFile->deleteBlockfile();
      delete mFile;
      mFile = new blockFile(filePath);
      ourBlackBook = new contactList(mFile);
      aContact = ourBlackBook->findOrAddContact("14083400352");
      aContact->setNickName("Me");
      aContact->setFirstName("Jim");
      aContact->setLastName("Lee");
      aContact->setCompanyName("Left Coast");
      aContact = ourBlackBook->findOrAddContact("13603335200");
      aContact->setNickName("AllieBob");
      aContact->setFirstName("Alex");
      aContact->setLastName("Lee");
      aContact = ourBlackBook->findOrAddContact("13607084218");
      aContact->setNickName("Vern");
      aContact->setFirstName("Julie");
      aContact->setLastName("Lee");
      aContact->setCompanyName("Slave");
      aContact = ourBlackBook->findOrAddContact("13603332799");
      aContact->setNickName("Shelbers");
      aContact->setFirstName("Shelby");
      aContact->setLastName("Lee");
      aContact->setCompanyName("UW");
      aContact = ourBlackBook->findOrAddContact("13609298457");
      aContact->setNickName("Danny-D");
      aContact->setFirstName("Daniel");
      aContact->setLastName("Lee");
      aContact->setCompanyName("Western");
      aContact = ourBlackBook->findOrAddContact("13603337892");
      aContact->setNickName("Guy");
      aContact->setFirstName("Guy");
      aContact->setLastName("Masters");
      aContact->setCompanyName("Shell Oil");
      aContact = ourBlackBook->findOrAddContact("13608403255");
      aContact->setNickName("Tyler");
      aContact->setFirstName("Tyler");
      aContact->setLastName("Smith");
      aContact->setCompanyName("Contractor");
      aContact = ourBlackBook->findOrAddContact("13603593687");
      aContact->setNickName("Tycho");
      aContact->setFirstName("Tycho");
      aContact->setLastName("Horning");
      aContact->setCompanyName("Dock rat");
      ourBlackBook->saveToFile();
      //Serial.println("Saved to file.");Serial.flush();
      //mFile->printFile();
      //Serial.flush();
    } else {
      ourBlackBook = new contactList(mFile);
    }
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
  : newBtn(NEWBTN_X,NEWBTN_Y) { mList = ourList; }


contNewBtn::~contNewBtn(void) {  }


void contNewBtn::doAction(void) {

  contact*  ourContact;

  ourContact = ourBlackBook->findByPN(NEWBTN_PN);
  if (!ourContact) {
    ourContact = ourBlackBook->findOrAddContact(NEWBTN_PN);
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


contTrashBtn::contTrashBtn(PNList* ourList)
  : trashBtn(TRASH_X,TRASH_Y) { mList = ourList; }


contTrashBtn::~contTrashBtn(void) { }


void contTrashBtn::doAction(void) { mList->deleteContact(); }



// *****************************************************
// *******************  PNEditField  *******************
// *****************************************************


PNEditField::PNEditField (rect* inRect,char* inText,PNListItem* ourListItem)
  : drawGroup(inRect) {

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
  mEditField->setParent(this);
  addObj(mEditField);
  setEventSet(fullClick);
}


PNEditField::~PNEditField(void) {  }


void PNEditField::drawSelf(void) { /*screen->drawRect(this,&white);*/ }
 

// If we get focus, we pass it on to the edit field. When the edit field
// looses focus, it will tell us by calling this this method. SO we don't
// "unset" the edit field's focus, we only set it.
void PNEditField::setFocus(bool setLoose) {
  
  if (setLoose) {
    mOurListItem->startedEdit();
    mEditField->setColors(&textSelectColor,&editFieldBColor);
    mEditBase->setColor(&editFieldBColor);
    if (ourKeyboard) {
      ourKeyboard->setEditField(mEditField);
    }
    currentFocus = mEditField;    // We manually switch focus to mEditField..
    mEditField->setFocus(true);   // mEditField will call this with "false" when it looses focus.
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
}


void PNEditField::doAction(void) { setFocusPtr(this); }


// Not including the \0. You may need to add one.
int PNEditField::getNumChars(void) { return mEditField->getNumChars(); }


void PNEditField::formatAsPN(void) { formatPN(mEditField); }


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
  pNumEditField->formatAsPN();
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


void PNListItem::drawSelf(void) { /*screen->drawRect(this,&green);*/ }


// globals used for texting, calling & deleting.
void PNListItem::startedEdit(void) {

  currContact = mContact;
}


void PNListItem::finishEdit(PNEditField* theField) {

  char* buff;
  int   numChars;

  currContact = NULL;
  buff = NULL;
  if(theField==pNumEditField) {
    pNumEditField->formatAsPN();
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
  : scrollingList(x,y,width,height,touchScroll,dragEvents) {  }


PNList::~PNList(void) {  }


void PNList::drawSelf(void) {  screen->fillRect(this,&backColor); }


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


PNListItem* PNList::itemByContact(contact* aContact) {

  PNListItem* trace;

  trace = (PNListItem*)getObj(0);
  while(trace) {
    if (trace->mContact==aContact) {
      return trace;
    }
    trace = (PNListItem*)trace->dllNext;
  }
  return NULL;
}


void PNList::deleteContact(void) {

  contact*    aContact;                         // Local storage. Less to worry about.
  PNListItem* anItem;

  if (currContact) {                            // If we have a contact to delete..
    aContact = currContact;                     // Save off the contact pointer;
    anItem = itemByContact(aContact);           // Using our pointer, find the list item that's hosting it.
    if (anItem) {                               // If we found the list item..
      currContact = NULL;                       // Just in case, loose the currentContact.
      ourBlackBook->deleteContact(aContact);    // Tell our black book to delete the contact. Using local address copy.
      delete(anItem);                           // Delete the list item.
      setPositions();                           // Close holes in the list of items.
      needRefresh = true;								        // Force a redraw.
    }
  }
}


void PNList::doAction(event* inEvent,point* locaPt) {

  if (inEvent->mType==dragBegin) {      // If its the start of a drag..
    setFocusPtr(NULL);
  }
  scrollingList::doAction(inEvent,locaPt); 
}

// *****************************************************
// ******************  contactPanel  *******************
// *****************************************************


contactPanel::contactPanel(void)
  : panel(contactApp,noEvents) {  }


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

  contTrashBtn* ourTrashBtn = new contTrashBtn(mPNList);
  addObj(ourTrashBtn);
}


void contactPanel::loop(void) {  }


void contactPanel::drawSelf(void) {

  screen->fillScreen(&backColor);
  screen->fillRect(0,0,width,MENU_BAR_H,&menuBarColor);
}


void contactPanel::closing(void) {

  setFocusPtr(NULL);
  ourBlackBook->saveToFile();
  if (ourKeyboard) {
    delete ourKeyboard;
    ourKeyboard = NULL;
  }
}
