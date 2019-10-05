#ifndef tools_h
#define tools_h

#include <commonComs.h>
#include <drawObj.h>
#include <blinker.h>
#include <bmpPipe.h>
#include <fontLabel.h>
#include <editLabel.h>
#include <bmpKeyboard.h>
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

#define CUT32_BMP "/icons/Cut32.bmp"
#define COPY32_BMP "/icons/Copy32.bmp"
#define PASTE32_BMP "/icons/Paste32.bmp"

#define ENABLE_LOG_BMP  "/images/enableDL.bmp"
#define COPY_LOG_BMP    "/images/copyDL.bmp"
#define DELETE_LOG_BMP  "/images/deleteDL.bmp"
#define FILE_NAME_BMP   "/images/fName.bmp"

// *****************************************************
//                      ourKeyboard
// *****************************************************

class ourKeyboard : public bmpKeyboard {

  public:

                ourKeyboard(editable* inEditObj,bool modal);
  virtual       ~ourKeyboard(void);

  virtual void  keyClicked(keyboardKey* aKey);
};



// *****************************************************
//                      timeText
// *****************************************************

// This is a special that does the short time format to an inputted seconds.
// It displays time as one best fit unit. s,m,h,d,w as a number with one
// decimal place. Not perfectly accurate, but close enough for humans.
// For example put in 125 seconds and it'll put out "2.0 m".


void  timeFormatter(unsigned long sec); // Its out here for anyone to use. Drop a seconds value in here..
extern  char  timeStrBuf[];             // Read out the formated string result here. Easy Peasy!


class timeText :  public editLabel {

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
class percentText :  public editLabel {

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
    virtual void  idle(void);                            
};


// *****************************************************
//                     monoNameText
// *****************************************************

class monoNameText :  public onlineCStrStateTracker,
                      public editLabel,
                      public idler {

  public:
                monoNameText(int x, int y,int width, int height);
  virtual       ~monoNameText(void);

            void  setTheLook();
    virtual void  readState(void);
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

          keyboard*       mKeyboard;
          drawGroup*      mParent;
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



// *****************************************************
//                   fileCopyObj
// *****************************************************


class fileCopyObj : public idler {

  public:
                fileCopyObj(void);
  virtual       ~fileCopyObj(void);

  virtual bool  startTransfer(char* localPath);
  virtual bool  doingTransfer(void);
  virtual int   percentComplete(void);
  virtual void  idle(void);

          unsigned long mTotalBytes;
          unsigned long mIndex;
          char*         mLocalPath;
          byte          mDataBuff[255];
          bool          mRunning;
};



// *****************************************************
//                   pauseUpdates
// *****************************************************


class pauseUpdates {

  public:
                pauseUpdates(void);
  virtual       ~pauseUpdates(void);              
};



// *****************************************************
//                    plantBotCom
// *****************************************************


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
 
enum floraReplySet  { noErr, unknownCom, badParam };

// Notice that in the code here, we only look for noErr and just toss the rest out? LAZY! Actually, a lot of
// stuff we just check that we get the right amount of buyes back. Ask for a byte and get one. Ask for an int,
// we get exactly that? We call it good. 

class plantBotCom : public commonComs {

  public:
                plantBotCom(void);
  virtual       ~plantBotCom(void);
          
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
          bool  startLogCopyCom(char* filemane);
          bool  getLogBuff(void);
          bool  logCopyComActive(void);
          int   logCopyPercent(void);
          bool  clearLogCom(void);

          void  runUpdates(bool stopStart);
          void  updateTime(void);           // MUST be called repeatedly by loop() NOT in idle().
          bool  getOnline(void);
          void  setOnline(bool online);
          
  protected:
  
          void  setUpdateTime(void);
          
          int           mIndex;
          bool          mOnline;
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

          unsigned long mFileIndex;
          char*         mLocalPath;
          byte          mDataBuff[255];
          bool          mRunning;
};


extern  plantBotCom ourComPort;


#endif
