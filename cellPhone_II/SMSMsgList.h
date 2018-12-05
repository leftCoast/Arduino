#ifndef SMSMsgList_h
#define SMSMsgList_h

#include "lists.h"

#define PNUM_LEN  12  // How many chars to save a phone number?


class SMSMsg : public linkListObj {

  public :
            SMSMsg(char* inFrom,char* inMsg);
  virtual   ~SMSMsg(void);

            int   getMsgLen(void);      // Just a call to strLen() so add 1 for '\0'
            void  getMsg(char* buff);
            int   getPNumLen(void);     // Same note as above. Add 1 for '\0'
            void  getPNum(char* buff);

  protected:            
            char  mPNum[PNUM_LEN];
            char* mMsg;
};



class SMSMsgList : public linkList {

  public:
          SMSMsgList(void);
  virtual ~SMSMsgList(void);

  virtual void    addMsg(char* PNum,char* Msg);
  virtual SMSMsg* findFirst(char* PNum,bool unLink=true);

  protected:

  virtual void  addToEnd(SMSMsg* newObj);
};

#endif
