
#include "cellListener.h"	// Well, duh.
#include <cellManager.h>	// Currently where the status register is living.
#include	"../../cellOS.h"	// Our job is to open panels. litlOS is the guy for this.
         
cellListener	ourListener;
bool				callIncoming; 
bool				newSMSmsg;
char*				SMSMsg;
byte				SMSBuff[256];


cellListener::cellListener(void)
  :idler() {
    
  callIncoming = false;
  answerID = -1;
  mSMSID = -1;
  mSMSIndex = 1;
  SMSMsg = (char*)&(SMSBuff[1]);
  newSMSmsg = false;
}


cellListener::~cellListener(void) {  }


void cellListener::begin(int answerPanelID) {

	hookup();
	answerID = answerPanelID;
}


void cellListener::idle(void) {
	
	if (!callIncoming) {
		if (statusReg.callStat == CS_ringingIn) {
			callIncoming = true;
			nextPanel = answerID;
		}
	}  else {
		if(statusReg.callStat != CS_ringingIn) {
			callIncoming = false;
		}
	}
	if (statusReg.numSMSs>0 && !newSMSmsg) {
		doSMS();
	}		
}


void cellListener::doSMS(void) {

	byte  numBytes;
	
	if (mSMSID==-1) {																// No command were working on?
		mSMSIndex++;																// Bump the index.	
		if (mSMSIndex>30) mSMSIndex=1;										// Keep in inbounds.												
		mSMSID = ourCellManager.sendCommand(getSMS,true);				// Send off the new query.
	} else {                                          					// We have a current one to work with?
		switch(ourCellManager.progress(mSMSID)) {                   // See what its up to..
			case com_standby  : break;                    				// Its on the list.
			case com_working  : break;                    				// Its cooking now!
			case com_holding  :                           				// Ha! Got a reply!
				numBytes = ourCellManager.numReplyBytes(mSMSID);        					// Reply size in bytes.
				if (numBytes>1) {													// More than 1 means we got a reply.
					ourCellManager.readReply(mSMSID,&(SMSBuff[0])); 						// Stuff the data into the buffer.
					newSMSmsg = true;												// Tell the world to come and get it!
				} else {
					ourCellManager.dumpReply(mSMSID);						// Dump the errors, most likely an empty index.
				}
				mSMSID = -1;                           					// Flag no command working.
			break;                              
			case com_complete :                           				// Its already been read but not dumped out.
			case com_missing  :                           				// Its already been deletd.
				mSMSID = -1;                             					// Flag no command working.
			break;
		}
	}
}




