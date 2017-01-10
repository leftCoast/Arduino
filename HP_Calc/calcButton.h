#ifndef calcButton_h
#define calcButton_h

#include <drawObj.h>
#include <idlers.h>

#define NUMBER_BTN  0
#define EDIT_BTN    1
#define FX_BTN      2

#define BEEP_PIN    23

extern void setupButtonColors(void);

class calcButton : public drawObj, public idler {
  
  public :
                  calcButton(char* inFStr,word inLocX, word inLocY,byte width,byte inType);
                  calcButton(char* inFStr,char* inAFStr, word inLocX, word inLocY,byte width,byte inType);
          //void    setSecond(boolean setClear);
  virtual void    idle(void);
  virtual void    drawSelf(void);
  virtual void    doAction(void); 
          void    beep(void);
          
  protected :
          char*   fStr;
          char*   aFStr;
          byte    bType;
          boolean secondFx;
};


class secondfxButton : public calcButton {

  public :
                  secondfxButton(char* inFStr,word inLocX, word inLocY,byte width,byte inType);

  virtual void    idle(void);
  virtual void    doAction(void);

};
  
/*
class degRadButton : public calcButton {

 public :
                  degRadButton(char* inFStr,word inLocX, word inLocY,byte width,byte inType);

  virtual void    doAction(void);

};
*/


  
}
#endif
