#include "tools.h"
#include <resizeBuff.h>
#include "debug.h"

#define SLEEP_TIME          2   // When looking for an answer, rest this long before peeking.
#define REPLY_TIMEOUT       100 // How long they get to give back a reply.
#define LONG_UPDATE_TIME    500 // When offline, only check this often.
#define SHORT_UPDATE_TIME   200 // When online, we can check this often.


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
  setTime(100,false); // You get 100 ms to reply. Get with it!
  setUpdateTime();    // How oftern do we bug the bot for data updates?
  
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
