#ifndef bmpKeyboard_h
#define bmpKeyboard_h

#include <scrKeyboard.h>
#include <bmpObj.h>


// *****************************************************
//                       bmpKeybaard
// *****************************************************


class bmpKeyboard : public keyboard {

  public:
                bmpKeyboard(editable* inEditObj,bool modal=false);
  virtual       ~bmpKeyboard(void);

  virtual void  loadKeys(void);
          int   col(int col, int row);
  
  protected:
  
          bool    mModal;
};



// *****************************************************
//                       bmpInputKey
// *****************************************************


class bmpInputKey :  public inputKey {

  public:
          bmpInputKey(char* inLabel,char* inNum,char* inSym,int inX,int inY,int inWidth,int inHeight,bmpKeyboard* inKeyboard);
  virtual ~bmpInputKey(void);

  virtual void    drawSelf(void);
};



// *****************************************************
//                       bmpControlKey
// *****************************************************


class bmpControlKey : public controlKey {

  public:
          bmpControlKey(char* inLabel,keyCommands inCom,int inX, int inY,int inWidth,int inHeight,bmpKeyboard* inKeyboard,char* bmpPath);
  virtual ~bmpControlKey(void);

  virtual void    drawSelf(void);
          
          bmpPipe mBmpPipe;
};


#endif
