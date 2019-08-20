#ifndef controlPanel_h
#define controlPanel_h

#include "label.h"
#include "panel.h"
#include "baseButton.h"
#include "slider.h"
#include <colorRect.h>

class controlPanel;


// *****************************************************
// ******************    waterBtn   ********************
// *****************************************************


class waterBtn : public baseButton {

    public:
            waterBtn(int x, int y,int width, int height);
    virtual ~waterBtn(void);
    
            void  setTheLook();
    virtual void  doAction(event* inEvent,point* locaPt);

            bool          mOnOff;
};



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


class okBtn : public baseButton {

    public:
                  okBtn(int x, int y,int width, int height,controlPanel* inPanel);
    virtual       ~okBtn(void);

    virtual void  doAction(event* inEvent,point* locaPt);

            controlPanel* mPanel;
};



// *****************************************************
// ****************    cancelBtn    ********************
// *****************************************************


class cancelBtn : public baseButton {

  public:
                  cancelBtn(int x, int y,int width, int height,controlPanel* inPanel);
  virtual         ~cancelBtn(void);

  virtual   void  doAction(event* inEvent,point* locaPt);
  
            controlPanel* mPanel;
};


     
// *****************************************************
// ****************   controlPanel   *******************
// *****************************************************


class controlPanel : public panel {

  public:
          controlPanel(void);
  virtual ~controlPanel(void);
          
          void  stateText(int state);
          void  moistureText(int moisture);
          void  dryLimitText(int dryLimit);
          void  waterTimeText(long wTime);
          void  soakTimeText(long sTime);
  virtual void  setup(void);

          void  checkState(void);
          void  checkMoisture(void);
          void  checkDryLimit(void);
          void  checkWaterTime(void);
          void  checkSoakTime(void);
          void  checkName(void);
          
          void  doComPump(bool onOff);

          void  doComSetDryLimit(float limit);
          void  doComSetWTime(float wTime);
          void  doComSetSTime(float sTime);
          
  virtual void  loop(void);
  virtual void  drawSelf(void);
  virtual void  close(void);
  virtual void  closing(void);

          int         mLastLimit;
          int         mLastWTime;
          int         mLastSTime;
          
          timeObj     mReadTimer;
          label*      mNameLabel;
          colorRect*  mNameLight;
          label*      mStateLabel;
          colorRect*  mStateLight;
          label*      mMoistureLabel;
          colorRect*  mMoistureLight;
          label*      mDryLimitLabel;
          colorRect*  mDryLimitLight;
          waterBtn*   mWaterBtn;
          selectBtn*  mDryLimitBtn;
          label*      mWaterTimeLabel;
          colorRect*  mWaterTimeLight;
          selectBtn*  mWaterTimeBtn;
          label*      mSoakTimeLabel;
          colorRect*  mSoakTimeLight;
          selectBtn*  mSoakTimeBtn;
          label*      mSliderLabel;
          editSlider* mSlider;
          okBtn*      mOkBtn;
          cancelBtn*  mCancelBtn;
};

#endif
