#ifndef quickCom_h
#define quickCom_h

#include	<idlers.h>
#include <timeObj.h>

//	The idea..
// Master side :
// The master initiates communication. Sends a command and optionally waits for a reply.
//
// Slave side :
// Slaves listen for a commands, take action on those commands and usually generate
// replies to those commands.

#define NO_ERR			0
#define PORT_ERR		1
#define STATE_ERR		2
#define TIMEOUT_ERR	3
#define MEMORY_ERR	4

#define MASTER_PORT	Serial1	// For now this ia all I could come up with.
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
				byte				readErr(void);													// What's wrong? 			
				bool				sendBuff(byte* buff,byte buffLen,bool wantReply);	// Send out a command buffer.
				byte				haveBuff(void);												// We have a got back complete buffer, how big?
				void				readBuff(byte* buff);										// Copy the data we got back into here.
	
	virtual	void				idle(void);														// The servants run things in the background.
				void				doSending(void);
				void				doListen(void);
				void				doReceiving(void);
				bool				resizeBuff(byte numBytes);
				
				masterStates	mState;
				byte				mNumBytesMoved;
				bool				mWantReply;
				byte				mError;
				byte*				mBuff;
};



class qCSlave : public idler, public timeObj {

	public:
				enum slaveStates { offline, listening, recieveing, holding, replying };
				
				qCSlave(void);		// Get our port and set initial values.
	virtual	~qCSlave(void);	// We're done here.

				void				begin(int baud);							// Get the baud rate and fire up the connection.
				void				setComTimeout(int timeOutMS=1000);	// How long we will wait for command to complete.
				byte				readErr(void);								// Something broken?
				byte				haveBuff(void);							// We received a buffer, how big?
				bool				readBuff(byte* buff);					// Copy the data we got back into here.
				bool				replyBuff(byte* buff,byte buffLen);	// Send a reply buffer.

	virtual	void				idle(void);									// Again, the servants run things in the background.
				void				doListen(void);
				void 				doReceiving(void);
				void				doSending(void);
				bool				resizeBuff(byte numBytes);
				
				slaveStates	mState;
				byte			mNumBytesMoved;
				byte			mError;
				byte*			mBuff;
};
#endif

	
	