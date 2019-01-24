#ifndef contactPanel_h
#define contactPanel_h

#include <drawObj.h>
#include <label.h>
#include <editField.h>
#include <panel.h>
#include <contactList.h>
#include <colorRect.h>
#include <keystroke.h>
#include <keyboard.h>


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
// ******************  contCloseBtn  *******************
// *****************************************************



class contCloseBtn :  public closeBtn {

   public:
          contCloseBtn(contactPanel* ourPanel);
  virtual ~contCloseBtn(void);

  virtual void  doAction(void);

          contactPanel* mPanel;
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


class PNEditField :  public drawGroup {

  public:
          PNEditField (rect* inRect,char* inText,PNListItem* ourListItem);
  virtual ~PNEditField(void);

  virtual void  drawSelf(void);
  virtual void  setFocus(bool setLoose);
  virtual void  doAction(void);
          int   getNumChars(void);      // Not including the \0. You may need to add one.
          void  getText(char* inBuff);  // You better have added the (1) for the \0.

          PNListItem* mOurListItem;
          editField*  mEditField;       // Our editing field.
          colorRect*  mEditBase;        // Our editing field's base.     
};



// *****************************************************
// *******************  PNListItem  ********************
// *****************************************************


class PNListItem :  public drawGroup {

  public:
          PNListItem(PNList* ourList,contact* inContact);
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

class PNList : public drawList {

  public:
          PNList(int x,int y,int width,int height);
  virtual ~PNList(void);
  
  virtual void          drawSelf(void);
          void          addContact(contact* contactPtr,bool showContact=false);
          void          fillList(void);
          PNListItem*   itemByContact(contact* aContact);
          void          deleteContact(void);
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

          PNList* mPNList;
};



#endif
