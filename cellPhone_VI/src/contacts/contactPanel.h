#ifndef contactPanel_h
#define contactPanel_h

#include <drawObj.h>
#include <label.h>
#include <editField.h>
#include <scrollingList.h>
#include <panel.h>
#include <contactList.h>
#include <colorRect.h>
#include <keystroke.h>
#include <scrKeyboard.h>
#include	"../../cellOS.h"

extern contactList* ourBlackBook;

class PNListItem;
class PNList;
class contactPanel;



// *****************************************************
// *******************  addrStarter  *******************
// *****************************************************
// This guy is just for starting up the background global
// contactList object. "ourBlackBook".


class addrStarter {

  public:
          addrStarter(void);
  virtual ~addrStarter(void);

          void  begin(char* filePath,bool resetFile=false);
};


// *****************************************************
// *******************  contNewBtn  ********************
// *****************************************************


class contNewBtn :  public newBtn {

   public:
          contNewBtn(PNList* ourList);
  virtual ~contNewBtn(void);

  virtual void    doAction(void);

          PNList* mList;

};


// *****************************************************
// ******************  contTextBtn  ********************
// *****************************************************


class contTextBtn : public textBtn {

  public:
          contTextBtn(void);
  virtual ~contTextBtn(void);

  virtual void    doAction(void);
};



// *****************************************************
// ******************  contCallBtn  ********************
// *****************************************************


class contCallBtn : public callBtn {

  public:
          contCallBtn(void);
  virtual ~contCallBtn(void);

  virtual void    doAction(void);
};



// *****************************************************
// ******************  contTrashBtn  *******************
// *****************************************************


class contTrashBtn : public trashBtn {

  public:
          contTrashBtn(PNList* ourList);
  virtual ~contTrashBtn(void);

  virtual void    doAction(void);
  
          PNList* mList;
};



// *****************************************************
// *******************  PNEditField  *******************
// *****************************************************


class PNEditField :  public cellEditField {

  public:
          PNEditField (rect* inRect,char* inText,keyboard* inKeyboard,PNListItem* ourListItem);
  virtual ~PNEditField(void);

  virtual void        drawSelf(void);
  virtual void        setFocus(bool setLoose);

          PNListItem* mOurListItem;
};



// *****************************************************
// *******************  PNListItem  ********************
// *****************************************************


class PNListItem :  public drawGroup {

  public:
          PNListItem(PNList* ourList,contact* inContact,keyboard* inKeyboard);
  virtual ~PNListItem(void);

  virtual void      draw(void);
  virtual void      drawSelf(void);
          void      startedEdit(void);
          void      finishEdit(PNEditField* theField);
          contact*  getContact(void);
          
          contact*      mContact;         // Who we represent.
          PNList*       mList;            // Where we live;
          PNEditField*  pNumEditField;    // Phone number editing field.
          PNEditField*  nickEditField;    // Repeat a lot..
          PNEditField*  fNameEditField;   //
          PNEditField*  lNameEditField; 
          PNEditField*  compEditField;
};



// *****************************************************
// *********************  PNList  **********************
// *****************************************************


class PNList : public scrollingList {

  public:
          PNList(int x,int y,int width,int height,keyboard* inKeyboard);
  virtual ~PNList(void);
  
  virtual void          drawSelf(void);
          void          addContact(contact* contactPtr,bool showContact=false);
          void          fillList(void);
          PNListItem*   itemByContact(contact* aContact);
          void          deleteContact(void);
  virtual void          doAction(event* inEvent,point* locaPt);

          keyboard*     mKeyboard;  // So we canpass on the pointer.
};



// *****************************************************
// ******************  contactPanel  *******************
// *****************************************************


class contactPanel :  public panel {

  public:
          contactPanel(void);
  virtual ~contactPanel(void);
           
  virtual void    setup(void);
  virtual void    loop(void);
  virtual void    drawSelf(void);
  virtual void    closing(void);

          PNList*   mPNList;
          keyboard* mKeyboard;
};



#endif
