#ifndef comPlayer_h
#define comPlayer_h

#include "idlers.h"
#include "lists.h"
#include "timeObj.h"
#include "resizeBuff.h"

// **************************************************
// Need a list of commands called over a period of
// time? Need this done without using delay()? Here's
// your tool for doing just that.
//
// 


// **************************************************
// Base command class
// If inSleep>0 it will cause the commands to hold
// for that period of time. If inSleep<=0 it will
// call the doCommand() method. Simple simple.
// **************************************************


class comObj :   public linkListObj {

   public:
                     comObj(int inSleep=0);
      virtual        ~comObj(void);

      virtual  void  doCommand(void);

               int   mSleepMS;
};



// **************************************************
// comPlayer class
// This will "play" the list inputted commands. If
// repeat is set to true then they will play over and
// over. You can make as many of these as you want.
// The'll all run independantly.
// **************************************************


class comPlayer : public queue,
                  public idler,
                  public timeObj {

   public:
                  comPlayer(void);
      virtual     ~comPlayer(void);

               void  clearComs(void);
               void  addCom(comObj* aCom);
               void  repeat(bool trueFalse);
               
      virtual  void  idle(void);

               bool  mRepeat;
};


#endif