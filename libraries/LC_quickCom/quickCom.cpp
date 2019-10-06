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
byte qCMaster::readErr(bool clearErr) {
	
	byte temp;

	temp = mError;
	if (clearErr) mError = NO_ERR;
	return temp;
}


// Load up our outgoing buffer for sending. Set "sending" state.
bool qCMaster::sendBuff(byte* buff,byte buffLen,bool wantReply) {
	
	byte	i;
	
	if (mState==standby && buffLen) {		// Ok, if we've nothing better going on..
		if (resizeQBuff(buffLen+1)) {			// we are able to get a buffer.
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
		resizeQBuff(0);
		mState = standby;
	} else {
		mError = STATE_ERR;						// We didn't say there was a buffer for you.
	}
}


// Or just toss it all away.. What a waste of effort that was!
void qCMaster::dumpBuff(void) {

	if(mState==holding) {
		resizeQBuff(0);
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
	
	while(MASTER_PORT.available()) {				// Is there someting left over in the port?
			MASTER_PORT.read();						// Flush it out!
	}
	while(MASTER_PORT.availableForWrite() && mNumBytesMoved<=mBuff[0]) {
		MASTER_PORT.write(mBuff[mNumBytesMoved]);
		mNumBytesMoved++;
	}
	if (mNumBytesMoved == mBuff[0]+1) {
		resizeQBuff(0);									// Done with it.
		if (mWantReply) {
			start();										// Start the reply timer.
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
			if (resizeQBuff(numBytes+1)) {			// Setup buffer.
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
		resizeQBuff(0);
		mError = TIMEOUT_ERR;
		mState = standby;
	}
}


// We're using a dynamically sized buffer. mBuff() & free(). This
// just auto-manages all of this.
bool qCMaster::resizeQBuff(byte numBytes) {

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

	mNumBytesMoved = 0;								// Not moved any yet..
	mError = NO_ERR;									// Not had an error yet either. Imagine!
	mBuff =	NULL;										// Save off the address of our buffer.
	mNumBytes = 0;
	setComTimeout(SLAVE_COMMAND_TIMEOUT);		// Set the deadman timer.
	mState = offline;
	
}
	
	
qCSlave::~qCSlave(void) { }


// The slave part is different. The owner of this bit sets up a single buffer for
// both input and output. Pass in the size and the address of this buffer. Then when
// data comes in, it'll be written into this buffer. Then when you want to send a
// reply you write it onto this same buffer. This is to save as much RAM footprint
// as possible for -little- processors.
//
// NOTE : The most bytes that can be sent is 255. So don't go overboard on buffer
// size.
void qCSlave::begin(byte* buff,byte numBytes,int baud) {

	if (mState==offline) {		// A quick sanity check. 
		mBuff =	buff;				// Save off the address of our buffer.
		mNumBytes = numBytes;	// How many bytes is that buffer?
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


// Get the comunication buffer;
byte* qCSlave::getComBuff(void) {

	return &(mBuff[1]);
}


// Send the reply on its way.
void qCSlave::replyComBuff(byte numBytes) {

	if (mState == holding) {	// We have to be holding to want a reply.
		mBuff[0] = numBytes;		// 
		mNumBytesMoved = 0;
		mState = replying;
	} else {
		mError = STATE_ERR;		// Getting your calls mixed up there?
	}
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
			if (mNumBytes>=(numBytes+1)) {				// If we can hold the message.
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
		mError = TIMEOUT_ERR;								// Master may have died.
		mState = listening;									// Go back to listening. Its all we can do..
	}
}

	
void qCSlave::doSending(void) {
	
	while(SLAVE_PORT.availableForWrite() && mNumBytesMoved<=mBuff[0]) {	// While we can write something and we have something to write..
		SLAVE_PORT.write(mBuff[mNumBytesMoved]);									// Write out a byte.
		mNumBytesMoved++;																	// Increment the index.
	}
	if (mNumBytesMoved == mBuff[0]+1) {												// If we've moved them all..
		mState = listening;																// Go back to listening.
	}
}



