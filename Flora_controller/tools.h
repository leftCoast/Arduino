#ifndef tools_h
#define tools_h

#include <quickCom.h>
#include <drawObj.h>
#include <blinker.h>
#include <bmpPipe.h>
#include <fontLabel.h>

#define SEC_IN_HOUR 3600
#define SEC_IN_DAY  86400
#define SEC_IN_WEEK 604800


void timeFormatter(unsigned long sec);

// *****************************************************
//                      onlineFText
// *****************************************************


class onlineFText : public fontLabel,
                    public idler {
  public:
            onlineFText(int x, int y,int width, int height);
    virtual ~onlineFText(void);
    
    virtual void  setState(void)=0;
    virtual void  idle();
    virtual void  drawSelf(void);
    
            bool  mOnline;
};



// *****************************************************
//                      onlineText
// *****************************************************


class onlineText : public label,
                   public idler {
  public:
            onlineText(int x, int y,int width, int height);
    virtual ~onlineText(void);
    
    virtual void  setState(void)=0;
    virtual void  idle();
    
            bool  mOnline;
};


// *****************************************************
//                     stateText
// *****************************************************


class stateText : public onlineFText {

  public:
                stateText(int x, int y,int width, int height);
  virtual       ~stateText(void);

  virtual void  setValue(void);
  virtual void  setState(void);
  virtual void  idle();

          int   mStateVal;
 };


 
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
//                        flasher
// *****************************************************


// Copied directly from Flora_II maybe one day it'll be a library thing.
class flasher : public drawObj,
                public blinker {
    public:
                  flasher(rect* inRect,colorObj* backColor=&black);
                  flasher(int inLocX,int inLocY,int inWidth,int inHeight,colorObj* backColor=&black);
                     
    virtual       ~flasher(void);
    virtual void  setBlink(bool onOff);
    virtual void  setLight(bool onOff);
    virtual void  drawSelf(void);
    
            colorObj  mForeColor;
            colorObj  mBackColor;
};



// *****************************************************
//                     bmpFlasher
// *****************************************************


class bmpFlasher :  public flasher {
    
    public:
                  bmpFlasher(int inX,int inY, int width,int height,char* onBmp, char* offBmp);
                  bmpFlasher(rect* inRect,char* onBmp, char* offBmp);
    virtual       ~bmpFlasher(void);

            void  setup(char* onBmp, char* offBmp);
    virtual void  drawSelf(void);

            bool      mReady;
            bmpPipe*  mOnBmp;
            bmpPipe*  mOffBmp;     
};

// ******************************************
//                    plantBotCom
// ******************************************


#define PLANTBOT_NAME_SIZE  24    // The name has to fit in a string this size. IE ONE LESS.
#define READ_TIME     250         // We'll nap for this long between pestering the plant bot for background info.
#define MAX_WTIME     120000      // Limit your water time to 2 minutes.
#define MAX_STIME     600000      // Limit your soak time to 10 minutes.



enum floraComSet  {
                  floraReset,
                      
                  readName,
                  setName,
                  readDryLimit,
                  setDryLimit,
                  readWaterTime,
                  setWaterTime,
                  readSoakTime,
                  setSoakTime,
                  
                  readPulse,
                  pulseOn,
                  pulseOff,
                  
                  readPump,
                  pumpOn,
                  pumpOff,

                  readLogging,
                  loggingOn,
                  loggingOff,
                  readLogSize,
                  readLogLines,
                  readLogWLines,
                  clearLog,
                  readLogBuff,
             
                  readState,
                  readTemp,
                  readMoisture
                  };
 
enum floraReplySet  { noErr, unknownCom, badParam }; // Notice we only look for noErr and just toss the rest out? LAZY!


class plantBotCom : public qCMaster {

  public:
                plantBotCom(void);
  virtual       ~plantBotCom(void);

          bool  getByte(byte com,byte* reply);
          bool  getInt(byte com,int* reply);
          bool  getFloat(byte com,float* reply);
          bool  getLong(byte com,long* reply);
          bool  getUnsignedLong(byte com,unsigned long* reply);
          bool  getCString(byte com,char* reply);
          bool  sendCommand(byte com);
          
          char*         getName(void) ;
          int           getLimit(void);
          int           getWaterTime(void) ;
          int           getSoakTime(void);
          int           getPulse(void);
          int           getPump(void);
          int           getLogging(void);
          unsigned long getLogSize(void);
          unsigned long getLogLines(void);
          unsigned long getLogWLines(void);
          int           getState(void);
          int           getTemp(void);
          int           getMoisture(void);
          
          bool  setNameReg(char* name);
          bool  setLimitReg(int limit);
          bool  setWTimeReg(int seconds);
          bool  setSTimeReg(int seconds);
          bool  setTimeReg(floraComSet timeType,int seconds);
          bool  setWaterReg(bool onOff);
          bool  setPulseReg(bool onOff);
          bool  setLoggingReg(bool onOff);
          bool  copyLogCom(char* filemane);
          bool  clearLogCom(void);
          
          void  updateTime(void);           // MUST be called repeatedly by loop() NOT in idle().
          bool  getOnline(void);
          
  protected:
          void  setUpdateTime(void);
          void  sleep(int ms);
          
          int           mIndex;
          bool          mOnline;
          timeObj       mSleepTimer;
          timeObj       mUpdateTimer;
          
          char          mName[PLANTBOT_NAME_SIZE];
          byte          mLimit;
          long          mWaterTime;
          long          mSoakTime;
          byte          mPulse;
          byte          mPump;
          byte          mLogging;
          unsigned long mLogSize;
          unsigned long mLogNumLines;
          unsigned long mLogNumWLines;
          byte          mState;
          byte          mTemp;
          byte          mMoisture;         
};


extern  plantBotCom ourComPort;


#endif