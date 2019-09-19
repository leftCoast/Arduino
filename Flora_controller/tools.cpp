#include "tools.h"
#include <resizeBuff.h>
#include "debug.h"

#define SLEEP_TIME          2   // When looking for an answer, rest this long before peeking.
#define REPLY_TIMEOUT       100 // How long they get to give back a reply.
#define LONG_UPDATE_TIME    500 // When offline, only check this often.
#define SHORT_UPDATE_TIME   200 // When online, we can check this often.


char  tTstring[100];

void timeFormatter(unsigned long sec) {
  
  int   weeks;
  int   days;
  int   hours;
  int   minutes;
  char* strPtr;
  float fract;
  int   dec;
  
  if (sec==0) {
    strcpy(tTstring,"0.0 s");
    return;
  }
  if (sec>=SEC_IN_WEEK) {
    weeks = sec/SEC_IN_WEEK;
    sec = sec-weeks*SEC_IN_WEEK;
    fract = sec/(float)SEC_IN_WEEK;
    dec = fract * 10;
    snprintf(tTstring,10,"%d.%d w",weeks,dec);
  } else if (sec>=SEC_IN_DAY) {
    days = sec/SEC_IN_DAY;
    sec = sec-days*SEC_IN_DAY;
    fract = sec/(float)SEC_IN_DAY;
    dec = fract * 10;
    snprintf(tTstring,10,"%d.%d d",days,dec);
  }  else if (sec>=SEC_IN_HOUR) {
    hours = sec/SEC_IN_HOUR;
    sec = sec-hours*SEC_IN_HOUR;
    fract = sec/(float)SEC_IN_HOUR;
    dec = fract * 10;
    snprintf(tTstring,10,"%d.%d h", hours,dec);
  }  else if (sec>=60) {
    minutes = sec/60;
    sec = sec-minutes*60;
    fract = sec/60.0;
    dec = fract * 10;
    snprintf(tTstring,10,"%d.%d m",minutes,dec);
  } else {
    snprintf (tTstring,5,"%d s",sec);
  }
}



// *****************************************************
//                      percentText
// *****************************************************


percentText::percentText(int x, int y,int width, int height)
  : label(x,y,width,height) { }

percentText::~percentText(void) { }


void percentText::setValue(int percent) {

  char  buff[10];

  snprintf (buff,10,"%d %%",percent);
  label::setValue(buff); 
}



// *****************************************************
//                      timeText
// *****************************************************


timeText::timeText(int x, int y,int width, int height)
  : label(x,y,width,height) { }

timeText::~timeText(void) { }


void timeText::setValue(int seconds) {

  debugger.trace("setValue() ",seconds,false);
  timeFormatter(seconds);
  label::setValue(tTstring); 
}



// *****************************************************
//                     stateText
// *****************************************************


stateText::stateText(int x, int y,int width, int height)
  : onlineFText(x,y,width,height) {

  mStateVal = 0;
  setTextSize(1);
  setJustify(TEXT_CENTER);
}

  
stateText::~stateText(void) {  }


void stateText::setValue(void) {

  if (mOnline) {
    mStateVal = ourComPort.getState();
    switch (mStateVal) {
      case 0 :  onlineFText::setValue("Reading"); break;
      case 1 :  onlineFText::setValue("Watering"); break;
      case 2 :  onlineFText::setValue("Soaking"); break;
    }
  } else {
    onlineFText::setValue("");
  }
}


void stateText::setState(void) { setValue(); }


void stateText::idle() {

  onlineFText::idle();
  if (mOnline) {
    if (mStateVal!=ourComPort.getState()) {
      setValue();
    }
  }
}



// *****************************************************
//                     nameText
// *****************************************************


nameText::nameText(int x, int y,int width, int height)
  : onlineFText(x,y,width,height) {

  setTextSize(1);
  setJustify(TEXT_CENTER);
}

  
nameText::~nameText(void) {  }


void nameText::setValue(void) {

  if (mOnline) {
    onlineFText::setValue(ourComPort.getName());
  } else {
    onlineFText::setValue("Offline");
  }
}


void nameText::setState(void) { setValue(); }


void nameText::idle() {

  onlineFText::idle();
  if (mOnline) {
    if (strcmp(buff,ourComPort.getName())) {
      setValue();
    }
  }
}



// *****************************************************
//                     sTimeText
// *****************************************************


sTimeText::sTimeText(int x, int y,int width, int height)
  : onlineText(x,y,width,height) { mSoakTime = 0; }

  
sTimeText::~sTimeText(void) {  }


void sTimeText::setValue(void) {

  if (mOnline) {
    timeFormatter(mSoakTime);
    onlineText::setValue(tTstring);
  } else {
    onlineText::setValue("--- s");
  }
}


void sTimeText::setState(void) {

  if (mOnline) { mSoakTime = ourComPort.getSoakTime(); }
  setValue();
}


void sTimeText::idle() {

  onlineText::idle();
  if (mOnline) {
    if (mSoakTime!=ourComPort.getSoakTime()) {
      mSoakTime = ourComPort.getSoakTime();
      setValue();
    }
  }
}


// *****************************************************
//                     wTimeText
// *****************************************************

wTimeText::wTimeText(int x, int y,int width, int height)
  : onlineText(x,y,width,height) { mWaterTime = 0; }

  
wTimeText::~wTimeText(void) {  }


void wTimeText::setValue(void) {

  if (mOnline) {
    timeFormatter(mWaterTime);
    onlineText::setValue(tTstring);
  } else {
    onlineText::setValue("--- s");
  }
}


void wTimeText::setState(void) {

  if (mOnline) { mWaterTime = ourComPort.getWaterTime(); }
  setValue();
}


void wTimeText::idle() {

  onlineText::idle();
  if (mOnline) {
    if (mWaterTime!=ourComPort.getWaterTime()) {
      mWaterTime = ourComPort.getWaterTime();
      setValue();
    }
  }
}



// *****************************************************
//                     limitText
// *****************************************************


limitText::limitText(int x, int y,int width, int height)
  : onlineText(x,y,width,height) { mLimit = 0; }

  
limitText::~limitText(void) {  }


void limitText::setValue(void) {

  char  limit[7];
  
    if (mOnline) {
      snprintf (limit,5,"%d",mLimit);
      strcat(limit," %");
    } else {
      strcpy(limit,"--- %");
    }
    onlineText::setValue(limit);
}


void limitText::setState(void) {

  if (mOnline) { mLimit = ourComPort.getLimit(); }
  setValue();
}


void limitText::idle() {

  onlineText::idle();
  if (mOnline) {
    if (mLimit!=ourComPort.getLimit()) {
      mLimit = ourComPort.getLimit();
      setValue();
    }
  }
}



// *****************************************************
//                   totalWaterText
// *****************************************************


totalWaterText::totalWaterText(int x, int y,int width, int height)
  : onlineText(x,y,width,height) {

  lPerSec = .01;
  setPrecision(1);
}

  
totalWaterText::~totalWaterText(void) {  }


void totalWaterText::setValue(void) {

  float res;
  char  wStr[10];
  int   prec;
  
  if (mOnline) {
    res = (mLogWetLines/2.0)*lPerSec;
    if (res>=100) {
      prec = 0;
    } else {
      prec = 1;
    }
    dtostrf(res,0,prec,wStr);
    strcat(wStr," l");
    onlineText::setValue(wStr);
  } else {
    onlineText::setValue("--- l");
  }
}


void totalWaterText::setState(void) {

  if (mOnline) { mLogWetLines = ourComPort.getLogWLines(); }
  setValue();
}


void totalWaterText::idle() {

  onlineText::idle();
  if (mOnline) {
    if (mLogWetLines!=ourComPort.getLogWLines()) {
      mLogWetLines = ourComPort.getLogWLines();
      setValue();
    }
  }
}



// *****************************************************
//                   totalTimeText
// *****************************************************


totalTimeText::totalTimeText(int x, int y,int width, int height)
  : onlineText(x,y,width,height) { }

  
totalTimeText::~totalTimeText(void) {  }


void totalTimeText::setValue(void) {

  if (mOnline) {
    timeFormatter(mLogLines/2);
    onlineText::setValue(tTstring);
  } else {
    onlineText::setValue("--- s");
  }
}


void totalTimeText::setState(void) {

  if (mOnline) { mLogLines = ourComPort.getLogLines(); }
  setValue();
}


void totalTimeText::idle() {

  onlineText::idle();
  if (mOnline) {
    if (mLogLines!=ourComPort.getLogLines()) {
      mLogLines = ourComPort.getLogLines();
      setValue();
    }
  }
}


// *****************************************************
//                     moistureText
// *****************************************************


moistureText::moistureText(int x, int y,int width, int height)
  : onlineText(x,y,width,height) { }

  
moistureText::~moistureText(void) {  }


void moistureText::setValue(void) {

  char  moistText[7];
  
    if (mOnline) {
      snprintf (moistText,5,"%d",mMoisture);
      strcat(moistText," %");
    } else {
      strcpy(moistText,"--- %");
    }
    onlineText::setValue(moistText);
}


void moistureText::setState(void) {

  if (mOnline) { mMoisture = ourComPort.getMoisture(); }
  setValue();
}


void moistureText::idle() {

  onlineText::idle();
  if (mOnline) {
    if (mMoisture!=ourComPort.getMoisture()) {
      mMoisture = ourComPort.getMoisture();
      setValue();
    }
  }
}



// *****************************************************
//                     tempText
// *****************************************************


tempText::tempText(int x, int y,int width, int height)
  : onlineText(x,y,width,height) { }

  
tempText::~tempText(void) {  }


void tempText::setValue(void) {

  char  aStr[7];
  
    if (mOnline) {
      snprintf (aStr,5,"%d",mTemp);
      strcat(aStr," C");
    } else {
      strcpy(aStr,"--- C");
    }
    onlineText::setValue(aStr);
}


void tempText::setState(void) {

  if (mOnline) { mTemp = ourComPort.getTemp(); }
  setValue();
}


void tempText::idle() {

  onlineText::idle();
  if (mOnline) {
    if (mTemp!=ourComPort.getTemp()) {
      mTemp = ourComPort.getTemp();
      setValue();
    }
  }
}



// *****************************************************
//                      onlineFText
// *****************************************************



onlineFText::onlineFText(int x, int y,int width, int height)
  : fontLabel(x,y,width,height) {
  
  mOnline = !ourComPort.getOnline();
  setColors(&white,&black);
  setJustify(TEXT_RIGHT);
  hookup();
  setTextSize(2);
}

  
onlineFText::~onlineFText(void) {  }


void onlineFText::idle() {
  
  if (mOnline != ourComPort.getOnline()) {
    mOnline = ourComPort.getOnline();
    setState();
  }
}

void onlineFText::drawSelf(void) {
  
  screen->fillRect(this,&black);
  fontLabel::drawSelf();
}

// *****************************************************
//                      onlineText
// *****************************************************



onlineText::onlineText(int x, int y,int width, int height)
  : label(x,y,width,height) {
  
  mOnline = !ourComPort.getOnline();
  setColors(&white,&black);
  setJustify(TEXT_RIGHT);
  hookup();
  setTextSize(2);
}

  
onlineText::~onlineText(void) {  }


void onlineText::idle() {
  
  if (mOnline!= ourComPort.getOnline()) {
    mOnline = ourComPort.getOnline();
    setState();
  }
}



// ******************************************
//                    plantBotCom
// ******************************************


plantBotCom::plantBotCom(void) 
  : qCMaster() {
  
  mIndex = 0;
  mOnline = false;
  setTime(REPLY_TIMEOUT,false); // You get this long to reply. Get with it!
  setUpdateTime();              // How oftern do we bug the bot for data updates?
  runUpdates(true);
}


plantBotCom::~plantBotCom(void) {  }


void plantBotCom::setUpdateTime(void) {

  if (mOnline) {
    mUpdateTimer.setTime(SHORT_UPDATE_TIME,true);
  } else {
    mUpdateTimer.setTime(LONG_UPDATE_TIME,true);
  }
}


void plantBotCom::sleep(int ms) {

  

  mSleepTimer.setTime(ms);        // Set up our timer. We have a timer just for this?
  if (!idling) {                  // If we are not IN the idle loop.. (Don't get all reentrant here!)
    while(!mSleepTimer.ding()) {  // While waiting for the timer to expire..
      theIdlers.idle();           // We repeatedly call idle() to keep the ALL the background stuff running.
    }
  } else {                        // We ARE in the idler loop. In that case we can't just call idle() because that will lock everything up.                
    while(!mSleepTimer.ding()) {  // So, while waiting for the timer to expire..
      idle();                     // We repeatedly call our own idle() to keep the our background stuff running.
    }
  }
}


bool plantBotCom::getByte(byte com,byte* reply) {

  byte  buff;
  
  readErr();
  sendBuff(&com,1,true);
  while(!haveBuff()&&!readErr()) {
    sleep(SLEEP_TIME);
  }
  buff = haveBuff();
  if (buff==1) {
    readBuff(reply);
    return true;
  } else {
    dumpBuff();
  }
  return false;
}


bool plantBotCom::getInt(byte com,int* reply) {

  byte  buff;
  
  readErr();
  sendBuff(&com,1,true);
  while(!haveBuff()&&!readErr()) {
    sleep(SLEEP_TIME);
  }
  buff = haveBuff();
  if (buff==sizeof(int)) {
    readBuff((byte*)reply);
    return true;
  } else {
    dumpBuff();
  }
  return false;
}


bool plantBotCom::getFloat(byte com,float* reply) {

  byte  buff;
  
  readErr();
  sendBuff(&com,1,true);
  while(!haveBuff()&&!readErr()) {
    sleep(SLEEP_TIME);
  }
  buff = haveBuff();
  if (buff==sizeof(float)) {
    readBuff((byte*)reply);
    return true;
  } else {
    dumpBuff();
  }
  return false;
}


bool plantBotCom::getLong(byte com,long* reply) {

  byte  buff;
  
  readErr();
  sendBuff(&com,1,true);
  while(!haveBuff()&&!readErr()) {
    sleep(SLEEP_TIME);
  }
  buff = haveBuff();
  if (buff==sizeof(long)) {
    readBuff((byte*)reply);
    return true;
  } else {
    dumpBuff();
  }
  return false;
}


bool plantBotCom::getUnsignedLong(byte com,unsigned long* reply) {

  byte  buff;
  
  readErr();
  sendBuff(&com,1,true);
  while(!haveBuff()&&!readErr()) {
    sleep(SLEEP_TIME);
  }
  buff = haveBuff();
  if (buff==sizeof(unsigned long)) {
    readBuff((byte*)reply);
    
    return true;
  } else {
    dumpBuff();
  }
  return false;
}


bool plantBotCom::getCString(byte com,char* reply) {

  byte  buff;
  
  readErr();
  sendBuff(&com,1,true);
  while(!haveBuff()&&!readErr()) {
    sleep(SLEEP_TIME);
  }
  buff = haveBuff();
  if (buff>0) {                     // C'mon! Even an empty string has a '\0'.
    readBuff((byte*)reply);
    return true;
  }
  return false;
}



// Send a comand that we just get an err/no-err back for.
bool plantBotCom::sendCommand(byte com) {

  byte  buff;
  
  readErr();                        // Clear errors.
  sendBuff(&com,1,true);            // Send the one byte command and ask for ack.
  while(!haveBuff()&&!readErr()) {  // We either get something back or time out.
    sleep(SLEEP_TIME);              // Have a nap, let the UI amuse the user.
  }
  buff = haveBuff();                // We kicked out of the loop. See if something came back.
  if (buff) {                       // We got somethig..
    if (buff==1) {                  // Anything other than one byte is an error.
      readBuff(&buff);              // Read the byte..
      if (buff==0) {                // If its a zero, this is good. It means no error.
        return true;                // Return success, we are done.
      }
    } else {                        // It was not one byte
      dumpBuff();                   // Whatever, its not what we want. Dump it.
    }
  }
  return false;
}


char* plantBotCom::getName(void)              { return mName; }

int plantBotCom::getLimit(void)               { return mLimit; }

int plantBotCom::getWaterTime(void)           { return round(mWaterTime/1000.0); }

int plantBotCom::getSoakTime(void)            { return round(mSoakTime/1000.0); }

int plantBotCom::getPulse(void)               { return mPulse; }

int plantBotCom::getPump(void)                { return mPump; }

int plantBotCom::getLogging(void)             { return mLogging; }

unsigned long plantBotCom::getLogSize(void)   { return mLogSize; }

unsigned long plantBotCom::getLogLines(void)   { return mLogNumLines; }

unsigned long plantBotCom::getLogWLines(void)   { return mLogNumWLines; }

int plantBotCom::getState(void)               { return mState; }

int plantBotCom::getTemp(void)                { return mTemp; }

int plantBotCom::getMoisture(void)            { return mMoisture; }


bool plantBotCom::setNameReg(char* name) {

  byte* buff;
  byte  numBytes;
  byte  res;
  
  buff = NULL;
  if (name&&strlen(name)<PLANTBOT_NAME_SIZE) {
    numBytes = strlen(name)+2;
    if (resizeBuff(numBytes,(char**)&buff)) {
      buff[0] = setName;
      strcpy(&buff[1],name);
      sendBuff(buff,numBytes,true);     // Send the one byte command and ask for ack.
      while(!haveBuff()&&!readErr()) {  // We either get something back or time out.
        sleep(SLEEP_TIME);              // Have a nap, let the UI amuse the user.
      }
      res = haveBuff();                // We kicked out of the loop. See if something came back.
      if (res) {                       // We got somethig..
        if (res==1) {                  // Anything other than one byte is an error.
          readBuff(&res);              // Read the byte..
          if (res==0) {                // If its a zero, this is good. It means no error.
            return true;                // Return success, we are done.
          }
        } else {                        // It was not one byte
          dumpBuff();                   // Whatever, its not what we want. Dump it.
        }
      }
    }
  }
  return false;
}


bool plantBotCom::setLimitReg(int limit) {

  byte  com[sizeof(int)+1];
  int*  intPtr;
  byte  res;
  
  com[0] = setDryLimit;
  intPtr = (int*)&com[1];
  *intPtr = limit;
  sendBuff(com,sizeof(int)+1,true);
  while(!haveBuff()&&!readErr()) {
    sleep(SLEEP_TIME);
  }
  res = haveBuff();                // We kicked out of the loop. See if something came back.
  if (res) {                       // We got somethig..
    if (res==1) {                  // Anything other than one byte is an error.
      readBuff(&res);              // Read the byte..
      if (res==0) {                // If its a zero, this is good. It means no error.
        return true;                // Return success, we are done.
      }
    } else {                        // It was not one byte
      dumpBuff();                   // Whatever, its not what we want. Dump it.
    }
  }
  return false;
}


bool plantBotCom::setWTimeReg(int seconds) { return setTimeReg(setWaterTime,seconds); }

bool plantBotCom::setSTimeReg(int seconds) { return setTimeReg(setSoakTime,seconds); }


bool plantBotCom::setTimeReg(floraComSet timeType,int seconds) {

  byte  com[sizeof(long)+1];
  long* longPtr;
  byte  res;
  
  com[0] = timeType;
  longPtr = (long*)&com[1];
  *longPtr = seconds * 1000;
  sendBuff(com,sizeof(long)+1,true);
  while(!haveBuff()&&!readErr()) {
    sleep(SLEEP_TIME);
  }
  res = haveBuff();                // We kicked out of the loop. See if something came back.
  if (res) {                       // We got somethig..
    if (res==1) {                  // Anything other than one byte is an error.
      readBuff(&res);              // Read the byte..
      if (res==0) {                // If its a zero, this is good. It means no error.
        return true;                // Return success, we are done.
      }
    } else {                        // It was not one byte
      dumpBuff();                   // Whatever, its not what we want. Dump it.
    }
  }
  return false;
}


bool plantBotCom::setWaterReg(bool onOff) {

  if (onOff) {
    return sendCommand(pumpOn);
  } else {
    return sendCommand(pumpOff);
  }
}


bool plantBotCom::setPulseReg(bool onOff) {
  
  if (onOff) {
    return sendCommand(pulseOn);
  } else {
    return sendCommand(pulseOff);
  }
}


bool plantBotCom::setLoggingReg(bool onOff) {

  if (onOff) {
    return sendCommand(loggingOn);
  } else {
    return sendCommand(loggingOff);
  }
}


bool plantBotCom::copyLogCom(char* filemane) {
  // readLogBuff but not yet..
}


bool plantBotCom::clearLogCom(void) {  return sendCommand(clearLog); }


void plantBotCom::updateTime(void) {

  if (mDoingUpdates) {
    if (mUpdateTimer.ding()) {
      switch(mIndex++) {
        case 0  : mOnline = getCString(readName,mName);                     break;
        case 1  : mOnline = getByte(readDryLimit,&mLimit);                  break;
        case 2  : mOnline = getLong(readWaterTime,&mWaterTime);             break;
        case 3  : mOnline = getLong(readSoakTime,&mSoakTime);               break;
        case 4  : mOnline = getByte(readPulse,&mPulse);                     break;
        case 5  : mOnline = getByte(readPump,&mPump);                       break;
        case 6  : mOnline = getByte(readLogging,&mLogging);                 break;
        case 7  : mOnline = getUnsignedLong(readLogSize,&mLogSize);         break;
        case 8  : mOnline = getUnsignedLong(readLogLines,&mLogNumLines);    break;
        case 9  : mOnline = getUnsignedLong(readLogWLines,&mLogNumWLines);  break;
        case 10 : mOnline = getByte(readState,&mState);                     break;
        case 11 : mOnline = getByte(readTemp,&mTemp);                       break;
        case 12 : mOnline = getByte(readMoisture,&mMoisture);               break;
        default : mIndex = 0; 
      }
      setUpdateTime();
    }
  }
}


void plantBotCom::runUpdates(bool stopStart) { mDoingUpdates = stopStart; }


bool plantBotCom::getOnline(void) { return mOnline; }



// *****************************************************
//                        flasher
// *****************************************************


flasher::flasher(rect* inRect,colorObj* backColor)
  : drawObj(inRect),
  blinker() {

  mBackColor.setColor(backColor);   // What is "off"?
  mForeColor.setColor(&red);        // Reasonable default.
}


flasher::flasher(int inX,int inY,int inWidth,int inHeight,colorObj* backColor)
  : drawObj(inX,inY,inWidth,inHeight),
  blinker() {
  
  mBackColor.setColor(backColor);   // What is "off"?
  mForeColor.setColor(&red);        // Reasonable default.
}

                     
flasher::~flasher(void) { }


// Basically a hacked version from blinker to remove the pinmode stuff.
// This is your on/off switch. Call with a boolean tru=on false=off.
// The object is created in the "off" mode.
void flasher::setBlink(bool onOff) {
  
 if (!init) {             // Not intialized?
    hookup();             // Set up idling.
    init = true;          // Note it.
  }
  if((onOff!=running)) {  // ignore if no change
    if (onOff) {          // Start blinking..    
      start();            // Starting NOW!
      setLight(true);     // light on!
      onTimer->start();   // set the time on timer.
      running = true;     // Set state.
      } 
    else {                // Stop blinking..
      setLight(false);    // light off.
      running = false;    // set state.
    }
  }
}   

    
void flasher::setLight(bool onOff) {

  lightOn = onOff;
  setNeedRefresh();
}


void flasher::drawSelf(void) {

  if (lightOn) {
    screen->fillRect(x,y,width,height,&mForeColor);
  } else {
    screen->fillRect(x,y,width,height,&mBackColor);
  }
}



// *****************************************************
//                     bmpFlasher
// *****************************************************


bmpFlasher::bmpFlasher(int inX,int inY, int width,int height,char* onBmp, char* offBmp)
  : flasher(inX,inY,width,height,&black) { 
 
  mReady = false;
  setup(onBmp,offBmp);
}

    
bmpFlasher::bmpFlasher(rect* inRect,char* onBmp, char* offBmp)
  : flasher(inRect,&black) { 
  
  mReady = false;
  setup(onBmp,offBmp);
}  

    
bmpFlasher::~bmpFlasher(void) {

  if (mOnBmp) delete(mOnBmp);
  if (mOffBmp) delete(mOffBmp);
}


void bmpFlasher::setup(char* onBmp,char* offBmp) {

  rect  sourceRect(0,0,width,height);
  mOnBmp = new bmpPipe(sourceRect);
  mOffBmp = new bmpPipe(sourceRect);
  if (mOnBmp&&mOffBmp) {
    if (mOnBmp->openPipe(onBmp) && mOffBmp->openPipe(offBmp)) {
      mReady = true;
    }
  }
}


void bmpFlasher::drawSelf(void) {

  if (mReady) {
    if (lightOn) {
      mOnBmp->drawBitmap(x,y);
    } else {
      mOffBmp->drawBitmap(x,y);
    }
  }
}



plantBotCom ourComPort;
