#ifndef fileBuff_h
#define fileBuff_h

#include "blockFile.h"

// IF you are an object that is going to be stored as a block in a blockFile.
// THEN THIS is a good class to be derived from. This handles the underlying
// nonsense of getting you in and out of the file. Other objects will suddenly
// know just how to put you in a file and pull you out. Won't that be nice?
//
// There are three methods you will need to inherit and make your own.
// calculateBuffSize() - How large of a buffer in bytes will you need to "flatten"
// yourself into?
//
// writeToBuff(char* buffPtr,unsigned long maxBytes); - Do the "flatten" thing.
// Write the information you need to re-construct yourself into a data buffer.
//
// loadFromBuff(char* buffPtr,unsigned long maxBytes); - Given a data buffer,
// reconstruct yourself into your former glory!
//
// saveSubFileBuffs() Is a method you MAY need to inherit and fill out. IF you
// manage a list of sub fileBuffs, this is the call that tells you its time to
// save them to disk. Or SD card. This is because the next thing your going to
// see is a call to write yourself onto the disk. If you don't manage a group of 
// file blocks, then just ignore this method.
//
// You see, some other class will have a list of file block IDs and used them
// to pull you and your siblings off of a SD card into memory. Then, when your
// task is complete, use these same file IDs to put your updated self back on
// the SD card.


class fileBuff {

	public:		
				fileBuff(blockFile* inFile);									// constructor for root ID
				fileBuff(blockFile* inFile,unsigned long blockID);		// constructor for all those "other guys".
	virtual	~fileBuff(void);

				unsigned	long	getID(void);
	virtual	unsigned	long	calculateBuffSize(void);
	virtual	void				writeToBuff(char* buffPtr,unsigned long maxBytes);
	virtual	unsigned	long	loadFromBuff(char* buffPtr,unsigned long maxBytes);
	virtual	bool				saveSubFileBuffs(void);
	virtual	void				eraseFromFile(void);
				bool				saveToFile(void);
				bool				readFromFile(void);
	
				
				unsigned long	mID;
				blockFile*		mFile;
				char*				mBuff;
};

#endif
