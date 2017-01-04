#ifndef displayObj_h
#define displayObj_h

#include <colorObj.h>

// This is going to by our base display object. Similar to adafruit's GFX object.
// The plan is to inherit this for whatever glue code each piece of 
// hardware needs to run.

class displayObj {    

public :
  displayObj(bool inGraphic,bool inColor,bool inTouch,bool inSD);

  virtual void begin(void);
  virtual void setTextColor(colorObj* inColor);
  virtual void setTextSize(byte inSize);
  virtual void setCursor(int inX,int inY);
  virtual void drawText(char* inText);
  virtual void fillScreen(colorObj* inColor);

  boolean isGraphic(void);
  boolean isColor(void);
  boolean hasTouchScreen(void);
  boolean hadSDDrive(void);
  
  protected:
    boolean  graphic;
    boolean  color;
    boolean  hasTouch;
    boolean  hasSD;
};

#endif

