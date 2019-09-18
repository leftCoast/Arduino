#ifndef homePanel_h
#define homePanel_h

#include <panel.h>
#include <litlOS.h> 
#include <bmpObj.h> 
#include <fontLabel.h> 
#include "baseButton.h"
#include "tools.h"


// *****************************************************
//                     nameText
// *****************************************************

class nameText : public onlineFText {

  public:
                nameText(int x, int y,int width, int height);
  virtual       ~nameText(void);

  virtual void  setValue(void);
  virtual void  setState(void);
  virtual void  idle();
 };



// *****************************************************
//                     sTimeText
// *****************************************************

class sTimeText : public onlineText {

  public:
                sTimeText(int x, int y,int width, int height);
  virtual       ~sTimeText(void);

  virtual void  setValue(void);
  virtual void  setState(void);
  virtual void  idle();
  
          int   mSoakTime;
};

          
// *****************************************************
//                     wTimeText
// *****************************************************


class wTimeText : public onlineText {

  public:
                wTimeText(int x, int y,int width, int height);
  virtual       ~wTimeText(void);

  virtual void  setValue(void);
  virtual void  setState(void);
  virtual void  idle();
  
          int   mWaterTime;
};

          
// *****************************************************
//                     limitText
// *****************************************************


class limitText : public onlineText {

  public:
                limitText(int x, int y,int width, int height);
  virtual       ~limitText(void);

  virtual void  setValue(void);
  virtual void  setState(void);
  virtual void  idle();
  
          int   mLimit;
};



// *****************************************************
//                   totalWaterText
// *****************************************************


class totalWaterText : public onlineText {

  public:
                totalWaterText(int x, int y,int width, int height);
  virtual       ~totalWaterText(void);

  virtual void  setValue(void);
  virtual void  setState(void);
  virtual void  idle();
  
          int   mLogWetLines;
          float lPerSec;
};



// *****************************************************
//                     moistureText
// *****************************************************


class moistureText : public onlineText {

  public:
                moistureText(int x, int y,int width, int height);
  virtual       ~moistureText(void);

  virtual void  setValue(void);
  virtual void  setState(void);
  virtual void  idle();
  
          int   mMoisture;
};

// *****************************************************
//                   totalTimeText
// *****************************************************


class totalTimeText : public onlineText {

  public:
                totalTimeText(int x, int y,int width, int height);
  virtual       ~totalTimeText(void);

  virtual void  setValue(void);
  virtual void  setState(void);
  virtual void  idle();
  
          int   mLogLines;
};


// *****************************************************
//                     tempText
// *****************************************************


class tempText : public onlineText {

  public:
                tempText(int x, int y,int width, int height);
  virtual       ~tempText(void);

  virtual void  setValue(void);
  virtual void  setState(void);
  virtual void  idle();
  
          int   mTemp;
};



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
