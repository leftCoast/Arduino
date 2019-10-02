#ifndef stateTracker_h
#define stateTracker_h

#include "Arduino.h"
#include "timeObj.h"

// statetracker : What is this? A common thing(?) one runs into writing code is something that refects the state
// of something else. Like a fuel guage for example. It reflects the state of your fuel level. Or possibly, some
// one elses fuel level. And, at the same time, you don't want to waste all your processor time watching the state
// of something. It would be nice if it could just take care of this by itsef and at a leasurey pace. This is what
// a stateTracker is the bases of.
//
// Now, typically, something like this is getting its state information from another remote system. So its all
// about checking if the remote system is online, and then getting the new values. In fact, this is so common I've
// included some nice general purpose classes as starting points/examples for whomever, including myself, uses this.
//
// onlineIntStateTracker - For tracking an int value.
// onlineULongStateTracker - For tracking an unsigned long value.
// onlineCStrStateTracker - For tracking a char* (C string) value.
// 
// So lets say you want to track 5 online int values. You could use the onlineIntStateTracker as your base class,
// then derive classes that would display each these in the best suitable way. RPM, Speed, fuel level, oil pressure.
// Starting to get the picture?


// *****************************************************
//                      stateTracker
// *****************************************************


class stateTracker {

  public:
          stateTracker(float periodMs=100,bool looseTime=true);  // Every periodMs you can check the state of something..
          ~stateTracker(void);
        
  virtual bool    checkState(void);         // Call this to tell you if the state of something has changed. By..         
  virtual void    readState(void);          // Reading current state and..      (User inherits and fills out)  
  virtual bool    compareStates(void);      // Compare with the.. Saved state.  (User inherits and fills out) 
  virtual void    saveState(void);          // And then, saving the new state.  (User inherits and fills out)  

          bool    mFirstState;              // Lets you know this is the first call. (Can be ignored)
          bool    mLooseTime;               // Set to true to ignore time drift over time. False means to factor out time drift as well as possible.
          timeObj mTimer;                   // Tells you when its ok to do the check. (Can be ignored)
};



// *****************************************************
//                  onlineCStrStateTracker
// *****************************************************


class onlineCStrStateTracker : public stateTracker {

  struct stateVar {
    bool  online;
    char* value;
  };
  
  public:
          onlineCStrStateTracker(float periodMs,bool looseTime=true);     // Every periodMs we are going to..
          onlineCStrStateTracker(void);
          ~onlineCStrStateTracker(void);
        
  virtual void    readState(void);
  virtual bool    compareStates(void);
  virtual void    saveState(void);

          stateVar  mCurrentState;
          stateVar  mSavedState;
};



// *****************************************************
//                  onlineULongStateTracker
// *****************************************************


class onlineULongStateTracker : public stateTracker {

  struct stateVar {
    bool          online;
    unsigned long value;
  };
  
  public:
          onlineULongStateTracker(float periodMs,bool looseTime=true);     // Every periodMs we are going to..
          onlineULongStateTracker(void);
          ~onlineULongStateTracker(void);
        
  virtual void    readState(void);
  virtual bool    compareStates(void);
  virtual void    saveState(void);

          stateVar  mCurrentState;
          stateVar  mSavedState;
};



// *****************************************************
//                  onlineIntStateTracker
// *****************************************************


class onlineIntStateTracker : public stateTracker {

  struct stateVar {
    bool  online;
    int   value;
  };
  
  public:
          onlineIntStateTracker(float periodMs,bool looseTime=true); 
          onlineIntStateTracker(void);
          ~onlineIntStateTracker(void);
        
  virtual void    readState(void);
  virtual bool    compareStates(void);
  virtual void    saveState(void);

          stateVar  mCurrentState;
          stateVar  mSavedState;
};


#endif
