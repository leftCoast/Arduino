#ifndef contactPanel_h
#define contactPanel_h

#include <drawObj.h>
#include <panel.h>

#define PNLIST_X    10
#define PNLIST_Y    20
#define PNLIST_W    220
#define PNLIST_H    200 //?

#define PN_ITEM_X   0
#define PN_ITEM_Y   0   // Doesn't really matter, it scrolls.
#define PN_ITEM_W   PNLIST_W
#define PN_ITEM_H   40


extern addrBook ourBlackBook;


// *****************************************************
// *******************  PNEditField  *******************
// *****************************************************


class PNEditField :  public drawGroup {

  public:
          PNEditField (rect* inRect,char* inText);
          ~PNEditField(void);
          
  virtual drawSelf(void);
  
          editField*  mEditField;    // Our editing field.
          colorRect*  mEditBase;     // Our editing field's base.     
};



// *****************************************************
// *******************  PNListItem  ********************
// *****************************************************


class PNListItem :  public drawGroup {

  public:
          PNListItem(rect,contact* inContact);
  virtual ~PNListItem(void);

          contact*      mContact;         // Who we represent.
          PNEditField*  pNumEditField;    // Phone number editing field.
          PNEditField*  nickEditField;    // Repeat a lot..
          PNEditField*  fNameEditField;   //
          PNEditField*  lNameEditField; 
          PNEditField*  compEditField;
          IOandKeys*    ourKeyboard;    // Just like it sounds, our texting keyboard.
};



// *****************************************************
// *********************  PNList  **********************
// *****************************************************

class PNList : public viewList {

  public:
          PNList(rect);
  virtual ~PNList(void);

          void  showAll(void);
};



// *****************************************************
// ******************  contactPanel  *******************
// *****************************************************


class contactPanel :  public panel {

  virtual  void  setup(void);
  virtual void  loop(void);
  virtual void  drawSelf(void);
  virtual void  close(void);
  virtual void  closing(void);
};



#endif
