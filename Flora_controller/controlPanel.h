#ifndef controlPanel_h
#define controlPanel_h

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
  virtual void  setup(void);
  virtual void  loop(void);
  virtual void  drawSelf(void);
  virtual void  close(void);
  virtual void  closing(void);
};

#endif