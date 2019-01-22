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

#define PNLIST_X      10
#define PNLIST_Y      20
#define PNLIST_W      220


#define PN_ITEM_X1    0
#define PN_ITEM_Y     0   // Doesn't really matter, it scrolls.
#define PN_ITEM_W     PNLIST_W
#define PN_ITEM_H     42
#define PN_ITEM_LX    30
#define PN_ITEM_LX2   PNLIST_W - PN_ITEM_LX
#define PN_ITEM_LY    PN_ITEM_H - 2

#define PNLIST_H      PN_ITEM_H * 4

#define PN_EITEM_SW   68
#define PN_EITEM_LW   104
#define PN_EITEM_H    15 

#define PN_EITEM_SX   4
#define PN_EITEM_SY   4
#define PN_EITEM_LX   4
#define PN_EITEM_LY   PN_EITEM_SY + PN_EITEM_H + 3

#define PN_EITEM_SG   4
#define PN_EITEM_LG   4

#define PN_EITEM_SX2  PN_EITEM_SX + PN_EITEM_SW + PN_EITEM_SG
#define PN_EITEM_SX3  PN_EITEM_SX2 + PN_EITEM_SW + PN_EITEM_SG
#define PN_EITEM_LX2  PN_EITEM_LX + PN_EITEM_LW + PN_EITEM_LG

#define PN_EITEM_INSET  3

#define CONT_BTN_RAD    3
#define CONT_BTN_SPC    20

#define CLOSE_X         PNLIST_X
#define CLOSE_Y         PNLIST_Y + PNLIST_H + 2
#define CLOSE_W         22
#define CLOSE_H         CLOSE_W
#define CLOSE_TXT_SIZE  2
#define CLOSE_CHAR_W    CLOSE_TXT_SIZE * 6
#define CLOSE_TXT_H     CLOSE_TXT_SIZE * 8

#define NEW_X         CLOSE_X + CLOSE_W + CONT_BTN_SPC
#define NEW_Y         CLOSE_Y
#define NEW_W         CLOSE_W
#define NEW_H         CLOSE_H
#define NEW_PN        "5551212"

#define TEXT_X        NEW_X + NEW_W + CONT_BTN_SPC
#define TEXT_Y        CLOSE_Y
#define TEXT_W        CLOSE_W
#define TEXT_H        CLOSE_H

#define CALL_X        TEXT_X + TEXT_W + CONT_BTN_SPC
#define CALL_Y        CLOSE_Y
#define CALL_W        CLOSE_W
#define CALL_H        CLOSE_H

#define TRASH_X       CALL_X + CALL_W + CONT_BTN_SPC
#define TRASH_Y       CLOSE_Y
#define TRASH_W       CLOSE_W
#define TRASH_H       CLOSE_H


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

          void  begin(char* filePath);
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
          contTrashBtn(void);
  virtual ~contTrashBtn(void);

  virtual void    doAction(void);
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
  
  virtual void  drawSelf(void);
          void  addContact(contact* contactPtr,bool showContact=false);
          void  fillList(void);
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
