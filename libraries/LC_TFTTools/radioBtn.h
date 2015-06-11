#ifndef radioBtn_h
#define radioBtn_h

#include "touchObj.h"

class radioBtnSet;

class radioBtn : public touchObj {
  
  public:
    radioBtn(void);
    ~radioBtn(void);
    
    virtual void setSet(radioBtnSet* inSet);
    virtual void setState(byte inState);
   
    radioBtnSet* ourSet;
};


class radioBtnSet : public touchObj, public linkList {
  
  public:
    radioBtnSet(void);
    ~radioBtnSet(void);
    
    virtual void    addObj(radioBtn* newObj);
    virtual void    draw(void);
    virtual void    drawSelf(void);
    virtual boolean checkTouch(TS_Point where);
    virtual void    setStates(radioBtn* hitBtn);
};


#endif
  
