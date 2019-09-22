#include "tools.h"
#include <resizeBuff.h>
#include "debug.h"
#include "floraOS.h"

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


/*
#define TEXT_SIZE     2
#define CHAR_WIDTH    6
#define BUTTON_HEIGHT 18
#define RADIUS        3



baseButton::baseButton(char* inLabel,int x, int y,int width, int height)
  : switchable(x, y, width, height, touchLift) {

  // default button colors.
  BGColor.setColor(LC_BLACK);
  activeBColor.setColor(LC_BLUE);
  activeBColor.blend(&white, 40);
  activeTColor.setColor(LC_WHITE);
  clickedBColor.setColor(LC_WHITE);
  clickedTColor.setColor(LC_BLACK);

  label = NULL;
  setText(inLabel);
}


baseButton::~baseButton(void) {

  if (label) {
    free(label);
    label = NULL;
  }
}


void baseButton::setText(char* inText) {
  
  int numChars;

  if (inText) {
    numChars = strlen(inText) + 1;
    if (resizeBuff(numChars,(uint8_t**)&label)) {
      strcpy(label, inText);
    }
  }
}


// The original didn't do what I expected. So I just hacked a local copy for the buttons here.
//nullEvent, touchEvent, liftEvent, dragBegin, dragOn, clickEvent
bool baseButton::acceptEvent(event* inEvent,point* locaPt) {

  switch (mEventSet) {
    case noEvents   : return false;             // noEvents, pass on..
    case touchLift    :                         // Classic button events, clicked lets you draw clicked.
      if (inEvent->mType==touchEvent) {         // If its a touch..
        if (inRect(locaPt)) {                   // - and if its on us..
          clicked   = true;                     // Might want to show we're clicked on.
          ourOS.beep(clicked);
          doAction(inEvent,locaPt);             // Do our stuff.
          theTouched  = this;                   // Tell the world WE are accepting this event set.
          needRefresh = true;                   // touchLift doesn't get a lift event. So it needs the setRefresh here.
          return true;                          // Tell the world the event has been accepted.
        }
      } else if (theTouched==this && inEvent->mType==liftEvent) {   // We only want lifts if we'd accepted the touch.
        clicked   = false;                      // And we're no longer clicked.
        ourOS.beep(clicked);
        doAction(inEvent,locaPt);               // Do our other stuff.
        needRefresh = true;                     // And here.. (see above)
        return true;                            // Again, tell the world the event has been accepted.
      }
      break;
    case fullClick  :               // Things like edit fields. A click changes their state.
      if (inEvent->mType==clickEvent) {   // If its a click event, that matches.
        if (inRect(locaPt)) {         // and if its on us..
          clicked   = false;          // No longer clicked by the time you see this.
          doAction();               // Do the action.
          return true;              // We don't set touched because this is a one shot event.
        }
      }
      break;
    case dragEvents :               // Things that move by touch.
      if (inEvent->mType==dragBegin) {      // If, the dragging finger has started..
        if (inRect(locaPt)) {         // and if its on us..
          doAction(inEvent,locaPt);     // Do our stuff.
          theTouched  = this;         // Tell the world WE are accepting this event set.
          return true;
        }
      } else if (inEvent->mType==dragOn) {    // still moving,
        doAction(inEvent,locaPt);         // Stil dragging? Keep drawing.
        return true;                  // Event has been accepted.
      } else if (inEvent->mType==liftEvent) { // Done dragging.
        doAction(inEvent,locaPt);         // Do our stuff.
        return true;                  // Again, tell the world the event has been accepted.
      }
      break;
    }
    return false;
  }


// OK, lets NOT call hookup() by default. If the derived needs to
// do idle things, let them call it.
void baseButton::idle(void) { }


void baseButton::drawSelf(void) {

  word dispWidth;
  word textWidth;

  if (mOnOff) {
    if (clicked) {
      screen->fillRoundRect(x-2, y-2, width+4, height+4, RADIUS+2, &activeBColor);
      screen->fillRoundRect(x, y, width, height, RADIUS, &clickedBColor);
      screen->setTextColor(&clickedTColor, &clickedBColor);
    } else {
      screen->fillRoundRect(x-2, y-2, width+4, height+4, RADIUS+2, &white); 
      screen->fillRoundRect(x, y, width, height, RADIUS, &activeBColor);
      screen->setTextColor(&activeTColor, &activeBColor);
    }
    screen->setTextSize(TEXT_SIZE);
    screen->setTextWrap(false);
    dispWidth = width - (2 * RADIUS);
    textWidth = (CHAR_WIDTH * TEXT_SIZE * strlen(label)) - 1;
    if (dispWidth > textWidth) {
      screen->setCursor(x + RADIUS + ((dispWidth - textWidth) / 2), y + 2);
      screen->drawText(label);
    }
  } else {
    rect arect;
    arect.setRect(this);
    arect.insetRect(-2);
    screen->fillRect(&arect,&BGColor);
  }
}



// ***************************************************************
//                      baseIconButton
// ***************************************************************


baseIconButton::baseIconButton(int x,int y,int width,int height,char* filePath)
  : baseButton(NULL,x,y,width,height),
  bmpPipe() {

  rect  sourceRect;

  if (filePath) {
    if (openPipe(filePath)) {
      sourceRect.setRect(0,0,32,32);
      setSourceRect(sourceRect);
    }
  }
}

 
baseIconButton::~baseIconButton(void) {  }

 // We.. Don't do this.
void baseIconButton::setText(char* intext) {  }


void baseIconButton::drawSelf(void) {

  if (mOnOff) {
    if (haveInfo) {
      drawBitmap(x,y);
    }
  } else {
    rect arect;
    arect.setRect(this);
    screen->fillRect(&arect,&black);  // Black is hardcoded 'cause the icons are drawn for a  black background.
  }
}
*/


// *****************************************************
//                  onlineCStrStateTracker
// *****************************************************


// Every periodMs we are going to..
onlineCStrStateTracker::onlineCStrStateTracker(float periodMs,bool looseTime)
  :stateTracker(periodMs,looseTime) {

    mSavedState.online  = true;  // Forces a refresh when going online.
    mSavedState.value   = NULL;

    mCurrentState.online  = true;  // A total lie, but a useful one..
    mCurrentState.value   = NULL;
}


onlineCStrStateTracker::~onlineCStrStateTracker(void) {

  resizeBuff(0,&(mSavedState.value));
  resizeBuff(0,&(mCurrentState.value));
}


// Reading current state and..        
void onlineCStrStateTracker::readState(void) { /* you fill out this one..*/ }

// Compare with the.. Saved state.         
bool onlineCStrStateTracker::compareStates(void) { return mCurrentState.online==mSavedState.online && !strcmp(mCurrentState.value,mSavedState.value); }

// And then, saving the new state.
void onlineCStrStateTracker::saveState(void) { 

  int numBytes;
  
  mSavedState.online = mCurrentState.online;
  numBytes = strlen(mCurrentState.value)+1;
  resizeBuff(numBytes,&(mSavedState.value));
  strcpy(mSavedState.value,mCurrentState.value);
}



// *****************************************************
//                  onlineULongStateTracker
// *****************************************************


// Every periodMs we are going to..
onlineULongStateTracker::onlineULongStateTracker(float periodMs,bool looseTime)
  :stateTracker(periodMs,looseTime) {

    mSavedState.online  = true;  // Forces a refresh when going online.
    mSavedState.value   = 0;

    mCurrentState.online  = true;  // A total lie, but a useful one..
    mCurrentState.value   = 0;
}


onlineULongStateTracker::~onlineULongStateTracker(void) {  }


// Reading current state and..        
void onlineULongStateTracker::readState(void) { /* you fill out this one..*/ }

// Compare with the.. Saved state.         
bool onlineULongStateTracker::compareStates(void) { return mCurrentState.online==mSavedState.online && mCurrentState.value==mSavedState.value; }

// And then, saving the new state.
void onlineULongStateTracker::saveState(void) { mSavedState = mCurrentState; }



// *****************************************************
//                  onlineIntStateTracker
// *****************************************************


// Every periodMs we are going to..
onlineIntStateTracker::onlineIntStateTracker(float periodMs,bool looseTime)
  :stateTracker(periodMs,looseTime) {

    mSavedState.online  = true;  // Forces a refresh when going online.
    mSavedState.value   = 0;

    mCurrentState.online  = true;  // A total lie, but a useful one..
    mCurrentState.value   = 0;
}


onlineIntStateTracker::~onlineIntStateTracker(void) {  }


// Reading current state and..        
void onlineIntStateTracker::readState(void) { /* you fil out this one..*/ }

// Compare with the.. Saved state.         
bool onlineIntStateTracker::compareStates(void) { return mCurrentState.online==mSavedState.online && mCurrentState.value==mSavedState.value; }

// And then, saving the new state.
void onlineIntStateTracker::saveState(void) { mSavedState = mCurrentState; }


          
// *****************************************************
//                      stateTracker
// *****************************************************


// Every periodMs we are going to..
stateTracker::stateTracker(float periodMs,bool looseTime) {

  mLooseTime = looseTime;
  mTimer.setTime(periodMs,true);
  mFirstState = true;
}


stateTracker::~stateTracker(void) {  }


bool stateTracker::checkState(void) {

  if (mTimer.ding()) {            // If the timer has expired..
    if (mLooseTime) {             // If we are allowing loose time..
        mTimer.start();           // Restart the timer from now.
    } else {                      // Else NOT allowing loose time..
      mTimer.stepTime();          // We reset the timer from the last start time. (Picky picky!)
    }
    readState();
    if (!compareStates()) {       // If we compare the states and thy are NOT the same..
       saveState();               // We save off the new state into the saved state.
       return true;               // Then return TRUE that the state has changed. 
    }
  }
  return false;                   // In all other cases, we just return false.
}


// You inherit this.
// A) Read the current state.
// B) Save the current state to a (current state) class member variable that you create.
void stateTracker::readState(void) { }


// You inherit this.
// A) Compare this current state with the saved last state.
// B) return the result. (Just like in an if statement, TRUE means they are the SAME.)
// 
// mFirstState : This is a boolean set to true in the constructor. You can set it to false
// here to show that you've successfully started to read states. It is nothing mnore than
// a boolean for you to use as you wish. I thought it might come in handy to know that this
// is the first time through or not.
bool stateTracker::compareStates(void) {  }


// You inherit this..
// You must set up a (saved state) class member variable. In this method you overwrite your
// saved state member variable with your current state variable.
void stateTracker::saveState(void) { }


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
