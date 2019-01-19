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

#define PNLIST_X    10
#define PNLIST_Y    20
#define PNLIST_W    220
#define PNLIST_H    192

#define PN_ITEM_X1  0
#define PN_ITEM_Y   0   // Doesn't really matter, it scrolls.
#define PN_ITEM_W   PNLIST_W
#define PN_ITEM_H   48

#define PN_EITEM_SW 60
#define PN_EITEM_LW 97
#define PN_EITEM_H  16 

#define PN_EITEM_SX 10
#define PN_EITEM_SY  7
#define PN_EITEM_LX 10
#define PN_EITEM_LY PN_EITEM_SY + PN_EITEM_H + 3

#define PN_EITEM_SG 10
#define PN_EITEM_LG  6

#define PN_EITEM_SX2 PN_EITEM_SX + PN_EITEM_SW + PN_EITEM_SG
#define PN_EITEM_SX3 PN_EITEM_SX2 + PN_EITEM_SW + PN_EITEM_SG
#define PN_EITEM_LX2 PN_EITEM_LX + PN_EITEM_LW + PN_EITEM_LG

#define PN_EITEM_INSET  3


extern contactList* ourBlackBook;

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
// *******************  PNEditField  *******************
// *****************************************************


class PNListItem;


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
          PNListItem(contact* inContact);
  virtual ~PNListItem(void);
  
  virtual void  drawSelf(void);
          void  finishEdit(PNEditField* theField);
          
          contact*      mContact;         // Who we represent.
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
          void  fillList(void);
};



// *****************************************************
// ******************  contactPanel  *******************
// *****************************************************


class contactPanel :  public panel {

  public:
          contactPanel(void);
  virtual ~contactPanel(void);
           
  virtual void  setup(void);
  virtual void  loop(void);
  virtual void  drawSelf(void);
  virtual void  close(void);
  virtual void  closing(void);

          PNList*         mPNList;
};



#endif
