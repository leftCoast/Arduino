#include <cellManager.h>
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

/*
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
*/


// How many bytes are you going to need to fit this command?
byte  cellCommand::getNumComBytes() {

  return numComBytes+1;
}


// Commands are command byte followed by up to 254 data byes.
void cellCommand::getComBuff(uint8_t* buff) {

  buff[0] = commandNum;
 for(byte i=1;i<=numComBytes;i++) {
  buff[i] = comData[i-1];
 }
}


// We are going to be handed a data buffer, allocate room for it.
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


// Send a one byte command. Returns an ID#.
int cellManager::sendCommand(byte commandNum,bool wantReply) {

  return sendCommand(commandNum,0,NULL,wantReply);                    
}


// Send  a one byte command with data string. Returns an ID#.
int cellManager::sendCommand(byte commandNum,char* dataStr,bool wantReply) {

  byte  numBytes;
  
  numBytes = 0;
  if (dataStr) {
    numBytes = strlen(dataStr)+1;
  }
  return sendCommand(commandNum,numBytes,(uint8_t*)dataStr,wantReply);                  
}


// Send  a one byte command with data block. Returns an ID#.
int cellManager::sendCommand(byte commandNum,byte numBytes,uint8_t* comData,bool wantReply) {

  cellCommand*  newCommand;

  newCommand = new cellCommand;                         // Whip up a new command block.
  if (newCommand) {                                     // If we got one, good!
    newCommand->commandID = ++mNextID;                  // Stuff in a new ID#
    newCommand->commandNum = commandNum;                // The command they want to send.
    newCommand->wantReply = wantReply;                  // Do they want a reply?
    newCommand->numComBytes = numBytes;                 // Note the buff size.
    if (numBytes>0) {                                   // Som sort of data going out?
      newCommand->comData = (uint8_t*)malloc(numBytes); // Ask for some RAM.
      if (newCommand->comData) {                        // If we got the RAM..
        for(byte i=0;i<numBytes;i++) {                  // Loop through an fill in the buff.
          newCommand->comData[i]=comData[i];
        }
      } else {                                          // Asked for RAM, got none. Our house is falling down!
        delete newCommand;                              // Toss the new command block.
        return -1;                                      // Bail out with an error.
      }
    }
    addToEnd(newCommand);                               // Add the command block to the Queue.
    return newCommand->commandID;                       // Success at this point.
  }                                                       
  return -1;                                            // Agaain, we're out of RAM. Maybe we should have joined the other side?
}


// Find a command by ID. And, if found? Return the status of that command.
// If no command is found, return -1 as a flag.
int cellManager::progress(int commandID) {

  cellCommand*  trace;

  trace = findByID(commandID); 
  if (trace) {
    return trace->comState;
  }
  return com_missing;           // probably recycled.
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


// Find this commad and read its reply.
void cellManager::readReply(int commandID,uint8_t* buff) {

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
// not "in proccess" mark it for deletion.
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
cellCommand* cellManager::findByID(int commandID) {

  cellCommand*  trace;

  trace = (cellCommand*)getFirst();
  while(trace) {
    if (trace->commandID==commandID) return trace;
    trace = (cellCommand*)trace->getNext();
  }
  return NULL;      // Meaning not found.
}


void cellManager::cleanList(void) {

  cellCommand*  trace;
  cellCommand*  temp;
  
  trace = (cellCommand*)getFirst();                     // Grab one off the top.
  while(trace) {                                        // While we're not sitting on a NULL.
    if (trace->ding()||trace->comState==com_complete) { // A expired or copleted command?
      temp = trace;                                     // Save a pointer to it.
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
  
  if (mCurrentCommand) {                        // Already working on one?
    return mCurrentCommand;                     // Grab it and go.
  }
  trace = (cellCommand*)getFirst();             // Grab one off the top.
  while(trace) {                                // While we're not sitting on a NULL.
    if (trace->comState==com_standby            // Starting at the top, these two  
        || trace->comState==com_working ) {     // are the ones we are interested in.
      mCurrentCommand = trace;                  // We'll do this guy!
      return mCurrentCommand;                   // Here 'tis!
    }
    trace = (cellCommand*)trace->getNext();
  }
  return NULL;                                  // Couldn't find any work.
}


// Give time to the actual task of running out the commands.
void cellManager::runCommand(void) {

  cellCommand*  aCom;

  aCom = getCurrent();                              // Grab either what we're working on or a new command.
  if (aCom) {                                       // Got one? Ok!
    switch(aCom->comState) {
      case com_standby  : launchCommand(aCom);    break;  // Launch it and go.
      case com_working  : checkForRreply(aCom);   break;  // Check to see if there is a reply.
      case com_holding  : 
      case com_complete :
      case com_missing  : mCurrentCommand = NULL; break;  // In these cases there's nothing more we can do.
    }
  }
}



void cellManager::launchCommand(cellCommand* aCom) {
  
  uint8_t*  ourComBuff;
  byte      numBytes;
  
  if (mState==holding) {                                // Oh ohh.. Someone left something behind in here.
    dumpBuff();                                         // Dump it out.
  }
  if (mState==standby) {                                // We're waiting too!
    numBytes = aCom->getNumComBytes();                  // Howw big is this going to be?
    ourComBuff = (uint8_t*)malloc(numBytes);            // Grab some RAM.
    if (ourComBuff) {                                   // Got it?
      aCom->getComBuff(ourComBuff);                     // Get the command buffer.
      readErr();                                        // Clear out old errors.
      sendBuff(ourComBuff,numBytes,aCom->wantReply);    // Send it out, get response.
      free(ourComBuff);                                 // Done with the command buff.
      if (!readErr()) {                                 // No troubles?
        if (aCom->wantReply) {                          // If they want a reply..
          aCom->comState = com_working;                 // Set it as working and lets go!                  
        } else {                                        // If they don't want a reply, its job is done.
          aCom->comState = com_complete;                // Mark for disposal;
          mCurrentCommand = NULL;                       // No current command.
        }
      }                                                 // If a write error? Do nothing and retry later.
    }                                                   // Memory Error? Again, just retry later.
  }
}


void cellManager::checkForRreply(cellCommand* aCom) {

  byte  numBytes;
  
  if (mState==holding) {                      // You've got mail!
    numBytes = haveBuff();                    // See how big it is..
    if (aCom->setReplySize(numBytes)) {       // Tell the command to grab RAM.
      readBuff((uint8_t*)(aCom->replyData));  // If it got the RAM stuff the reply data in there.
      aCom->comState = com_holding;           // Flag that its holding a reply.
      mCurrentCommand = NULL;                 // We're done with this guy.
    }                                         // If we can't get the RAM? Try again later.
  }
}


// A little side job here. We are entrusted with
// keeping the status block updated. This also gives
// a good example on how to use this machine.
void cellManager::doStatus(void) {

  byte  numBytes;

  if (mStatusID==-1) {                              // We need to fire one off?
     mStatusID = sendCommand(getStatus,true);       // Well, that was easy.
  } else {                                          // We have a current one to work with?
    switch(progress(mStatusID)) {                   // Se what its up to..
      case com_standby  : break;                    // Its on the list.
      case com_working  : break;                    // Its cooking now!
      case com_holding  :                           // Ha! Got a reply!
        numBytes = numReplyBytes(mStatusID);        // Reply size in bytes.
        if (numBytes) {                             // Non zero means we got a reply.
          if (numBytes==sizeof(cellStatus)) {       // We got the right byte count?
            readReply(mStatusID,(byte*)&statusReg); // Stuff the data into the struct.
          }
          mStatusID = -1;                           // Flag no command working.
          mStatusTimer.start();                     // Set our timer to start the next one.
        }
      break;                              
      case com_complete :                           // Its already been read but not dumped out.
      case com_missing  :                           // Its already been deletd.
        mStatusID = -1;                             // Flag no command working.
        if (mStatusTimer.ding()) {                  // If the timer is still running, H=Just let it go.
          mStatusTimer.start();                     // If the timer has expired, reset it.
        }
      break;
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