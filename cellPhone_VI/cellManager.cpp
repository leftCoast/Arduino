#include "cellManager.h"

#define COM_TIMEOUT 2000



// *****************************************************
// ******************  cellCommand *********************
// *****************************************************


cellCommand::cellCommand(void) 
  : linkListObj(),
  timeObj(COM_TIMEOUT) {

  commandNum      = 0;
  numComBytes     = 0;
  comData         = NULL;
  wantReply       = false;
  numReplyBytes   = 0;
  replyData       = NULL;
  commandID       = 0;
  comState        = com_standby;
};


cellCommand::~cellCommand(void) {

  if(comData) free(comData);
  if(replyData) free(replyData);
}
  


// *****************************************************
// ******************  cellManager *********************
// *****************************************************


cellManager::cellManager(void)
  : qCMaster(),
  linkList() {

  mNextID = 0;  
}


// Nothing really to do. Inherited will dump
// and delete all the rmaining commands.
cellManager::~cellManager(void) {  }


int cellManager::sendCommand(byte inCommandNum,char* inComData,bool inWantReply) {

  cellCommand*  newCommand;
  byte          numBytes;
  
  newCommand = new cellCommand;
  if (newCommand) {
    newCommand->commandID = mNextID++;
    newCommand->commandNum = inCommandNum;
    numBytes = strlen(inComData);
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
      newCommand->wantReply = inWantReply;
    }
    addToEnd(newCommand);
    return newCommand->commandID;                         // Success at this point.
  }                                                       
  return -1;                                              // Agaain, we're out of RAM. Maybe we should have joined the other side?
}



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
byte cellManager::numReplyBytes(int commandID) {

  cellCommand*  trace;

  trace = findByID(commandID); 
  if (trace) {
    if (trace->comState==com_holding) {
      return trace->numReplyBytes;
    }
  }
  return 0;
}


// Fund this commad and read its reply. A successful read will
// delete the command.
void cellManager::readReply(int commandID,byte* buff) {
  
  cellCommand*  trace;
  byte          i;
  
  trace = findByID(commandID); 
  if (trace) {
    if (trace->comState==com_holding) {
      for(i=0;i<trace->numReplyBytes;i++) {
        buff[i] = trace->replyData[i+1];
      }
      unlinkObj(trace);                       // At this point this command has completed its useful life.
      delete trace;                           //  Recycle.
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
      unlinkObj(trace);                                                   // At this point this command has completed its useful life.
      delete trace;                                                       //  Recycle.
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
