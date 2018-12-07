#ifndef labelButton_h
#define labelButton_h

#include <label.h>

 
class labelButton : public label {
  
  public :
          labelButton(char* inLabel,word locX, word locY,byte width,byte height);
  virtual ~labelButton(void);          
                  
  virtual void    drawSelf(void);
  virtual void    doAction(void);
};

#endif
