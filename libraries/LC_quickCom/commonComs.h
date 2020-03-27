#ifndef commonComs_h
#define commonComs_h

#include "quickCom.h"

#define QCOM_SLEEP_TIME		2   // When looking for an answer, rest this long before peeking.
#define QCOM_REPLY_TIMEOUT	75 // How long they get to give back a reply.

// *****************************************************
//                    commonComs
// *****************************************************


class commonComs :  public qCMaster {

  public:
                commonComs(void);
  virtual       ~commonComs(void);

  virtual bool  getByte(byte com,byte* reply);
  virtual bool  getInt(byte com,int* reply);
  virtual bool  getFloat(byte com,float* reply);
  virtual bool  getLong(byte com,long* reply);
  virtual bool  getUnsignedLong(byte com,unsigned long* reply);
  virtual bool  getCString(byte com,char* reply);
  virtual bool  sendCommand(byte com);

  //protected:
          void  sleep(int ms);
          
          timeObj       mSleepTimer;
};


#endif