#ifndef textPanel_h
#define textPanel_h

#include <drawObj.h>
#include <label.h>
#include <editField.h>
#include <scrollingList.h>
#include <panel.h>
#include <contactList.h>
#include <colorRect.h>
#include <keystroke.h>
#include <scrKeyboard.h>

extern  contact* pleaseText;


// *****************************************************
// ******************  textSortBtn  *******************
// *****************************************************

/*
class textSortBtn :  public sortBtn {

   public:
          textSortBtn(contactPanel* ourPanel);
  virtual ~textSortBtn(void);

  virtual void  doAction(void);

          textPanel* mPanel;
};
*/

// *****************************************************
// ******************  textSearchBtn  ******************
// *****************************************************


class textSearchBtn :  public searchBtn {

   public:
          textSearchBtn(int x,int y);
  virtual ~textSearchBtn(void);

  virtual void  doAction(void);

};



// *****************************************************
// ********************  convList  **********************
// *****************************************************

// List of text conversations by contact name.

class convList : public scrollingList {

  public:
          convList(int x,int y,int width,int height);
  virtual ~convList(void);
  
  virtual void          drawSelf(void);
          void          fillList(void);
          //convListItem* itemByContact(contact* aContact);
          void          deleteConv(void);
  virtual void          doAction(event* inEvent,point* locaPt);
};



// *****************************************************
// ******************  textPanel  *******************
// *****************************************************

// Ok, there's two levels here. First is loopking at the list of conversations by
//contanct name and the other would be list of messages by order of time. Both
// will need a keyboard for searching and typing messages.

class textPanel  :  public panel {

  public:
          textPanel(void);
  virtual ~textPanel(void);
           
  virtual void    setup(void);
  virtual void    loop(void);
  virtual void    drawSelf(void);
  virtual void    closing(void);

          convList* mConvList;
          //timeObj     statusTimer;
          textView*   mText;
          battPercent*  mBatPct;
          RSSIicon*     mRSSI;
};



#endif
