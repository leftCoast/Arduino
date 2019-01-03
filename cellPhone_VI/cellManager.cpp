#include "cellManager.h"
#include <cellCommon.h>

#define COM_TIMEOUT 16000
#define STATUS_TIME 2000
#define ID_MAX      2000

#define out Serial.print
#define outln Serial.println()

cellManager ourCellManager;
cellStatus  statusReg;



// *****************************************************
// ******************  cellCommand *********************
// *****************************************************


cellCommand::cellCommand(void) 
  : linkListObj(),
  timeObj(COM_TIMEOUT) {

  commandID       = 0;
  commandNum      = 0;
  numComBytes     = 0;
  comData         = NULL;
  wantReply       = false;
  numReplyBytes   = 0;
  replyData       = NULL;
  comState        = com_standby;
};





cellCommand::~cellCommand(void) {

  if(comData) free(comData);
  if(replyData) free(replyData);
}


void cellCommand::showCommand(void) {

  out("------- Command -------");outln;
  out("commandID     = ");out(commandID);outln;
  out("commandNum    = ");out(commandNum);outln;
  out("numComBytes   = ");out(numComBytes);outln;
  out("comData       : ");out((char*)comData);outln;
  out("wantReply     = ");out(wantReply);outln;
  out("numReplyBytes = ");out(numReplyBytes);outln;
  out("replyData     : ");out((char*)replyData);outln;
  out("comState      = ");out(comState);outln;
  out("-------*------*-------");outln;
  
}


byte  cellCommand::getNumComBytes() { return numComBytes+2; }


void cellCommand::getComBuff(char* buff) {

  buff[0] = commandNum;
  strcpy(&buff[1],(char*)comData);
}


bool cellCommand::setReplySize(byte numBytes) {

  replyData = (byte*)malloc(numBytes);
  if (replyData) {
    numReplyBytes = numBytes;
    return true;
  }
  return false;
}



// *****************************************************
// ******************  cellManager *********************
// *****************************************************


cellManager::cellManager(void)
  : qCMaster(),
  linkList() {

  mNextID         = 0;      // Start up the counter.
  mCurrentCommand = NULL;   // No current command.
  mStatusID       = -1;     // Flag for no status cooking.
  mStatusTimer.setTime(STATUS_TIME);
}


// Nothing really to do. Inherited will dump
// and delete all the rmaining commands.
cellManager::~cellManager(void) {  }


int cellManager::sendCommand(byte inCommandNum,char* inComData,bool inWantReply) {

  cellCommand*  newCommand;
  byte          numBytes;

  newCommand = new cellCommand;
  if (newCommand) {
    newCommand->commandID = ++mNextID;
    newCommand->commandNum = inCommandNum;
    newCommand->wantReply = inWantReply;
    numBytes = 0;
    if (inComData) {
      numBytes = strlen(inComData);
    }
    if (numBytes) {
      numBytes++;
      newCommand->comData = (byte*)malloc(numBytes);
      if (newCommand->comData) {
        newCommand->numComBytes = numBytes;
        strcpy((char*)newCommand->comData,inComData);
      } else {                                            // Oh no.. Not enough RAM. This looks bad..
        delete newCommand;
        return -1;
      }
    }
    addToEnd(newCommand);
    return newCommand->commandID;                         // Success at this point.
  }                                                       
  return -1;                                              // Agaain, we're out of RAM. Maybe we should have joined the other side?
}


int cellManager::sendCommand(byte inCommandNum,bool inWantReply) { return sendCommand(inCommandNum,NULL,inWantReply); }


// Find a command by ID and if found return the status of that command.
// If no command is found it returns -1 as a flag.
int cellManager::progress(int commandID) {

  cellCommand*  trace;

  trace = findByID(commandID); 
  if (trace) {
    return trace->comState;
  }
  return -1;                      // Meaning not found. Possibly recycled.
}


// If a command is holding a reply buffer, this will return
// the total # of bytes in that buffer.
byte  cellManager::numReplyBytes(int commandID) {

  cellCommand* aCommand;

  aCommand = findByID(commandID);
  if (aCommand) {
    if (aCommand->comState==com_holding) {
      return aCommand->numReplyBytes;
    }
  }
  return 0;
}


// Find this commad and read its reply. A successful read will
// delete the command.
void cellManager::readReply(int commandID,byte* buff) {

  cellCommand*  aCommand;
  byte          i;
  
  aCommand = findByID(commandID);
  if (aCommand) {
    if (aCommand->comState==com_holding) {
      for(i=0;i<aCommand->numReplyBytes;i++) {
        buff[i] = aCommand->replyData[i];
      }
      aCommand->comState=com_complete;          // We have served our purpose!
    }
  }
}


// The user has lost interest in this command. If its
// not "in proccess" delete it.
void cellManager::dumpReply(int commandID) {

  cellCommand*  trace;
  
  trace = findByID(commandID); 
  if (trace) {
    if (trace->comState==com_holding || trace->comState==com_standby) {   // Not in-process?
      trace->comState=com_complete;                                       // We have served our purpose!
    }
  }
}


// Returns a pointer to a command with the given ID.
// If no command is found it returns a NULL as a flag.
cellCommand* cellManager::findByID(int inCommandID) {

  cellCommand*  trace;

  trace = (cellCommand*)getFirst();
  while(trace) {
    if (trace->commandID==inCommandID) return trace;
    trace = (cellCommand*)trace->getNext();
  }
  return NULL;      // Meaning not found.
}


void cellManager::cleanList(void) {

  cellCommand*  trace;
  cellCommand*  temp;
  
  trace = (cellCommand*)getFirst();                     // Grab one off the top.
  while(trace) {                                        // While we're not sitting on a NULL.
    if (trace->ding()||trace->comState==com_complete) { // A expired command?
      temp = trace;                                     // Save  pointer to it.
      trace = (cellCommand*)trace->getNext();           // Run trace past.
      unlinkObj(temp);                                  // Unlink the expired command.
      delete temp;                              //  Recycle.
    } else {
      trace = (cellCommand*)trace->getNext();   // Default run past when not finding expired commands.
    }
  }
}


cellCommand* cellManager::getCurrent(void) {

  cellCommand*  trace;
  
  if (mCurrentCommand) return mCurrentCommand;
  trace = (cellCommand*)getFirst();             // Grab one off the top.
  while(trace) {                                // While we're not sitting on a NULL.
    if (trace->comState!=com_holding) {         // If its not holding an output buff.
      mCurrentCommand = trace;                  // We'll do this guy!
      return mCurrentCommand;                   // Here 'tis!
    }
  }
  return mCurrentCommand;                       // Was a NULL, still is a NUll.
}


void cellManager::runCommand(void) {

  cellCommand*  aCom;
  char*         ourComBuff;
  byte          numBytes;
  bool          reply;

  aCom = getCurrent();                              // Grab either what we're working on or a new command.
  if (aCom) {                                       // Got one? Ok!
    switch(aCom->comState) {                        // What have we got here?
      case com_standby    :                         // Waiting to send.
        if (mState==holding) {                      // Oh ohh.. Someone left something behind in here.
          dumpBuff();                               // Dump it out.
        }
        if (mState==standby) {                      // We're waiting too!
          numBytes = aCom->getNumComBytes();        // Howw big is this going to be?
          ourComBuff = (char*)malloc(numBytes);     // Grab some RAM.
          if (ourComBuff) {                         // Got it?
            aCom->getComBuff(ourComBuff);           // Get the command.
            reply = aCom->wantReply;                // Save off if they want a reply.
            readErr();                              // Clear out old errors.
            sendBuff((byte*)ourComBuff,numBytes,reply);    // Send it out, get response.
            free(ourComBuff);                       // Done with the command buff.
            if (!readErr()) {                       // No troubles?
              if (reply) {                          // If they want a reply..
                aCom->comState = com_working;       // Set it as working and lets go!                  
              } else {                              // If they don't want a reply, its job is done.
                aCom->comState = com_complete;      // Mark for disposal;
                mCurrentCommand = NULL;             // No current command.
              }
            }                                       // If a write error? Do nothing and retry later.
          }                                         // Not enough RAM? Again do nothing and try again. 
        }                                           // Same. Try later.
      break;                                   
      case com_working    :                         // This guy is waiting for a reply.
        if (mState==holding) {                      // You've got mail!
          numBytes = haveBuff();                    // See how big it is..
          if (aCom->setReplySize(numBytes)) {       // Tell the command to grab RAM.
              readBuff((byte*)(aCom->replyData));     // If it got the RAM stuff the reply data in there.
              aCom->comState = com_holding;         // Flag that its holding a reply.
          } else {                                  // Running low on RAM?
            dumpBuff();                             // You know what? That means there is big troubles coming down.
            aCom->comState = com_complete;          // Mark for disposal;
          }
          mCurrentCommand = NULL;                   // In ny case, We have no more current command.
        }
      break;
    }
  }
}


void cellManager::doStatus(void) {

  cellCommand*  statusCom;
  byte          numBytes;

  if (mStatusID==-1) {                          // We need to fire one off?
     mStatusID = sendCommand(getStatus,true);   // Well, that was easy.
  } else {                                      // We have a current one to work with?
    numBytes = numReplyBytes(mStatusID);        // reply size in bytes.
    if (numBytes) {
      if (numBytes==sizeof(cellStatus)) {       // We got the right byte count?
        readReply(mStatusID,(byte*)&statusReg); // Stuff the data into the struct.
      }
      mStatusID = -1;                           // Flag no command working.
      mStatusTimer.start();                     // Set our timer to start the next one.
    }
  }
}


void cellManager::showCellStatus(void) {
  
  out("------------ cellStatus ------------");outln;
  out("battery V  : ");out(statusReg.batteryVolts);out("mV\n");
  out("battery %  : ");out(statusReg.batteryPercent);out("%\n");
  out("RSSI       : ");out(statusReg.RSSI);outln;;
  out("Net Stat   : ");
  switch (statusReg.networkStat) {
    case 0 : out("Not registered"); break;
    case 1 : out("Reg. (home)"); break;
    case 2 : out("Reg. (searching)"); break;
    case 3 : out("Denied"); break;
    case 4 : out("Unknown"); break;
    case 5 : out("Reg. (roaming)"); break;
    default : out("Undefined"); break;
  }
  outln;
  out("volume     : ");out(statusReg.volume);out("\n");
  out("callStat   : ");out(statusReg.callStat);out("\n");
  out("numSMSs    : ");out(statusReg.numSMSs);out("\n");
  out("Net Time   : ");out(statusReg.networkTime);out("\n");
  out("------------------------------------");outln;
  out("------------------------------------");outln; 
}

void cellManager::idle(void) {

  qCMaster::idle();
  cleanList();
  if (mStatusTimer.ding()) {
    doStatus();
  }
  runCommand();
}
