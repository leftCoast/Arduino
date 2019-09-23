#ifndef tools_h
#define tools_h

#include <quickCom.h>
#include <drawObj.h>
#include <blinker.h>
#include <bmpPipe.h>
#include <fontLabel.h>
#include "stateTracker.h"

#define SEC_IN_HOUR 3600
#define SEC_IN_DAY  86400
#define SEC_IN_WEEK 604800

// Screen labels. Pictures look sooo much better than drawing them.
#define CURR_MOIST_BMP  "/images/currM.bmp"
#define CURR_TEMP_BMP   "/images/currT.bmp"
#define TOTAL_WATER_BMP "/images/totalW.bmp"
#define TOTAL_TIME_BMP  "/images/totalT.bmp"
#define LIMIT_BMP       "/images/limit.bmp"
#define WATER_TIME_BMP  "/images/wTime.bmp"
#define SOAK_TIME_BMP   "/images/sTime.bmp"

#define WATER_ON_BMP  "/icons/H2OOn32.bmp"
#define WATER_OFF_BMP "/icons/H2OOff32.bmp"
#define ON_GREEN_BMP  "/icons/grnLED1.bmp"
#define OFF_GREEN_BMP "/icons/grnLED0.bmp"



// *****************************************************
//                      timeText
// *****************************************************

// This is a special that does the short time format to an inputted seconds.
// It displays time as one best fit unit. s,m,h,d,w as a number with one
// decimal place. Not perfectly accurate, but close enough for humans.
// For example put in 125 seconds and it'll put out "2.0 m".


void  timeFormatter(unsigned long sec); // Its out here for anyone to use. Drop a seconds value in here..
extern  char  timeStrBuf[];             // Read out the formated string result here. Easy Peasy!


class timeText :  public label {

  public:
            timeText(int x, int y,int width, int height);
    virtual ~timeText(void);
    
    virtual void  setValue(unsigned long seconds);
};


// *****************************************************
//                      percentText
// *****************************************************

// Like int time text below. We format as a percent but its an int
// So its just 0..100 Eg : "46 %"
class percentText :  public label {

  public:
            percentText(int x, int y,int width, int height);
    virtual ~percentText(void);
    
    virtual void  setValue(int percent);
};



// *****************************************************
//                      onlinePercentText
// *****************************************************


class onlinePercentText : public percentText,
                          public onlineIntStateTracker,
                          public  idler {
  public:
            onlinePercentText(int x, int y,int width, int height);
    virtual ~onlinePercentText(void);
    
            void  setTheLook();
    virtual void  readState(void);
    virtual void  doAction(void);
    virtual void  idle(void);                            
};



// *****************************************************
//                     nameText
// *****************************************************

class nameText : public onlineCStrStateTracker,
                 public fontLabel,
                 public  idler {

  public:
                nameText(int x, int y,int width, int height);
  virtual       ~nameText(void);

            void  setTheLook();
    virtual void  readState(void);
    virtual void  idle(void);
    virtual void  drawSelf(void);
};



// *****************************************************
//                     stateText
// *****************************************************

class stateText : public onlineIntStateTracker,
                 public fontLabel,
                 public idler {

  public:
                stateText(int x, int y,int width, int height);
  virtual       ~stateText(void);

            void  setTheLook();
    virtual void  readState(void);
    virtual void  idle(void);
    virtual void  drawSelf(void);
};



// *****************************************************
//                      currentMoistureText
// *****************************************************


class currentMoistureText : public onlinePercentText {
  
  public:
            currentMoistureText(int x, int y,int width, int height);
  virtual   ~currentMoistureText(void);

  virtual void  readState(void);  
};



// *****************************************************
//                      moistureLimitText
// *****************************************************


class moistureLimitText : public onlinePercentText {
  
  public:
            moistureLimitText(int x, int y,int width, int height);
  virtual   ~moistureLimitText(void);

  virtual void  readState(void);  
};



// *****************************************************
//                      currentTempText
// *****************************************************


class currentTempText : public onlineIntStateTracker,
                        public label,
                        public  idler {
  
  public:
            currentTempText(int x, int y,int width, int height);
  virtual   ~currentTempText(void);
  virtual void  setTheLook(void);
  virtual void  readState(void);
  virtual void  idle(void);
};


// *****************************************************
//                      totalWaterText
// *****************************************************


class totalWaterText : public onlineULongStateTracker,
                        public label,
                        public  idler {
  public:
            totalWaterText(int x, int y,int width, int height);
  virtual   ~totalWaterText(void);
  virtual void  setTheLook(void);
  virtual void  readState(void);
  virtual void  idle(void);

          float mlPerSec;
};



// *****************************************************
//                   totalLogTimeText
// *****************************************************


class totalLogTimeText :  public onlineULongStateTracker,
                          public timeText,
                          public  idler {
  public:
                totalLogTimeText(int x, int y,int width, int height);
  virtual       ~totalLogTimeText(void);
  virtual void  setTheLook(void);
  virtual void  readState(void);
  virtual void  idle(void);
};
  


// *****************************************************
//                   waterTimeText
// *****************************************************


class waterTimeText :  public onlineIntStateTracker,
                       public timeText,
                       public  idler {
  public:
                waterTimeText(int x, int y,int width, int height);
  virtual       ~waterTimeText(void);
  virtual void  setTheLook(void);
  virtual void  readState(void);
  virtual void  idle(void);
};



// *****************************************************
//                   soakTimeText
// *****************************************************


class soakTimeText :  public onlineIntStateTracker,
                       public timeText,
                       public  idler {
  public:
                soakTimeText(int x, int y,int width, int height);
  virtual       ~soakTimeText(void);
  virtual void  setTheLook(void);
  virtual void  readState(void);
  virtual void  idle(void);
};


/*










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

*/

// ******************************************
//                    plantBotCom
// ******************************************


#define PLANTBOT_NAME_SIZE  24    // The name has to fit in a string this size. IE ONE LESS.
#define MAX_WTIME           120      // Limit your water time to 2 minutes.
#define MAX_STIME           600      // Limit your soak time to 10 minutes.



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
          
          char*         getName(void);        // You get a pointer to where we stored it. You'll need to copy it out.
          int           getLimit(void);
          int           getWaterTime(void);
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

          void  runUpdates(bool stopStart);
          void  updateTime(void);           // MUST be called repeatedly by loop() NOT in idle().
          bool  getOnline(void);
          
  protected:
          void  setUpdateTime(void);
          void  sleep(int ms);
          
          int           mIndex;
          bool          mOnline;
          timeObj       mSleepTimer;
          timeObj       mUpdateTimer;
          bool          mDoingUpdates;
          
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
