#ifndef calcButton_h
#define calcButton_h

#include <touchObj.h>

class calcButton : public touchObj {
  
  public :
                  calcButton(char* inFStr,word locX, word locY,byte width, byte height);
  
  virtual void    drawSelf(void);
  virtual void    doAction(TS_Point where); 
  
  protected :
          char* fStr;
};

#endif
