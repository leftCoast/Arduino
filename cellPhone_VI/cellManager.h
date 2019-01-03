#ifndef cellManager_h
#define cellManager_h

#include "lists.h"
#include "timeObj.h"
#include "quickCom.h"
#include "litlOS.h"    // Just for the log view

enum cellComStates { com_standby, com_working, com_holding, com_complete };


class cellCommand : public linkListObj,
                    public  timeObj {

  public:
          cellCommand(void);
  virtual ~cellCommand(void);

          void    showCommand(void);
          byte    getNumComBytes(void);
          void    getComBuff(char* buff);
          bool    setReplySize(byte numBytes);
          
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

          int           sendCommand(byte commandNum,char* comData,bool wantReply);
          int           sendCommand(byte inCommandNum,bool inWantReply);            // Or no string..
          int           progress(int commandID);
          cellCommand*  findByID(int inCommandID);
          byte          numReplyBytes(int CommandID);
          void          readReply(int CommandID,byte* buff);
          void          dumpReply(int commandID);

          
          
          void          cleanList(void);
          cellCommand*  getCurrent(void);
          void          runCommand(void);
          void          doStatus(void);
          void          showCellStatus(void);
  virtual void          idle();
          
          int           mNextID;
          cellCommand*  mCurrentCommand;
          int           mStatusID;
          timeObj       mStatusTimer;
};


#endif
