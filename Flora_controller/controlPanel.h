#ifndef controlPanel_h
#define controlPanel_h

#include "floraOS.h"
#include "label.h"
#include "panel.h"
#include "tools.h"
#include <colorRect.h>
#include <blinker.h>
#include <bmpPipe.h>
#include <fontLabel.h>
#include <liveText.h>



// *****************************************************
//                    editName 
// *****************************************************


class editName : public monoNameText {

   public:
          editName(int x,int y,int width,int height,keyboard* inKeyboard);
  virtual ~editName(void);

  virtual void  endEditing(void);
  virtual void  doAction(event* inEvent,point* locaPt);
  virtual void  idle(void);
  
          keyboard* mKeyboard;
};



// *****************************************************
//                    editLimit 
// *****************************************************


class editLimit : public moistureLimitText {

   public:
          editLimit(int x,int y,int width,int height,keyboard* inKeyboard);
  virtual ~editLimit(void);

  virtual void  endEditing(void);
  virtual void  doAction(event* inEvent,point* locaPt);
  virtual void  idle(void);
  
          keyboard* mKeyboard;
};



// *****************************************************
//                    editWTime 
// *****************************************************


class editWTime : public waterTimeText {

   public:
          editWTime(int x,int y,int width,int height,keyboard* inKeyboard);
  virtual ~editWTime(void);

  virtual void  endEditing(void);
  virtual void  doAction(event* inEvent,point* locaPt);
  virtual void  idle(void);
  
          keyboard* mKeyboard;
};



// *****************************************************
//                    editSTime 
// *****************************************************


class editSTime : public soakTimeText {

   public:
          editSTime(int x,int y,int width,int height,keyboard* inKeyboard);
  virtual ~editSTime(void);

  virtual void  endEditing(void);
  virtual void  doAction(event* inEvent,point* locaPt);
  virtual void  idle(void);
  
          keyboard* mKeyboard;
};



// *****************************************************
//                    controlPanel 
// *****************************************************


class controlPanel : public panel {

  public:
          controlPanel(void);
  virtual ~controlPanel(void);
          
  virtual void  setup(void);
          void  doComSetDryLimit(float limit);
          void  doComSetWTime(float wTime);
          void  doComSetSTime(float sTime);
          void  msg(char* text);
  virtual void  drawSelf(void);

          ourKeyboard*  mKeyboard;
          liveText*     mMessage;
};


#endif
