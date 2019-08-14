#ifndef controlPanel_h
#define controlPanel_h

#include "label.h"
#include "panel.h"
#include "baseButton.h"
#include "slider.h"

class controlPanel;

// *****************************************************
// ******************    pumpBtn   *********************
// *****************************************************


class pumpBtn : public baseButton {

    public:
            pumpBtn(int x, int y,int width, int height);
    virtual ~pumpBtn(void);

    virtual void  doAction(event* inEvent,point* locaPt);

            bool          mOnOff;
};



// *****************************************************
// ******************   selectBtn   ********************
// *****************************************************


class selectBtn : public baseButton {

  public:
                selectBtn(int x,int y,int width,int height,int inChoice);
  virtual       ~selectBtn(void);

          int     getChoice(void);
  virtual void    drawSelf(void);
  virtual void    doAction(event* inEvent,point* locaPt);

          int     mChoice;
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
                  okBtn(int x, int y,int width, int height);
    virtual       ~okBtn(void);

    virtual void  doAction(event* inEvent,point* locaPt);
};


// *****************************************************
// ****************    cancelBtn    ********************
// *****************************************************


class cancelBtn : public baseButton {

  public:
                  cancelBtn(int x, int y,int width, int height);
  virtual         ~cancelBtn(void);

  virtual   void  doAction(event* inEvent,point* locaPt);
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
          
  virtual void  loop(void);
  virtual void  drawSelf(void);
  virtual void  close(void);
  virtual void  closing(void);

          timeObj     mReadTimer;
          label*      mNameLabel;
          label*      mMoistureLabel;
          label*      mDryLimitLabel;
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
