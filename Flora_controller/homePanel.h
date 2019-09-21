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


class waterBtn : public baseIconButton {

    public:
            waterBtn(int x, int y,int width, int height);
    virtual ~waterBtn(void);
    
            void  setTheLook();
    virtual void  doAction(event* inEvent,point* locaPt);

            bool  mOnOff;
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
  virtual void  drawSelf(void);

          fontLabel*  mNameLabel;
          waterBtn*   mWaterBtn;
};



#endif
