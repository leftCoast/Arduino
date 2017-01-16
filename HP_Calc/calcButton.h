#ifndef calcButton_h
#define calcButton_h

#include <idlers.h>
#include <drawObj.h>

#define NUMBER_BTN  0
#define EDIT_BTN    1
#define FX_BTN      2
#define DEG_RAD_BTN 3

#define BEEP_PIN    23

extern void setupButtonColors(void);

class calcButton : public drawObj, public idler {
  
  public :
                  calcButton(char* inFStr,word inLocX, word inLocY,byte width,byte inType);
                  calcButton(char* inFStr,char* inAFStr, word inLocX, word inLocY,byte width,byte inType);
                  
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
  

class degRadButton : public calcButton {

 public :
                  degRadButton(word inLocX, word inLocY,byte width, byte height);

          void    setColors(colorObj* inTColor,colorObj* inHTColor,colorObj* inBColor);
  virtual void    drawSelf(void);
  virtual void    doAction(void);

          colorObj tColor;
          colorObj hColor;
          colorObj bColor;
};

#endif
