#include "SMSMsgList.h"



// ************** SMSMsg stuff **************


SMSMsg::SMSMsg(char* PNum,char* Msg) : linkListObj() {

  if (strlen(PNum)<PNUM_LEN) {
    strcpy(mPNum,PNum);
  } else {
    mPNum[0]='\0';
  }
  mMsg = malloc(strlen(Msg)+1);
  if (mMsg) {
    strcpy(mMsg,Msg);
  }
}


SMSMsg::~SMSMsg(void) { if (mMsg) free(mMsg); }


int SMSMsg::getMsgLen(void) { return strlen(mMsg); }


void SMSMsg::getMsg(char* buff) { strcpy(buff,mMsg); }


int SMSMsg::getPNumLen(void) { return strlen(mPNum); }


void SMSMsg::getPNum(char* buff) { strcpy(buff,mPNum); }



// ************** SMSMsgList stuff **************



SMSMsgList::SMSMsgList(void) : linkList() { }


SMSMsgList::~SMSMsgList(void) {  }


void SMSMsgList::addMsg(char* PNum,char* Msg) {

  SMSMsg* newMsg = new SMSMsg(PNum,Msg);
  if (newMsg) { 
    addToEnd(newMsg);
  }
}


SMSMsg* SMSMsgList::findFirst(char* PNum,bool unLink) {

  SMSMsg* trace;
  char    buff[PNUM_LEN];
  
  trace = (SMSMsg*) getList();
  while(true) {
    if(trace) {
      trace->getPNum(buff);
      if (!strcmp(PNum,buff)) { // Found it.
        if (unLink) {
          unlinkObj((linkListObj*)trace);
        }
        return trace;
      }
      trace = (SMSMsg*)trace->getNext();
    } else {
      return NULL;
    }
  }
}



void SMSMsgList::addToEnd(SMSMsg* newObj) { linkList::addToEnd((linkListObj*) newObj); }
