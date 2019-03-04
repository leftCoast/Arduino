#ifndef cellManager_h
#define cellManager_h

#include <lists.h>
#include <timeObj.h>
#include <quickCom.h>
#include <cellCommon.h>

enum cellComStates { com_standby, com_working, com_holding, com_complete, com_missing };


class cellCommand : public linkListObj,
                    public  timeObj {

  public:
          cellCommand(void);
  virtual ~cellCommand(void);

          void    showCommand(void);
          byte    getNumComBytes(void);
          void    getComBuff(uint8_t* buff);
          bool    setReplySize(byte numBytes);

          int           commandID;
          uint8_t       commandNum;
          bool          wantReply;
          int           numComBytes;
          uint8_t*      comData;
          int           numReplyBytes;
          uint8_t*      replyData;
          
          cellComStates comState;
};


class cellManager : public qCMaster,
                    public linkList {
  public:
          cellManager(void);
  virtual ~cellManager(void);

          
          int           sendCommand(byte commandNum,bool wantReply);                                  // Send a one byte command. Returns an ID#.
          int           sendCommand(byte commandNum,char* dataStr,bool wantReply);                    // Send  a one byte command with data string. Returns an ID#.
          int           sendCommand(byte commandNum,byte numBytes,uint8_t* comData,bool wantReply);   // Send  a one byte command with data block. Returns an ID#.
          int           progress(int commandID);                                                      // Look up this ID and return the command's status.
          byte          numReplyBytes(int CommandID);                                                 // How many bytes you need to allocate to fit your reply.
          void          readReply(int CommandID,byte* buff);                                          // Read out your reply into the buffer you allocated.
          void          dumpReply(int commandID);                                                     // Decide you don't want the reply.

          
          cellCommand*  findByID(int inCommandID);
          cellCommand*  getCurrent(void);
          void          cleanList(void);
          void          runCommand(void);
          void          launchCommand(cellCommand* aCom);
          void          checkForRreply(cellCommand* aCom);
          void          doStatus(void);
          void				clearStatus(void);
          void          showCellStatus(void);
  virtual void          idle();
          
          int           mNextID;
          cellCommand*  mCurrentCommand;
          int           mStatusID;
          timeObj       mStatusTimer;
};

extern cellManager ourCellManager;
extern cellStatus  statusReg;

#endif
