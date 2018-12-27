#include "fileBuff.h"

fileBuff::fileBuff(blockFile* inFile,unsigned long blockID) {
  //tracer trace("fileBuff::fileBuff()");
  
    mFile = inFile;
    mID = blockID;
    mBuff = NULL;
}


fileBuff::~fileBuff(void) { }
  //tracer trace("fileBuff::~fileBuff()");
  

unsigned long fileBuff::getID(void) { 
  //tracer trace("fileBuff::getID()");
    
  return mID;
  }


// Inherited will overwrite this one.
unsigned long fileBuff::calculateBuffSize(void) {
  //tracer trace("fileBuff::calculateBuffSize()");
  
  return 0;
}

// Inherited will overwrite this one.
void fileBuff::writeToBuff(char* buffPtr,unsigned long maxBytes) {  }


// Inherited will overwrite this one.
unsigned long fileBuff::loadFromBuff(char* buffPtr,unsigned long maxBytes) {  }


bool fileBuff::saveToFile(void) {
  //tracer trace("fileBuff::saveToFile()");
  
  unsigned long numBytes;
  bool          success;

  success = false;
  if (mFile) {
    numBytes = calculateBuffSize();
    if (numBytes) {
      mBuff = (char*)malloc(numBytes);
      if (mBuff) {
        writeToBuff(mBuff,numBytes);
        success = mFile->writeBlock(mID,mBuff,numBytes);
        free(mBuff);
        mBuff = NULL;
      }
    }
  }
  return success;
}


bool fileBuff::readFromFile(void) {
  //tracer trace("fileBuff::readFromFile()");
  
  unsigned long numBytes;
  bool          success;

  success = false;
  if (mFile) {
    numBytes = mFile->getBlockSize(mID);
    if (numBytes) {
      mBuff = (char*)malloc(numBytes);
      if (mBuff) {
        if (mFile->getBlock(mID,mBuff,numBytes)) {
          loadFromBuff(mBuff,numBytes);
          success = true;
        }
        free(mBuff);
        mBuff = NULL;
      }
    }
  }
  return success;
}
