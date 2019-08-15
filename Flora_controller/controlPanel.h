#ifndef controlPanel_h
#define controlPanel_h

#include "label.h"
#include "panel.h"
#include "baseButton.h"
#include "slider.h"

class controlPanel;

// *****************************************************
// ******************    waterBtn   *********************
// *****************************************************


class waterBtn : public baseButton {

    public:
            waterBtn(int x, int y,int width, int height);
    virtual ~waterBtn(void);

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

          void  moistureText(int moisture);
          void  dryLimitText(int dryLimit);
          void  waterTimeText(unsigned long wTime);
          void  soakTimeText(unsigned long sTime);
  virtual void  setup(void);

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
          label*      mMoistureLabel;
          label*      mDryLimitLabel;
          waterBtn*   mWaterBtn;
          selectBtn*  mDryLimitBtn;
          label*      mWaterTimeLabel;
          selectBtn*  mWaterTimeBtn;
          label*      mSoakTimeLabel;
          selectBtn*  mSoakTimeBtn;
          label*      mSliderLabel;
          editSlider* mSlider;
          okBtn*      mOkBtn;
          cancelBtn*  mCancelBtn;
};

#endif
