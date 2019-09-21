#ifndef controlPanel_h
#define controlPanel_h

#include "globals.h"
#include "floraOS.h"
#include "label.h"
#include "panel.h"
#include "slider.h"
#include "tools.h"
#include <colorRect.h>
#include <blinker.h>
#include <bmpPipe.h>
#include <fontLabel.h>

class controlPanel;


// *****************************************************
// ******************   selectBtn   ********************
// *****************************************************


class selectBtn : public baseButton {

  public:
                selectBtn(int x,int y,int width,int height,int inChoice,controlPanel* inPanel);
  virtual       ~selectBtn(void);

          int     getChoice(void);
  virtual void    drawSelf(void);
  virtual void    doAction(event* inEvent,point* locaPt);

          int           mChoice;
          controlPanel* mPanel;
};

extern  selectBtn*  selectedVal;



// *****************************************************
// *****************   editSlider   ********************
// *****************************************************


class editSlider :  public slider {
  
  public:
                editSlider(int x,int y,int width,int height);
  virtual       ~editSlider(void);

  virtual void  setLabel(label* inLabel);
  virtual void  setText(void);
  virtual void  select(bool onOff);
  virtual void  doAction(event* inEvent,point* locaPt);

          label*  mLabel;
};


// *****************************************************
// ******************    okBtn    **********************
// *****************************************************


class okBtn : public baseIconButton {

    public:
                  okBtn(int x, int y,int width, int height,controlPanel* inPanel);
    virtual       ~okBtn(void);

    virtual void  doAction(event* inEvent,point* locaPt);

            controlPanel* mPanel;
};



// *****************************************************
// ****************    cancelBtn    ********************
// *****************************************************


class cancelBtn : public baseIconButton {

  public:
                  cancelBtn(int x, int y,int width, int height,controlPanel* inPanel);
  virtual         ~cancelBtn(void);

  virtual   void  doAction(event* inEvent,point* locaPt);
  
            controlPanel* mPanel;
};


     
// *****************************************************
//                    controlPanel 
// *****************************************************

class controlPanel : public panel {

  public:
          controlPanel(void);
  virtual ~controlPanel(void);
          
  virtual void  setup(void);
          void  doComSetDryLimit(float limit);
          void  doComSetWTime(float wTime);
          void  doComSetSTime(float sTime);
  virtual void  drawSelf(void);

          selectBtn*    mDryLimitBtn;
          selectBtn*    mWaterTimeBtn;
          selectBtn*    mSoakTimeBtn;
          timeText*     mSliderTLabel;
          percentText*  mSliderPLabel;
          editSlider*   mSlider;
          okBtn*        mOkBtn;
          cancelBtn*    mCancelBtn;
};

#endif
