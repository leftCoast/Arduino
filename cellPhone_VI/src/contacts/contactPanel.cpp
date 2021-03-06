
#include "../phone/phone.h"

#include <resizeBuff.h>
#include <keystroke.h>
#include <scrKeyboard.h>
#include <lineObj.h>
#include <PNLabel.h>

#include "contactPanel.h"
#include "../../textPanel.h"

#include <debug.h>

#define PNLIST_X        10
#define PNLIST_Y        24 //28
#define PNLIST_W        220


#define PN_ITEM_X1      0
#define PN_ITEM_Y       0   // Doesn't really matter, it scrolls.
#define PN_ITEM_W       PNLIST_W
#define PN_ITEM_H       40 //42
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
contact*        currContact = NULL;


// *****************************************************
// *******************  addrStarter  *******************
// *****************************************************


addrStarter::addrStarter(void) {  }
addrStarter::~addrStarter(void) {  }

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
      aContact->setNickName("Left Coast");
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


void contTextBtn::doAction(void) {

  currentContact = currContact;
  nextPanel = textApp;
}



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


PNEditField::PNEditField (rect* inRect,char* inText,keyboard* inKeyboard,PNListItem* ourListItem)
  : cellEditField(inRect,inText,inKeyboard) { mOurListItem = ourListItem; }


PNEditField::~PNEditField(void) {  }


void PNEditField::setThisFocus(bool setLoose) {
	
	cellEditField::setThisFocus(setLoose);		// Parents can do their thing..
	if (setLoose) {
		mOurListItem->startedEdit();
	} else {
		mOurListItem->finishEdit(this);
	}
}


void PNEditField::drawSelf(void) { }
 

// *****************************************************
// *******************  PNListItem  ********************
// *****************************************************


PNListItem::PNListItem(PNList* ourList,contact* inContact,keyboard* inKeyboard)
  : drawGroup(PN_ITEM_X1,PN_ITEM_Y,PN_ITEM_W,PN_ITEM_H) {

  lineObj*  aDivider;
  rect      aRect;

  mList    = ourList;
  mContact = inContact;

  aDivider = new lineObj(PN_ITEM_LX,PN_ITEM_LY,PN_ITEM_LX2,PN_ITEM_LY,&redButtonColor);
  addObj(aDivider);

  aRect.setRect(PN_EITEM_LX,PN_EITEM_LY,PN_EITEM_LW,PN_EITEM_H);
  pNumEditField = new PNEditField(&aRect,mContact->mPN,inKeyboard,this);
  pNumEditField->formatAsPN();
  addObj(pNumEditField);


  aRect.setRect(PN_EITEM_SX,PN_EITEM_SY,PN_EITEM_SW,PN_EITEM_H);
  nickEditField = new PNEditField(&aRect,mContact->mNickName,inKeyboard,this);
  addObj(nickEditField);


  aRect.setRect(PN_EITEM_SX2,PN_EITEM_SY,PN_EITEM_SW,PN_EITEM_H);
  fNameEditField = new PNEditField(&aRect,mContact->mFirstName,inKeyboard,this);
  addObj(fNameEditField);


  aRect.setRect(PN_EITEM_SX3,PN_EITEM_SY,PN_EITEM_SW,PN_EITEM_H);
  lNameEditField = new PNEditField(&aRect,mContact->mLastName,inKeyboard,this);
  addObj(lNameEditField);


  aRect.setRect(PN_EITEM_LX2,PN_EITEM_LY,PN_EITEM_LW,PN_EITEM_H);
  compEditField = new PNEditField(&aRect,mContact->mCompanyName,inKeyboard,this);
  addObj(compEditField);
}


PNListItem::~PNListItem(void) {  }


void PNListItem::draw(void) {

  if (mList->isVisible(this)) {
      drawGroup::draw();
  }
  needRefresh = false;
}


void PNListItem::drawSelf(void) { }


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


PNList::PNList(int x,int y,int width,int height,keyboard* inKeyboard)
  : scrollingList(x,y,width,height,touchScroll,dragEvents) { mKeyboard = inKeyboard; }


PNList::~PNList(void) {  }


void PNList::drawSelf(void) { }


void PNList::addContact(contact* contactPtr,bool showContact) {

  PNListItem*   itemPtr;

  if (contactPtr) {
    itemPtr = new PNListItem(this,contactPtr,mKeyboard);
    if (itemPtr) {
      addObj(itemPtr);
      if (showContact) {
        showItem(itemPtr);
      }
    }
  }
}


void PNList::fillList(void) {

	int		numAddr;
	contact*	contactPtr;

	ourBlackBook->sort(true);
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
      ourBlackBook->sort(true);						// Resort the contact list.
      delete(anItem);                           // Delete the list item.
      setPositions();                           // Close holes in the list of items.
      setNeedRefresh();									// Force a redraw.
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
  : cellOSPanel(contactApp) { mKeyboard = NULL; }


contactPanel::~contactPanel(void) { if (mKeyboard) delete mKeyboard; }


void contactPanel::setup(void) {

  //mKeyboard = new keyboard();
  mKeyboard = new bmpKeyboard(NULL,true);
  mKeyboard->loadKeys();
  addObj(mKeyboard);
  
  mPNList = new PNList(PNLIST_X,PNLIST_Y,PNLIST_W,PNLIST_H,mKeyboard);  // Whip up the list of contacts.
  mPNList->fillList();                                                  // Fill it with goodies.
  addObj(mPNList);                                                      // Pass it to the a group.

  contNewBtn* ourNewButton = new contNewBtn(mPNList);
  mMenuBar->addObj(ourNewButton);

  contTextBtn*  ourTextBtn = new contTextBtn();
  mMenuBar->addObj(ourTextBtn);

  contCallBtn* ourCallBtn = new contCallBtn();
  mMenuBar->addObj(ourCallBtn);

  contTrashBtn* ourTrashBtn = new contTrashBtn(mPNList);
  mMenuBar->addObj(ourTrashBtn);
}


void contactPanel::loop(void) {  }


void contactPanel::drawSelf(void) {

  screen->fillScreen(&backColor);
}


void contactPanel::closing(void) {

  setFocusPtr(NULL);
  ourBlackBook->saveToFile();
}
