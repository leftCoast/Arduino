#ifndef controlPanel_h
#define controlPanel_h

#include "label.h"
#include "panel.h"
#include "baseButton.h"



// *****************************************************
// ******************    pumpBtn   *********************
// *****************************************************


class pumpBtn : public baseButton {

    public:
            pumpBtn(int x, int y,int width, int height);
    virtual ~pumpBtn(void);

    virtual void  doAction(event* inEvent,point* locaPt);
};



// *****************************************************
// ****************   controlPanel   *******************
// *****************************************************


class controlPanel : public panel {

  public:
          controlPanel(void);
  virtual ~controlPanel(void);

          void  moistureText(int moisture);
          void  waterTimeText(unsigned long wTime);
          void  soakTimeText(unsigned long sTime);
  virtual void  setup(void);
  virtual void  loop(void);
  virtual void  drawSelf(void);
  virtual void  close(void);
  virtual void  closing(void);

  timeObj mReadTimer;
  label*  mMoistureLabel;
  label*  mWaterTimeLabel;
  label*  mSoakTimeLabel;
};

#endif
