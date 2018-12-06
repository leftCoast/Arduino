#ifndef labelButton_h
#define labelButton_h

#include <idlers.h>
#include <drawObj.h>

 
class labelButton : public drawObj, public idler {
  
  public :
          labelButton(char* inFStr,word inLocX, word inLocY,byte width,byte height);
  virtual ~labelButton(void);          
                  
  virtual void    idle(void);
  virtual void    drawSelf(void);
  virtual void    doAction(void); 
  virtual void    beep(void);
          
  protected :
          char*   fStr;
};

#endif
