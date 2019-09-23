#ifndef homePanel_h
#define homePanel_h

#include <panel.h>
#include <litlOS.h> 
#include <bmpObj.h> 
#include <fontLabel.h> 
#include "tools.h"


// *****************************************************
//                       waterBtn
// *****************************************************


class waterBtn :  public bmpObj,
                  public onlineIntStateTracker,
                  public idler
                  {

    public:
            waterBtn(int x, int y,int width, int height);
    virtual ~waterBtn(void);
    
            void  setTheLook();
    virtual void  readState(void);
    virtual void  doAction(void);
    virtual void  idle(void);
};



// *****************************************************
//                      homeScreen
// *****************************************************


class homeScreen : public homePanel {

  public:
          homeScreen(void);
  virtual ~homeScreen(void);
  
  virtual void  setup(void);
  virtual void  loop(void);
  virtual void  draw(void);
  virtual void  drawSelf(void);

          fontLabel*  mNameLabel;
          waterBtn*   mWaterBtn;
};



#endif
