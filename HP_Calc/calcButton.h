#ifndef calcButton_h
#define calcButton_h

#include <drawObj.h>

class calcButton : public drawObj {
  
  public :
                  calcButton(char* inFStr,word locX, word locY,byte width);
  
  virtual void    drawSelf(void);
  virtual void    doAction(void); 
  
  protected :
          char* fStr;
};

#endif
