#include "tools.h"
#include <resizeBuff.h>
#include "floraOS.h"
#include <debug.h>

#define LONG_UPDATE_TIME    500 // When offline, only check this often.
#define SHORT_UPDATE_TIME   100 // When online, we can check this often.
#define NUM_FILEBUFFS       8   // When doing a file transfer, how many filer buffer to get before releasing to other things.


// *****************************************************
//                      ourKeyboard
// *****************************************************


ourKeyboard::ourKeyboard(editable* inEditObj,bool modal)
  :bmpKeyboard(inEditObj,modal) {  }
ourKeyboard::~ourKeyboard(void) {  }

void ourKeyboard::keyClicked(keyboardKey* aKey) { ourOS.beep(); }



// *****************************************************
//                      timeText
// *****************************************************


timeText::timeText(int x, int y,int width, int height)
  : editLabel(x,y,width,height) { }

timeText::~timeText(void) { }


void timeText::setValue(unsigned long seconds) {

  timeFormatter(seconds);
  label::setValue(timeStrBuf); 
}


char timeStrBuf[10]; // "9000 w" Is only 7 chars. It doesn't do years.

void timeFormatter(unsigned long sec) {
  
  int   weeks;
  int   days;
  int   hours;
  int   minutes;
  char* strPtr;
  float fract;
  int   dec;
  
  if (sec==0) {
    strcpy(timeStrBuf,"0.0 s");
    return;
  }
  if (sec>=SEC_IN_WEEK) {
    weeks = sec/SEC_IN_WEEK;
    sec = sec-weeks*SEC_IN_WEEK;
    fract = sec/(float)SEC_IN_WEEK;
    dec = fract * 10;
    snprintf(timeStrBuf,10,"%d.%d w",weeks,dec);
  } else if (sec>=SEC_IN_DAY) {
    days = sec/SEC_IN_DAY;
    sec = sec-days*SEC_IN_DAY;
    fract = sec/(float)SEC_IN_DAY;
    dec = fract * 10;
    snprintf(timeStrBuf,10,"%d.%d d",days,dec);
  }  else if (sec>=SEC_IN_HOUR) {
    hours = sec/SEC_IN_HOUR;
    sec = sec-hours*SEC_IN_HOUR;
    fract = sec/(float)SEC_IN_HOUR;
    dec = fract * 10;
    snprintf(timeStrBuf,10,"%d.%d h", hours,dec);
  }  else if (sec>=60) {
    minutes = sec/60;
    sec = sec-minutes*60;
    fract = sec/60.0;
    dec = fract * 10;
    snprintf(timeStrBuf,10,"%d.%d m",minutes,dec);
  } else {
    snprintf (timeStrBuf,5,"%d s",sec);
  }
}



// *****************************************************
//                      percentText
// *****************************************************


percentText::percentText(int x, int y,int width, int height)
  : editLabel(x,y,width,height) { }


percentText::~percentText(void) { }


void percentText::setValue(int percent) {

  char  buff[10];

  snprintf (buff,10,"%d %%",percent);
  label::setValue(buff); 
}



// *****************************************************
//                      onlinePercentText
// *****************************************************


onlinePercentText::onlinePercentText(int x, int y,int width, int height)
  : percentText(x,y,width,height) {  }

  
onlinePercentText::~onlinePercentText(void) {  }

    
void onlinePercentText::setTheLook() {

  if (mCurrentState.online) {
    percentText::setValue(mCurrentState.value);
  } else {
    label::setValue("--- %");
  }
}


void onlinePercentText::readState(void) {  }


void onlinePercentText::idle(void) {
  
  percentText::idle();
  if (checkState()) {
    setTheLook();
  }
}                           



// *****************************************************
//                      monoNameText
// *****************************************************


monoNameText::monoNameText(int x, int y,int width, int height)
  : onlineCStrStateTracker(),
  editLabel(x,y,width,height) {

  setColors(&white,&black);
  setJustify(TEXT_CENTER);
  setTextSize(2);
  editLabel::hookup();
}

monoNameText::~monoNameText(void) { } 


void monoNameText::setTheLook() {
  
  if (mCurrentState.online) {
    setValue(mCurrentState.value);
  } else {
    setValue("Offline");
  }
}


void monoNameText::readState(void) {

  int   numBytes;

  resizeBuff(0,&mCurrentState.value);                     // First we recycle the memory from the last string.
  mCurrentState.online = ourComPort.getOnline();          // Lets see if we are online at all? (And save it)
  if (mCurrentState.online) {                             // If we are online..
    numBytes = strlen(ourComPort.getName())+1;            // Read off how long the string is.
    if (resizeBuff(numBytes,&mCurrentState.value)) {      // If we are able to get the memory to store it..
      strcpy(mCurrentState.value,ourComPort.getName());   // We copy in the new name.
    }
  }
}


void monoNameText::idle(void) { 

  editLabel::idle();
  if (checkState()) setTheLook();
}



// *****************************************************
//                      nameText
// *****************************************************


nameText::nameText(int x, int y,int width, int height)
  : onlineCStrStateTracker(),
  fontLabel(x,y,width,height) {

  setColors(&white,&black);
  setJustify(TEXT_CENTER);
  setTextSize(1);
  hookup();
}

nameText::~nameText(void) { } 


void nameText::setTheLook() {
  
  if (mCurrentState.online) {
    setValue(mCurrentState.value);
  } else {
    setValue("Offline");
  }
}


void nameText::readState(void) {

  int   numBytes;

  resizeBuff(0,&mCurrentState.value);                     // First we recycle the memory from the last string.
  mCurrentState.online = ourComPort.getOnline();          // Lets see if we are online at all? (And save it)
  if (mCurrentState.online) {                             // If we are online..
    numBytes = strlen(ourComPort.getName())+1;            // Read off how long the string is.
    if (resizeBuff(numBytes,&mCurrentState.value)) {      // If we are able to get the memory to store it..
      strcpy(mCurrentState.value,ourComPort.getName());   // We copy in the new name.
    }
  }
}


void nameText::idle(void) { if (checkState()) setTheLook(); }


void nameText::drawSelf(void) {

  screen->fillRect(this,&black);  // Sadly the fancy fon't need to have their fields blanked out first.
  fontLabel::drawSelf();
}



// *****************************************************
//                      stateText
// *****************************************************


stateText::stateText(int x, int y,int width, int height)
  : onlineIntStateTracker(),
  fontLabel(x,y,width,height) {

  setColors(&white,&black);
  setJustify(TEXT_CENTER);
  setTextSize(1);
  hookup();
}


stateText::~stateText(void) { } 


void stateText::setTheLook() {
  
  if (mCurrentState.online) {
    switch (mCurrentState.value) {
      case 0 :  setValue("Reading"); break;
      case 1 :  setValue("Watering"); break;
      case 2 :  setValue("Soaking"); break;
    }
  } else {
    setValue("");
  }
}


void stateText::readState(void) {

  mCurrentState.online = ourComPort.getOnline();
  mCurrentState.value = ourComPort.getState();
}


void stateText::idle(void) { if (checkState()) setTheLook(); }


void stateText::drawSelf(void) {

  screen->fillRect(this,&black);  // Sadly the fancy fonts need to have their fields blanked out first.
  fontLabel::drawSelf();
}



// *****************************************************
//                      currentMoistureText
// *****************************************************


currentMoistureText::currentMoistureText(int x, int y,int width, int height)
  : onlinePercentText(x,y,width,height) {

  setColors(&white,&black);
  setJustify(TEXT_RIGHT);
  setTextSize(2);
  percentText::hookup();
}


currentMoistureText::~currentMoistureText(void) { } 

void currentMoistureText::readState(void) {

  mCurrentState.online = ourComPort.getOnline();
  mCurrentState.value = ourComPort.getMoisture();
}



// *****************************************************
//                      moistureLimitText
// *****************************************************


moistureLimitText::moistureLimitText(int x, int y,int width, int height)
  : onlinePercentText(x,y,width,height) {

  setColors(&white,&black);
  setJustify(TEXT_RIGHT);
  setTextSize(2);
  percentText::hookup();
}

moistureLimitText::~moistureLimitText(void) { } 

void moistureLimitText::readState(void) {

  mCurrentState.online = ourComPort.getOnline();
  mCurrentState.value = ourComPort.getLimit();
}



// *****************************************************
//                      currentTempText
// *****************************************************


currentTempText::currentTempText(int x, int y,int width, int height)
  : onlineIntStateTracker(),
  label(x,y,width,height) {

  setColors(&white,&black);
  setJustify(TEXT_RIGHT);
  setTextSize(2);
  hookup();
}

currentTempText::~currentTempText(void) { } 


void currentTempText::setTheLook() {

  char  buff[10];
  
  if (mCurrentState.online) {
    snprintf (buff,10,"%d c",mCurrentState.value);
    setValue(buff);
  } else {
    setValue("--- c");
  }
}


void currentTempText::readState(void) {

  mCurrentState.online = ourComPort.getOnline();
  mCurrentState.value = ourComPort.getTemp();
}


void currentTempText::idle(void) { if (checkState()) setTheLook(); }



// *****************************************************
//                      totalWaterText
// *****************************************************


totalWaterText::totalWaterText(int x, int y,int width, int height)
  : onlineULongStateTracker(),
  label(x,y,width,height) {

  setColors(&white,&black);
  setJustify(TEXT_RIGHT);
  setTextSize(2);
  hookup();
  mlPerSec = .01;
}

totalWaterText::~totalWaterText(void) { } 


void totalWaterText::setTheLook(void) {

  float res;
  char  wStr[10];
  int   prec;
  
  if (mCurrentState.online) {
    res = (mCurrentState.value/2.0)*mlPerSec;
    if (res>=100) {
      prec = 0;
    } else {
      prec = 1;
    }
    dtostrf(res,0,prec,wStr);
    strcat(wStr," l");
    setValue(wStr);
  } else {
    setValue("--- l");
  }
}


void totalWaterText::readState(void) {

  mCurrentState.online = ourComPort.getOnline();
  mCurrentState.value = ourComPort.getLogWLines();
}


void totalWaterText::idle(void) { if (checkState()) setTheLook(); }



// *****************************************************
//                   totalLogTimeText
// *****************************************************


totalLogTimeText::totalLogTimeText(int x, int y,int width, int height)
  : onlineULongStateTracker(),
  timeText(x,y,width,height) {

  setColors(&white,&black);
  setJustify(TEXT_RIGHT);
  setTextSize(2);
  timeText::hookup();
}

  
totalLogTimeText::~totalLogTimeText(void) {  }


void totalLogTimeText::setTheLook(void) {
    
  if (mCurrentState.online) {
    timeText::setValue(mCurrentState.value/2);
  } else {
    label::setValue("--- s");
  }
}

 
void totalLogTimeText::readState(void) {

  mCurrentState.online = ourComPort.getOnline();
  mCurrentState.value = ourComPort.getLogLines();
}


void totalLogTimeText::idle(void) { if (checkState()) setTheLook(); }



// *****************************************************
//                   waterTimeText
// *****************************************************


waterTimeText::waterTimeText(int x, int y,int width, int height)
  : onlineIntStateTracker(),
  timeText(x,y,width,height) {

  setColors(&white,&black);
  setJustify(TEXT_RIGHT);
  setTextSize(2);
  timeText::hookup();
}

  
waterTimeText::~waterTimeText(void) {  }


void waterTimeText::setTheLook(void) {
    
  if (mCurrentState.online) {
    timeText::setValue(mCurrentState.value);
  } else {
    label::setValue("--- s");
  }
}

 
void waterTimeText::readState(void) {

  mCurrentState.online = ourComPort.getOnline();
  mCurrentState.value = ourComPort.getWaterTime();
}


void waterTimeText::idle(void) {
  
  timeText::idle();
  if (checkState()) {
    setTheLook();
  }
}



// *****************************************************
//                   soakTimeText
// *****************************************************


soakTimeText::soakTimeText(int x, int y,int width, int height)
  : onlineIntStateTracker(),
  timeText(x,y,width,height) {

  setColors(&white,&black);
  setJustify(TEXT_RIGHT);
  setTextSize(2);
  timeText::hookup();
}

  
soakTimeText::~soakTimeText(void) {  }


void soakTimeText::setTheLook(void) {
    
  if (mCurrentState.online) {
    timeText::setValue(mCurrentState.value);
  } else {
    label::setValue("--- s");
  }
}

 
void soakTimeText::readState(void) {

  mCurrentState.online = ourComPort.getOnline();
  mCurrentState.value = ourComPort.getSoakTime();
}


void soakTimeText::idle(void) {

  timeText::idle();
  if (checkState()) {
    setTheLook();
  }
}



// *****************************************************
//                   pauseUpdates
// *****************************************************

// So simple. When you are going to do somehing involved and long that would cause updates
// to time out, just add "pauseUpdates x;" as your first line. Then, when your method or function
// goes out of scope, the second line is automatically called and updates are turned back on.
// Its also nestible. You can recurse in as much as you like, calling pauseUpdates. Only the
// outermost call will actually restart updates.

int gPause = 0;

pauseUpdates::pauseUpdates(void) {

  ourComPort.updateTime();        // Give it one last kick before shutting it down.
  ourComPort.runUpdates(false);   // Shut it down.
  gPause++;                       // Bump up our nesting index.
}


pauseUpdates::~pauseUpdates(void) {

  if (gPause>0) {                 // If our nesting index is greater than zero..
    gPause--;                     // Bump it down by one.
  }
  if (!gPause) {                  // If our nesting index is zero..
    ourComPort.runUpdates(true);  // We're at the top level, fire up updates again.
  }
}             



// *****************************************************
//                    plantBotCom
// *****************************************************


plantBotCom::plantBotCom(void) 
  : commonComs() {
  
  mIndex = 0;
  mOnline = false;
  setUpdateTime();              // How oftern do we bug the bot for data updates?
  runUpdates(true);
  mLocalPath = NULL;
  mRunning = false;
}


plantBotCom::~plantBotCom(void) {  }


void plantBotCom::setUpdateTime(void) {

  if (mOnline) {
    mUpdateTimer.setTime(SHORT_UPDATE_TIME,true);
  } else {
    mUpdateTimer.setTime(LONG_UPDATE_TIME,true);
  }
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
  if (name) {
    if (strlen(name)>=PLANTBOT_NAME_SIZE) {
      name[PLANTBOT_NAME_SIZE-1]='\0';
    }
    numBytes = strlen(name)+2;
    if (resizeBuff(numBytes,(char**)&buff)) {
      buff[0] = setName;
      strcpy((char*)&buff[1],name);
      sendBuff(buff,numBytes,true);     // Send the one byte command and ask for ack.
      while(!haveBuff()&&!readErr()) {  // We either get something back or time out.
        sleep(QCOM_SLEEP_TIME);              // Have a nap, let the UI amuse the user.
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
    sleep(QCOM_SLEEP_TIME);
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
    sleep(QCOM_SLEEP_TIME);
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


bool plantBotCom::getLogBuff(void) {

  byte            com[          sizeof(byte) + sizeof(byte) + sizeof(unsigned long  )]; 
  int             numComBytes = sizeof(byte) + sizeof(byte) + sizeof(unsigned long  ); // Count up bytes we're sending.
  unsigned long*  ULptr;
  File            localFile;
  byte            numRepBytes;
 
  numRepBytes = 254;                                    // As far as I can tell 254 is the max you can do..
  com[0] = readLogBuff;                                 // Stuff in our command.
  com[1] = numRepBytes;                                 // Stuff in the buffer size.
  ULptr = (unsigned long*)&(com[2]);                    // Point to the next byte as an unsigned long
  *ULptr = mFileIndex;                                  // Using the pointer stuff in the unsigned long index.
  readErr();                                            // Clear errors.
  sendBuff(com,numComBytes,true);                       // Send the command and ask for ack.
  while(!haveBuff() && !readErr()) {                    // We either get something back or time out.
    sleep(QCOM_SLEEP_TIME);                             // Have a nap, let the UI amuse the user.
  }
  numRepBytes = haveBuff();                             // We kicked out. See if there's any bytes to grab.
  if (numRepBytes) {                                    // If there were..
    readBuff(mDataBuff);                                // Save the data.
    Serial.print((char*)mDataBuff);
    localFile = SD.open(mLocalPath,FILE_WRITE);         // Pop open our file, if possible.
    if (localFile) {                                    // Ok, we got the local file. So far everything is good.
        localFile.write(mDataBuff,numRepBytes);         // Dump in the bytes.
        localFile.close();                              // Close our file.
        mFileIndex = mFileIndex + numRepBytes;          // Bring the remote file index up to date.
      }
      mRunning = numRepBytes==254;                      // We got a full buffer? Then we'll have another go at it.
  }
  return true;                                          // There is no check for not being online here so we assume we are online.
}
  

bool plantBotCom::startLogCopyCom(char* filemane) {

  File  localFile;
  bool  success;

  success = false;                                          // Lets assume this isn't going to work.
  if (getOnline()) {                                        // We're online right?
    if (!mRunning) {                                        // We're not already doing a transfer now are we?
      if (filemane) {                                       // Sanity third! They actually sent us something, right?
        if (resizeBuff(strlen(filemane)+1,&mLocalPath)) {   // Can we get the RAM to store the pathname?
          strcpy(mLocalPath,filemane);                      // OK ok, we can. So lets save the path for later.
          localFile = SD.open(mLocalPath,FILE_WRITE);       // Try to open/create a logfile.
          if (localFile) {                                  // Ok, we got the local file. So far everything is good.                        
            localFile.close();                              // Close the file.
            SD.remove(mLocalPath);                          // And delete it. Makes sure we clear it out.
            mFileIndex = 0;                                 // Zero out our index.
            mRunning = true;                                // Flag that we're running.
            success = true;                                 // Look! It worked!
          } else {                                          // Not able to open the file?
            resizeBuff(0,&mLocalPath);                      // Dump the path buffer. Its a bust.
          }
        }
      }
    }
  }
  return success;
}


bool plantBotCom::logCopyComActive(void) { return mRunning;}


int plantBotCom::logCopyPercent(void) { return round(((float)mFileIndex/(float)getLogSize())*100.0); }


bool plantBotCom::clearLogCom(void) {

  if (!mRunning) {
    return sendCommand(clearLog);
  } else {
    return false;
  }
}


bool plantBotCom::runFileTransfer(void) {

  int numBuffs;

  if (!mRunning) return mOnline;            // Not running? Do nothing change nothing.
  numBuffs = NUM_FILEBUFFS;                 // Set up how many we can grab before we get booted.
  while(mRunning && numBuffs) {             // Make hay while the sun shines. Grab file buffers.
    getLogBuff();                           // The acutal grabe itself.
  }
  return getByte(readMoisture,&mMoisture);  // Easy to get, most dynamic variable. Gives an online check.
}


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
        case 13 : mOnline = runFileTransfer();                              break;     
        default : mIndex = 0; 
      }
      setUpdateTime();
    }
  }
}


void plantBotCom::runUpdates(bool stopStart) { mDoingUpdates = stopStart; }


bool plantBotCom::getOnline(void) { return mOnline; }
void plantBotCom::setOnline(bool online) { mOnline = online; }






plantBotCom ourComPort;
