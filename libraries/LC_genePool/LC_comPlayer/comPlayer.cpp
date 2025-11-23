#include "comPlayer.h"


// **************************************************
// Base command class
// If inSleep>0 it will cause the commands to hold
// for that period of time. If inSleep<=0 it will
// call the doCOmmand() method. Simple simple.
// **************************************************


comObj::comObj(int inSleep) { mSleepMS = inSleep; }


comObj::~comObj(void) {  }


void comObj::doCommand(void) { }



// **************************************************
// comPlayer class
// This will "play" the list inputted commands. If
// repeat is set to true then they will play over and
// over. You can make as many of these as you want.
// The'll all run independantly.
// **************************************************


// Constructor. Standard setup of all the base classes that make this machine up.
comPlayer::comPlayer(void)
   : queue(),
   idler(),
   timeObj() {
   
   mRepeat = false;
}


// Destructor. We don't allocate anything ourselves that isn't already dealt with. So,
// nothing to do here.  
comPlayer::~comPlayer(void) {  }


// If waiting, stop. Dump all commands, reset mRepeat to false.
void comPlayer::clearComs(void) {

   setTime(0);
   mRepeat = false;
   dumpList();
}


// Where you add new commands to the list.
void comPlayer::addCom(comObj* aCom) { 

   hookup();
   push((linkListObj*)aCom);
}


// Do we want this set of commands to repeat over and over? Or, just go through once and
// delete themselves? Tell the player that here.
void comPlayer::repeat(bool trueFalse) { mRepeat = trueFalse; }


// Everything is done in idle time. This is the "engine" of the command player.
// Automatically called in the idle loop.
void comPlayer::idle(void) {

   comObj* nextCom;
   
   if (ding() && !isEmpty()) {
      nextCom = (comObj*)pop();
      if (nextCom->mSleepMS>0) {
         setTime(nextCom->mSleepMS);
      } else {
         nextCom->doCommand();
      }
      if (mRepeat) {
         push(nextCom);
      } else {
      	delete(nextCom);
      }
   }
}

