#ifndef quickCom_h
#define quickCom_h

#include	<idlers.h>
#include <timeObj.h>


#define NO_ERR			0
#define STATE_ERR		1
#define TIMEOUT_ERR	2
#define MEMORY_ERR	3


//	The idea..
// Master side :
// The master initiates communication. Sends a command and optionally waits for a reply.
//
// Slave side :
// Slaves listen for a commands, take action on those commands and usually generates
// replies to those commands.
//
// Now these are just data, smallish blocks of byes. They are not thought of as
// c strings just blocks of data. You can send a cstring through this, up to 255 bytes.
// But don't start thinking you have to add a byte for the trailing EOS or anything.
// Its just blocks of bytes.
//
// I put that last note in because I forgot and had to come over here and look.


#define MASTER_PORT	Serial1	// For now this is all I could come up with.
#define SLAVE_PORT	Serial1	// Change these to reflect your hardware setup.

#define	MASTER_REPLY_TIMEOUT		15000		// Defaults. How long to wait for a reply.
#define	SLAVE_COMMAND_TIMEOUT	 1000		// How long to finish receiving a command.


class qCMaster : public idler, public timeObj {

	public:
				enum masterStates { offline, standby, sending, listening, recieveing, holding };
				
				qCMaster(void);			// Get our port and set initial values.
	virtual	~qCMaster(void);			// We're done here.
	
				void				begin(int baud);												// Get the baud rate and fire up the connection.
				void				setReplyTimeout(int timeOutMS);							// How long we'll wait for a reply to complete from the command.
				byte				readErr(bool clearErr=true);								// What's wrong? 			
				bool				sendBuff(byte* buff,byte buffLen,bool wantReply);	// Send out a command buffer.
				bool				isSending(void);												// Typically use haveBuff() to see when done. This is for blind sending.
				byte				haveBuff(void);												// We have a got back complete buffer, how big?
				void				readBuff(byte* buff);										// Copy the data we got back into here.
				void				dumpBuff(void);												// For some reason we can't use it.
	
	virtual	void				idle(void);														// The servants run things in the background.
				void				doSending(void);
				void				doListen(void);
				void				doReceiving(void);
				bool				resizeQBuff(byte numBytes);
				
				masterStates	mState;
				byte				mNumBytesMoved;
				bool				mWantReply;
				byte				mError;
				byte*				mBuff;
};



class qCSlave : public idler, public timeObj {

	public:
				enum slaveStates { offline, listening, recieveing, holding, replying };
				
				qCSlave(void);						// Get our port and set initial values.
	virtual	~qCSlave(void);												// We're done here.

				void				begin(byte* buff,byte numBytes,int baud);				// Get buffer bytes & baud then fire up the connection.
				void				setComTimeout(int timeOutMS=1000);	// How long we will wait for command to complete.
				byte				readErr(void);								// Something broken?
				byte				haveBuff(void);							// We received a buffer, how big?
				void				slaveReset(void);							// Call this if there is to be no reply.
				byte*				getComBuff(void);							// Hand back a pointer to the start of the com buffer.
				void				replyComBuff(byte buffLen);			// Com buffer has been loaded with data, send it back.

	virtual	void				idle(void);									// Again, the servants run things in the background.
				void				doListen(void);
				void 				doReceiving(void);
				void				doSending(void);
				
				slaveStates	mState;
				byte			mNumBytesMoved;
				byte			mError;
				byte*			mBuff;
				byte			mNumBytes;

};
#endif

	
	