#include "commonComs.h"



// *****************************************************
//                    commonComs
// *****************************************************


commonComs::commonComs(void) 
  : qCMaster() {
  
  setTime(QCOM_REPLY_TIMEOUT,false); // You get this long to reply. Get with it!
}


commonComs::~commonComs(void) {  }


void commonComs::sleep(int ms) {

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


bool commonComs::getByte(byte com,byte* reply) {

  byte  buff;
  
  readErr();
  sendBuff(&com,1,true);
  while(!haveBuff()&&!readErr()) {
    sleep(QCOM_SLEEP_TIME);
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


bool commonComs::getInt(byte com,int* reply) {

  byte  buff;
  
  readErr();
  sendBuff(&com,1,true);
  while(!haveBuff()&&!readErr()) {
    sleep(QCOM_SLEEP_TIME);
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


bool commonComs::getFloat(byte com,float* reply) {

  byte  buff;
  
  readErr();
  sendBuff(&com,1,true);
  while(!haveBuff()&&!readErr()) {
    sleep(QCOM_SLEEP_TIME);
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


bool commonComs::getLong(byte com,long* reply) {

  byte  buff;
  
  readErr();
  sendBuff(&com,1,true);
  while(!haveBuff()&&!readErr()) {
    sleep(QCOM_SLEEP_TIME);
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


bool commonComs::getUnsignedLong(byte com,unsigned long* reply) {

  byte  buff;
  
  readErr();
  sendBuff(&com,1,true);
  while(!haveBuff()&&!readErr()) {
    sleep(QCOM_SLEEP_TIME);
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


bool commonComs::getCString(byte com,char* reply) {

  byte  buff;
  
  readErr();
  sendBuff(&com,1,true);
  while(!haveBuff()&&!readErr()) {
    sleep(QCOM_SLEEP_TIME);
  }
  buff = haveBuff();
  if (buff>0) {                     // C'mon! Even an empty string has a '\0'.
    readBuff((byte*)reply);
    return true;
  }
  return false;
}


// Send a comand that we just get an err/no-err back for.
bool commonComs::sendCommand(byte com) {

  byte  buff;
  
  readErr();                        // Clear errors.
  sendBuff(&com,1,true);            // Send the one byte command and ask for ack.
  while(!haveBuff()&&!readErr()) {  // We either get something back or time out.
    sleep(QCOM_SLEEP_TIME);              // Have a nap, let the UI amuse the user.
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


