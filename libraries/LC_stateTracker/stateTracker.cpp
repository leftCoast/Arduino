#include "stateTracker.h"
#include "resizeBuff.h"

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

onlineIntStateTracker::onlineIntStateTracker(void)
  :stateTracker() {

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


onlineULongStateTracker::onlineULongStateTracker(void)
  :stateTracker() {

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


onlineCStrStateTracker::onlineCStrStateTracker(void)
  :stateTracker() {

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










          
