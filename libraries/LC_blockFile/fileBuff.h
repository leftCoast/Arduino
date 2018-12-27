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
// Write the information you need to construct yourself into a data buffer.
//
// loadFromBuff(char* buffPtr,unsigned long maxBytes); - Given a data buffer,
// reconstruct yourself into your former glory!
//
// You see, some other class will have a list of file block IDs and used them
// to pull you and your siblings off of a SD card into memory. Then, when your
// task is complete, use these same file IDs to put your updated self back on
// the SD card.


class fileBuff {

  public:
  
          fileBuff(blockFile* inFile,unsigned long blockID);
  virtual ~fileBuff(void);

          unsigned long   getID(void);
  virtual unsigned long   calculateBuffSize(void);
  virtual void            writeToBuff(char* buffPtr,unsigned long maxBytes);
  virtual unsigned long   loadFromBuff(char* buffPtr,unsigned long maxBytes);
          bool            saveToFile(void);
          bool            readFromFile(void);

  unsigned long mID;
  blockFile*    mFile;
  char*         mBuff;
};

#endif
