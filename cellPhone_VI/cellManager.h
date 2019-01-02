#ifndef cellManager_h
#define cellManager_h

#include "lists.h"
#include "timeObj.h"
#include "quickCom.h"


enum cellComStates { com_standby, com_sending, com_listening, com_recieveing, com_holding };


class cellCommand : public linkListObj,
                    public  timeObj {

  public:
          cellCommand(void);
  virtual ~cellCommand(void);
  
          byte          commandNum;
          int           numComBytes;
          byte*         comData;
          bool          wantReply;
          int           numReplyBytes;
          byte*         replyData;
          int           commandID;
          cellComStates comState;  
};


class cellManager : public qCMaster,
                    public linkList {
  public:
          cellManager(void);
  virtual ~cellManager(void);

          int     sendCommand(byte commandNum,char* comData,bool wantReply);
          int     progress(int commandID);
          byte    numReplyBytes(int commandID);
          void    readReply(int commandID,byte* buff);                   // Copy the data we got back into here.
          void    dumpReply(int commandID);

          cellCommand*  findByID(int inCommandID);
          void    cleanList(void);
          
          int   mNextID;
};


#endif
