#include <cellManager.h>
#include <cellCommon.h>

#define COM_TIMEOUT 	30000
#define STATUS_TIME	1250
#define ID_MAX       2000

#define out Serial.print
#define outln Serial.println()

cellManager	ourCellManager;
cellStatus	statusReg;
int			statusMS;

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
  startMS			= millis();
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

  mNextID				= 0;      // Start up the counter.
  mCurrentCommand		= NULL;   // No current command.
  mStatusID				= -1;     // Flag for no status cooking.
  mStatusTimer.setTime(STATUS_TIME);
  clearStatus();
  mPatchedCallStat	= false;
}


// Nothing really to do. Inherited will dump
// and delete all the remaining commands.
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
    if (numBytes>0) {                                   // Some sort of data going out?
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
// If no command is found, return com_missing as a flag.
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
      case com_working  : checkForReply(aCom);    break;  // Check to see if there is a reply.
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
    numBytes = aCom->getNumComBytes();                  // How big is this going to be?
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


void cellManager::checkForReply(cellCommand* aCom) {

  byte  numBytes;
  
  if (mState==holding) {                      		// You've got mail!
    numBytes = haveBuff();                    		// See how big it is..
    if (aCom->setReplySize(numBytes)) {       		// Tell the command to grab RAM.
      readBuff((uint8_t*)(aCom->replyData));  		// If it got the RAM stuff the reply data in there.
      aCom->comState = com_holding;           		// Flag that its holding a reply.
      mCurrentCommand = NULL;                 		// We're done with this guy.
    }                                         		// If we can't get the RAM? Try again later.
  }
}


void cellManager::getStatusTime(int commandID) {

	cellCommand*  trace;
  
	trace = findByID(commandID); 
	if (trace) {
		statusMS = millis() - trace->startMS;
	}
}
	

// A little side job here. We are entrusted with
// keeping the status block updated. This also gives
// a good example on how to use this machine.
void cellManager::doStatus(void) {

	byte  numBytes;
	int	tempStat;
	
	if (mStatusID==-1) {                              			// We need to fire one off?
		mStatusID = sendCommand(getStatus,true);       			// Well, that was easy.
	} else {                                          			// We have a current one to work with?
		switch(progress(mStatusID)) {                   		// See what its up to..
			case com_standby  : break;                    		// Its on the list.
			case com_working  : break;                    		// Its cooking now!
			case com_holding  :                           		// Ha! Got a reply!
				tempStat = statusReg.callStat;						// Just in case we patched the status, save this one.
				numBytes = numReplyBytes(mStatusID);        		// Reply size in bytes.
				if (numBytes) {                             		// Non zero means we got a reply.
					if (numBytes==sizeof(cellStatus)) {       	// We got the right byte count?
						readReply(mStatusID,(byte*)&statusReg);	// Stuff the data into the struct.
						if (mPatchedCallStat) {							// So, if we actually did update the stat between times..
							statusReg.callStat = tempStat;			// Patch this new, but actually older, status with the updated value;
						}
					}
					getStatusTime(mStatusID);
					mStatusID = -1;                           	// Flag no command working.
				}
				mPatchedCallStat = false;								// In any case we only pass by one. Clear the flag.
      	break;                              
			case com_complete :                           		// Its already been read but not dumped out.
			case com_missing  :                           		// Its already been deletd.
				clearStatus();
				mStatusID = -1;                             		// Flag no command working.
			break;
		}
	}
	mStatusTimer.start();                     					// Set our timer to start the next one.
}


// what should the status look like when we loose connection.
void	cellManager::clearStatus(void) {
	
	statusReg.FONAOnline			= 0;
	statusReg.batteryVolts		= 0;
	statusReg.batteryPercent	= 0;
	statusReg.RSSI					= 0;
	statusReg.networkStat		= NS_unknown;
	statusReg.volume				= 0;
	statusReg.callStat			= CS_unknown;
	statusReg.numSMSs				= 0;
	statusReg.statNum				= 0;
	//strcpy(statusReg.callerID,"unknown");
  	statusReg.errByte				= 0;
}


// Status can last a LONG time in cuputer-land. (Typically around a second.) If we see a
// change in status, we can patch it so as not to mess up the UI when it reads old data.
void cellManager::patchStatCallStat(int newCallStat) {

	statusReg.callStat = newCallStat;
	mPatchedCallStat = true;
}


/*
// This sends it out the serial port. Good for debugging.
void cellManager::showCellStatus(void) {
  
  out(F("------------ cellStatus ------------"));outln;
  out(F("FONA online: "));out(statusReg.FONAOnline);out("\n");
  out(F("battery V  : "));out(statusReg.batteryVolts);out(F("mV\n"));
  out(F("battery %  : "));out(statusReg.batteryPercent);out(F("%\n"));
  out(F("RSSI       : "));out(statusReg.RSSI);outln;;
  out(F("Net Stat   : "));
  switch (statusReg.networkStat) {
    case 0 : out(F("Not registered")); break;
    case 1 : out(F("Reg. (home)")); break;
    case 2 : out(F("Reg. (searching)")); break;
    case 3 : out(F("Denied")); break;
    case 4 : out(F("Unknown")); break;
    case 5 : out(F("Reg. (roaming)")); break;
    default : out(F("Undefined")); break;
  }
  outln;
  out(F("volume     : "));out(statusReg.volume);out(F("\n"));
  out(F("callStat   : "));out(statusReg.callStat);out(F("\n"));
  out(F("numSMSs    : "));out(statusReg.numSMSs);out(F("\n"));
  out(F("statNum    : "));out(statusReg.statNum);out(F("\n"));
  //out(F("Caller ID  : "));out(statusReg.callerID);out(F("\n"));
  out(F("Error Byte : "));out(statusReg.errByte);out(F("\n"));
  out(F("------------------------------------"));outln;
  out(F("------------------------------------"));outln; 
}
*/

// And here's what we do over and over in the background idle() loop.
void cellManager::idle(void) {

  qCMaster::idle();									// Let the communications code do its thing. (We inherit from qCMaster, so we need to patch its idle(); call.)
  cleanList();											// Check to see if any commands need recycling.
  if (mStatusID!=-1||mStatusTimer.ding()) {	// If working on a status or its time to update status..
    doStatus();										// Fire off or check a status command.
  }		
  runCommand();										// And finally, we have a look at our command list to see if anything needs doing.
}
