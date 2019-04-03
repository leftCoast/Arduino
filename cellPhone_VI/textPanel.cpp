#include "cellOS.h"
#include "textPanel.h"
#include <cellCommon.h>
#include <cellManager.h>

contact* pleaseText;        


// *****************************************************
// ******************  textSortBtn  *******************
// *****************************************************

/*
textSortBtn::textSortBtn(contactPanel* ourPanel)
  : sortBtn(ourPanel) {  }

  
textSortBtn::~textSortBtn(void) {  }


void textSortBtn::doAction(void) {  }
*/

/*
// *****************************************************
// ******************  textContactBtn  ******************
// *****************************************************

textContactBtn::textContactBtn(int x, int y)
  : searchBtn(x,y) { }
  
textContactBtn::~textContactBtn(void) {  }

void  doAction(void) {  }
*/


// *****************************************************
// ********************  convList  **********************
// *****************************************************

// List of text dialogs by contact name.

 convList::convList(int x,int y,int width,int height)
  : scrollingList(x,y,width,height,touchScroll,dragEvents) {  }
  
 convList::~convList(void) {  }
  
void convList::drawSelf(void) { }
void convList::fillList(void) { }
//convListItem* convList::itemByContact(contact* aContact) { }
void convList::deleteConv(void) { }
void convList::doAction(event* inEvent,point* locaPt) { }



// *****************************************************
// ******************  textPanel  *******************
// *****************************************************

// Ok, there's two levels here. First is looking at the list of conversations by
// contanct name and the other would be list of messages by order of time. Both
// will need a keyboard for searching and typing messages.

textPanel::textPanel(void)
  : panel(textApp,noEvents) { }

textPanel::~textPanel(void) {  }
           
void textPanel::setup(void) {

  menuBar* ourMenuBar = new menuBar((panel*)this);
  addObj(ourMenuBar);

  contactsBtn* ourContactsBtn = new contactsBtn(CLOSE_X+40,CLOSE_Y);
  addObj(ourContactsBtn);
}


void textPanel::loop(void) { }
void textPanel::drawSelf(void) {

  rect  userRect(0,24,240,320-24);

  screen->fillRect(&userRect,&backColor);
}

void textPanel::closing(void) { }
