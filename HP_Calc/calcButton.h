#ifndef calcButton_h
#define calcButton_h

#include <drawObj.h>

#define NUMBER_BTN  0
#define EDIT_BTN    1
#define FX_BTN      2

class calcButton : public drawObj {
  
  public :
                  calcButton(char* inFStr,word inLocX, word inLocY,byte width,byte inType);
                  //calcButton(char* inFStr,char* inAFStr, word inLocX, word inLocY,byte width,byte inType);
          //void    setInvert(void);
          //void    clearInvert(void);
  virtual void    drawSelf(void);
  virtual void    doAction(void); 
  
  protected :
          char* fStr;
          //char* aFStr;
          byte  bType;
          //boolean invert;
};

extern void setupButtonColors(void);

#endif
