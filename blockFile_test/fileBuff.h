#ifndef fileBuff_h
#define fileBuff_h

#include "blockFile.h"


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
