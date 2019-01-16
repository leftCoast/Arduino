#include "fileBuff.h"


//constructor for root ID
fileBuff::fileBuff(blockFile* inFile) {
  //tracer trace("fileBuff::fileBuff() for root.");
  
    mFile = inFile;							// Allright, the blockfile.
    mBuff = NULL;								// Init our buff pointer.
    mID = mFile->readRootBlockID();		// Read the root ID.
    if (mID==0) {								// Zero!? Wow, brand new file!
    	mID = mFile->getNewBlockID();		// It says, first allocated will be root.
    }  	
}


// constructor for all those "other" guys.
fileBuff::fileBuff(blockFile* inFile,unsigned long blockID) {
  //tracer trace("fileBuff::fileBuff() normal flavor.");
  
    mFile = inFile;
    mID = blockID;
    mBuff = NULL;
}


fileBuff::~fileBuff(void) {  }
  //tracer trace("fileBuff::~fileBuff()");
  

unsigned long fileBuff::getID(void) { 
  //tracer trace("fileBuff::getID()");
    
  return mID;
  }


// Inherited will overwrite this one.
unsigned long fileBuff::calculateBuffSize(void) { return 0; }


// Inherited will overwrite this one.
void fileBuff::writeToBuff(char* buffPtr,unsigned long maxBytes) {  }


// Inherited will overwrite this one.
unsigned long fileBuff::loadFromBuff(char* buffPtr,unsigned long maxBytes) { return 0; }


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
