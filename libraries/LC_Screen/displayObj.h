#ifndef displayObj_h
#define displayObj_h

#include <colorObj.h>

// This is going to by our base display object. Similar to adafruit's GFX object.
// The plan is to inherit this for whatever glue code each piece of 
// hardware needs to run.

  struct point {
    word x;
    word y;
  };
  

class displayObj {    

public :
  displayObj(bool inGraphic,bool inColor,bool inTouch,bool inSD,bool inReadable);

  virtual boolean dispObjBegin(void);
  virtual void setRotation(byte inRotation);
  virtual void setTextColor(colorObj* tColor);
  virtual void setTextColor(colorObj* tColor,colorObj* bColor);
  virtual void setTextSize(byte inSize);
  virtual void setTextWrap(boolean inWrap);
  virtual void setCursor(word inX,word inY);
  virtual void drawText(char* inText);
  virtual void fillScreen(colorObj* inColor);
  virtual void fillRect(word locX,word locY,word width,word height, colorObj* inColor);
  virtual void drawRect(word locX,word locY,word width,word height, colorObj* inColor);
  virtual void fillRectGradient(word inX,word inY,word inXSize,word inYSize,colorObj* startColor,colorObj* endColor,boolean vertical=true);
  virtual void fillScreenGradient(colorObj* startColor,colorObj* endColor,boolean vertical=true);
  virtual void fillRoundRect(word locX,word locY,word width,word height,word radius,colorObj* inColor);
  virtual void drawRoundRect(word locX,word locY,word width,word height,word radius,colorObj* inColor);
  virtual void drawCircle(word locX,word locY,word inDiam, colorObj* inColor);
  virtual void fillCircle(word locX,word locY,word inDiam, colorObj* inColor);
  virtual void drawVLine(word locX,word locY,word height,colorObj* inColor);
  virtual void drawHLine(word locX,word locY,word width,colorObj* inColor);
  virtual void drawLine(word locX,word locY,word locX2,word locY2,colorObj* inColor);
  virtual void drawPixel(word x,word y,colorObj* pColor);
    
  virtual void drawPixelInvert(word x,word y);
  virtual void frameRectInvert(word x,word y,word width,word height);

  virtual point     getPoint(void);
  virtual boolean   touched(void);
  
  boolean isGraphic(void);
  boolean isColor(void);
  boolean hasTouchScreen(void);
  boolean hadSDDrive(void);
  boolean canRead(void);
  
  protected:
    boolean  graphic;
    boolean  color;
    boolean  hasTouch;
    boolean  hasSD;
    boolean  readable;
};

#endif

