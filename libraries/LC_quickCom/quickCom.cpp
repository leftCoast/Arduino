#include "quickCom.h"


// ***************************************************************************
// ********************************* qCMaster *********************************
// ***************************************************************************


qCMaster::qCMaster(void) {

	mBuff = NULL;
	mNumBytesMoved = 0;
	mWantReply = false;
	mError = NO_ERR;
	setReplyTimeout(MASTER_REPLY_TIMEOUT);
	mState = offline;
}


qCMaster::~qCMaster(void) {  }
	
	
void qCMaster::begin(int baud) {

	if (mState==offline) {			// A quick sanity check.
		MASTER_PORT.begin(baud);	// Give the port a kick to start.
		hookup();						// Puts us in the idler queue. 
		mState = standby;				// Then we are open for business!
	} else {
		mError = STATE_ERR;			// Getting your calls mixed up there?
	}
}


void qCMaster::setReplyTimeout(int timeOutMS) { setTime(timeOutMS,false); }


// Read, return and clear the last error.
byte qCMaster::readErr(void) {
	
	byte temp;
	
	temp = mError;
	mError = NO_ERR;
	return temp;
}


// Load up our outgoing buffer for sending. Set "sending" state.
bool qCMaster::sendBuff(byte* buff,byte buffLen,bool wantReply) {
	
	byte	i;
	
	if (mState==standby && buffLen) {		// Ok, if we've nothing better going on..
		if (resizeBuff(buffLen+1)) {			// we are abe to get a buffer.
			mNumBytesMoved = 0;
			mWantReply = wantReply;
			mBuff[0] = buffLen;
			for (i=0;i<buffLen;i++) {
				mBuff[i+1]=buff[i];
			}
			mState = sending;
			return true;
		} else {
			mError = MEMORY_ERR;
		}
	} else {
		mError = STATE_ERR;						// Getting your calls mixed up there?
	}
	return false;
}


// We have something to share with the class? This can be called anytime.
// Don't set error flags if called at "the wrong time". IE: No STATE_ERRs.
byte qCMaster::haveBuff(void) {
	
	if(mState==holding) {
		return mBuff[0];
	}
	return 0;
}


// This will read them all out. Use haveBuff() to know how
// many that is going to be.
void qCMaster::readBuff(byte* buff) {
	
	byte 	i;
	
	if(mState==holding) {
		for(i=0;i<mBuff[0];i++) {
			buff[i] = mBuff[i+1];
		}
		resizeBuff(0);
		mState = standby;
	} else {
		mError = STATE_ERR;						// We didn't say there was a buffer for you.
	}
}


// Or just toss it all away.. What a waste of effort that was!
void qCMaster::dumpBuff(void) {

	if(mState==holding) {
		resizeBuff(0);
		mState = standby;
	}
}



// The servant's entrance. All the work is being done behind the scenes.
// Here is where that work happens. 
void qCMaster::idle(void) {

	switch(mState) {
		case offline		:	
		case standby		:
		case holding		: break;							// In these cases, just exit as fast as possible.
		case listening 	: doListen();		break;	// Waiting by mailbox..
		case sending		: doSending();		break;	// Work on sending.
		case recieveing	: doReceiving();	break;	// Work on receiving.
		default				: break;							// It theoretically can't get here.
	
	}
}


// We already know we're sending something. Get busy.	
void qCMaster::doSending(void) {
	

	while(MASTER_PORT.availableForWrite() && mNumBytesMoved<=mBuff[0]) {
		MASTER_PORT.write(mBuff[mNumBytesMoved]);
		mNumBytesMoved++;
	}
	if (mNumBytesMoved == mBuff[0]+1) {
		resizeBuff(0);								// Done with it.
		if (mWantReply) {
			start();									// Start the reply timer.
			mState = listening;
		} else {
			mState = standby;
		}
	}
}


void qCMaster::doListen(void) {
	
	byte	numBytes;
	byte	dumpByte;
	
	if (ding()) {											// Time out? Nothing came?
		mError = TIMEOUT_ERR;							// Flag the error.
		mState = standby;									// Reset our state, sigh..
	} else {
		if (MASTER_PORT.available()) {				// Is there someting to read from the port?
			numBytes = MASTER_PORT.read();			// First byte SHALL BE the number of bytes for the message.
			if (resizeBuff(numBytes+1)) {				// Setup buffer.
				mBuff[0] = numBytes;						// Save size.
				mNumBytesMoved = 0;						// Setup for a reading..
				mState = recieveing;						//
				doReceiving();								// Might as well grab what we can now.
			} else {
				start();	
				while(MASTER_PORT.available() 
						&& mNumBytesMoved<numBytes
						&& !ding()) {						// Roll out the message on the floor.
					dumpByte = MASTER_PORT.read();	// Would have been nice to tell them, no answer coming.
					mNumBytesMoved++;
					delay(1);								// YES I KNOW I put a delay into a idle function. ONE milisecond.
				}
				mError = MEMORY_ERR;
			}
		}
	}
}


// Same as doSend() we already know we're receiving, get to it.
void qCMaster::doReceiving(void) {
	
	while(MASTER_PORT.available()
			&& mNumBytesMoved<mBuff[0]
			&& !ding()) {
		mNumBytesMoved++;
		mBuff[mNumBytesMoved] = MASTER_PORT.read();
	}
	if (mNumBytesMoved == mBuff[0]) {
		mState = holding;
	} else if (ding()) {
		resizeBuff(0);
		mError = TIMEOUT_ERR;
		mState = standby;
	}
}


// We're using a dynamically sized buffer. mBuff() & free(). This
// just auto-manages all of this.
bool qCMaster::resizeBuff(byte numBytes) {

	if (mBuff) {
		free(mBuff);
		mBuff = NULL;
	}
	if (numBytes) {
		mBuff = (byte*)malloc(numBytes);
		return mBuff != NULL;
	}
	return true;								// Because they asked for none.
}



// ***************************************************************************
// ********************************* qCSlave *********************************
// ***************************************************************************


qCSlave::qCSlave(void) {

	mNumBytesMoved = 0;
	mError = NO_ERR;
	mBuff = NULL;
	setComTimeout(SLAVE_COMMAND_TIMEOUT);
	mState = offline;
}
	
	
qCSlave::~qCSlave(void) {  resizeBuff(0); }


void qCSlave::begin(int baud) {

	if (mState==offline) {		// A quick sanity check. 
		SLAVE_PORT.begin(baud);	// Give the port a kick to start.
		hookup();					// Puts us in the idler queue.
		mState = listening;		// Then we are open for business!
	} else {
		mError = STATE_ERR;		// Getting your calls mixed up there?
	}
}


// How long after the first byte will we give the master to complete his buffer?
// If the master crashes or we're out of synk. Best to reset and try again.
void qCSlave::setComTimeout(int timeOutMS) { setTime(timeOutMS,false); }


// Read, return and clear the last error.
byte qCSlave::readErr(void) {
	
	byte temp;
	
	temp = mError;
	mError = NO_ERR;
	return temp;
}


// We have something to share with the class? This is used as the
// polling "do we need to deal with something?" call. So lets not
// toss out a billion pointless errors here.
byte qCSlave::haveBuff(void) {
	
	if(mState==holding) {
		return mBuff[0];
	}
	return 0;
}


// This will read them all out. Use haveBuff() to know how
// many that is going to be.
bool qCSlave::readBuff(byte* buff) {
	
	byte 	i;
	
	if(mState==holding) {
		for(i=0;i<mBuff[0];i++) {
			buff[i] = mBuff[i+1];
		}
		resizeBuff(0);
		mState = listening;
		return true;
	} else {
		mError = STATE_ERR;
	}
	return false;
}


// Send this as the reply buffer;
bool qCSlave::replyBuff(byte* buff,byte buffLen) {

	byte	i;
	
	if (mState == listening) {				// Yes, listening is still technically the correct state for a reply.
		if (resizeBuff(buffLen+1)) {		// Stretch the buffer to fit.		
			mBuff[0] = buffLen;
			for (i=0;i<=buffLen;i++) {		// Save off he info.
				mBuff[i+1]=buff[i];
			}
			mNumBytesMoved = 0;				// Nothing's moved yet.
			mState = replying;				// We're going to be replying now.
			return true;						// Success!
		} else {									// Oh ohh.. Not enough memory for the buffer!
			mError = MEMORY_ERR;				// Note it.
		}
	} else {
		mError = STATE_ERR;
	}
	return false;
}


// The servant's entrance. All the work is being done behind the scenes.
// Here is where that work happens. 
void qCSlave::idle(void) {

	switch(mState) {
		case offline		: 
		case holding		:	break;
		case listening		:	doListen();		break; 
		case recieveing	:	doReceiving();	break;
		case replying		:	doSending();	break;
		default				:	break;							// It theoretically can't get here.
	}
}


// The Feather seems to preload a Zero into the incoming stream when it fires up.
// This is an error and it only happens when the USB cable is not connected.
// So, we ignore leading zeros because they're an erros as a leading byte anyway.
void qCSlave::doListen(void) {
	
	byte	numBytes;
	byte	dumpByte;
	
	if (SLAVE_PORT.available()) {							// Is there someting to read from the port?
		numBytes = SLAVE_PORT.read();						// First byte SHALL BE the number of bytes for the message.
		if (numBytes>0) {										// First byte can't be a zero.
			if (resizeBuff(numBytes+1)) {					// Resize the buffer to hold the message.
				mBuff[0] = numBytes;							// That first byte shwing size.
				mNumBytesMoved = 0;							// Setup for a reading..
				start();											// We start the timer from the first byte.
				mState = recieveing;							//
				doReceiving();									// Might as well grab what we can now.
			} else {												// Now what should we do? We can't fit the message!
				start();	
				while(SLAVE_PORT.available() 
						&& mNumBytesMoved<=numBytes
						&& !ding()) {							// Roll out the message on the floor.
					dumpByte = SLAVE_PORT.read();			// Would have been nice to tell them, no answer coming.
					mNumBytesMoved++;
					delay(1);									// YES I KNOW I put a delay into a idle function. ONE milisecond.
				}
				mError = MEMORY_ERR;
			}
		}
	}
}


void qCSlave::doReceiving(void) {
	
	while(SLAVE_PORT.available()							// While there are bytes to read.
			&& mNumBytesMoved<mBuff[0]						// And we still HAVE bytes we NEED to read.
			&& !ding()) {										// AND, not time out (SENDER CRASHED)
		mNumBytesMoved++;										// Keep track of this.
		mBuff[mNumBytesMoved] = SLAVE_PORT.read();	// We read bytes!
	}
	if (mNumBytesMoved == mBuff[0]) {					// Got 'em all? Good!
		mState = holding;										// Move on!
	} else if(ding()) {										// Oh oh.. did the timer ding?
		resizeBuff(0);											// Recycle, reuse..
		mError = TIMEOUT_ERR;								// Master may have died.
		mState = listening;									// Go back to listening. Its all we can do..
	}
}

	
void qCSlave::doSending(void) {
	
	while(SLAVE_PORT.availableForWrite() && mNumBytesMoved<=mBuff[0]) {
		SLAVE_PORT.write(mBuff[mNumBytesMoved]);
		mNumBytesMoved++;	
	}
	if (mNumBytesMoved == mBuff[0]+1) {
		resizeBuff(0);
		mNumBytesMoved = 0;
		mState = listening;
	}
}


// We're using a dynamically sized buffer. malloc() & free(). This
// just auto-manages all of this.
bool qCSlave::resizeBuff(byte numBytes) {

	if (mBuff) {
		free(mBuff);
		mBuff = NULL;
	}
	if (numBytes) {
		mBuff = (byte*)malloc(numBytes);
		return mBuff != NULL;
	}
	return true;							// Because they asked for none.
}
