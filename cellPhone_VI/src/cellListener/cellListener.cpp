
#include "cellListener.h"						// Well, duh.
#include <cellManager.h>						// Currently where the status register is living.
#include "../contacts/contactPanel.h"		// We pass off message to these guys.
#include	"../../cellOS.h"						// One of our jobs is to open panels. litlOS is the guy for this.

#define PN_BUFF_BYTES    20
#define MSG_BUFF_BYTES   200
#define RAW_BUFF_BYTES   255
         
cellListener	ourListener;
bool				callIncoming;
bool				newSMSmsg;
char				SMSPN[PN_BUFF_BYTES];
char				SMSMsg[MSG_BUFF_BYTES];
char				SMSRaw[RAW_BUFF_BYTES];


cellListener::cellListener(void)
  :idler() {
    
  callIncoming = false;
  answerID		= -1;
  mSMSID			= -1;
  mSMSIndex		= 1;							
  newSMSmsg		= false;
  SMSPN[0] 		= '\0';
  SMSMsg[0] 	= '\0';
  SMSRaw[0] 	= '\0';
}


cellListener::~cellListener(void) {  }


void cellListener::begin(int answerPanelID) {

	hookup();
	answerID = answerPanelID;
}


void cellListener::idle(void) {
	
	if (!callIncoming) {													// If we've not seen an incoming call yet..
		if (statusReg.callStat == CS_ringingIn) {					// Oh! A call's coming in!
			callIncoming = true;										// Tell the world we have a call.
			nextPanel = answerID;									// Call up the telephone App.
		}
	}  else {
		if(statusReg.callStat != CS_ringingIn) {
			callIncoming = false;
		}
	}
	if (statusReg.numSMSs>0) {
		doSMS();
	} else {
		mSMSIndex = 1;											// No messages? Restart our next search at 1.
	}
}


void cellListener::doSMS(void) {

	byte  numBytes;
	byte	indexBuff[2];
	
	indexBuff[1] = 0;																			// Going to pretend its a string.
	if (mSMSID==-1) {																			// No command were working on?	
		indexBuff[0] = mSMSIndex;															// Pop in the message slot we want.
		mSMSID = ourCellManager.sendCommand(getSMS,(char*)indexBuff,true);	// Send off the new query.
		mSMSIndex++;																			// Bump the index.	
		if (mSMSIndex>30) mSMSIndex=1;													// Keep in inbounds.
	} else {                                          								// We have a current one to work with?
		switch(ourCellManager.progress(mSMSID)) {                   			// See what its up to..
			case com_standby  : break;                    							// Its on the list.
			case com_working  : break;                    							// Its cooking now!
			case com_holding  :                           							// Ha! Got a reply!
				numBytes = ourCellManager.numReplyBytes(mSMSID);      			// Reply size in bytes.
				if (numBytes>1) {																// More than 1 means we got a reply.
					ourCellManager.readReply(mSMSID,(uint8_t*)SMSRaw); 			// Stuff the data into the buffer.
					decodeSMS();																// Parse it into the two buffers.
					ourBlackBook->addMessage(SMSPN,SMSMsg);							// Save it to the contact list.
					newSMSmsg = true;															// Tell the world to come and get it!
				} else {
					ourCellManager.dumpReply(mSMSID);									// Dump the errors, most likely an empty index.
				}
				mSMSID = -1;                           								// Flag no command working.
			break;                              
			case com_complete :                           							// Its already been read but not dumped out.
			case com_missing  :                           							// Its already been deletd.
				mSMSID = -1;                             								// Flag no command working.
			break;
		}
	}
}


void cellListener::decodeSMS(void) {

	byte		index;
	
	strcpy(SMSPN,&(SMSRaw[1]));		// Byte 0 is error, after that is the phone number.
	index = strlen(SMSPN)+2;			// Locate the message part. Add two, error byte & EOS byte.
	strcpy(SMSMsg,&(SMSRaw[index]));	// Copy out the message.
	filterPNStr(SMSPN);					// Clear out the phone number junk.
}

