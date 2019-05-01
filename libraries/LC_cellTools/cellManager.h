#ifndef cellManager_h
#define cellManager_h

#include <lists.h>
#include <timeObj.h>
#include <quickCom.h>
#include <cellCommon.h>

//#define STATUS_TIME 1000


// This is the guy that actually talks to the FONA controller. And that, in turn, talks
// to the SIM800 celphone hardware. The overall design is to have a queue for cell
// commands. This is because, who knows how long each command will take? So we queue
// them up and deal with them during idle time. Some need responses, some do not.
//
// All commands get a a unique ID number, they hold their own state (cellComStates), and
// a set lifespan. If a command sits in the queue for too long, its timed out and
// deleted. The section of the code that originated the command may ask for a command by
// ID. A possible valid response is that the command is not found. Why? Probably because
// the FONA controller crashed. Who really knows? But at least it knows something went
// wrong and can take some sort of appropriate action.
//
// Along with all this, is a status block of data that is agreed on by both sides to pass
// back what's going on over in cell-hardware-land. This status thing is used as a
// background service getting info like; How charged is the battery? How much cell
// signal do we have? Is anyone trying to call us right now? This way the different
// sections of the phone application can display battery usage, cell "bars", know to
// start the call answer screen etc.
//
// The structure of the status block is in cellCommon.h. Well, at least our version of
// it is. Because of Arduino stupidity, when you put in a .h file, the IDE also adds
// EVERYTHING from that file's folder.. Into your code. This completely blew away all
// available memory on the poor FONA controller, trying to host code that was deigned
// for a larger platform. So REMEBER, there are two copies of cellCommon.h. If you
// want to make a change, you have to change BOTH. Where is the other one? You'll have
// to go look for it. (Who knows where it'll end up by the time your reading this.)


enum cellComStates { com_standby, com_working, com_holding, com_complete, com_missing };

// You typically don't interact with this "cellCommand" class. Its used by "cellManager"
// and that's the one you should interact with.

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

// Globals below
//
// There is a global "ourCellManager" that is a global version of this class "cellManager"
// that you should interact with.
//
// To see what's going on in cell-phone land, have a look at the global "statusReg". It'll
// have all the pertinate data filled in.

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
          void          checkForReply(cellCommand* aCom);
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
