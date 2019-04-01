#include "fileBuff.h"


// constructor for root ID. For explanation of root file ID see : blockFile.h
fileBuff::fileBuff(blockFile* inFile) {
  
	mFile	= inFile;							// Allright, the blockfile.
	mBuff	= NULL;								// Init our buff pointer.
	mID	= mFile->readRootBlockID();	// Read the root ID.
	if (mID==0) {								// Zero!? Wow, brand new file!
		mID = mFile->getNewBlockID();		// It says, first allocated will be root.
	}  	
}


// constructor for all those "other" guys.
fileBuff::fileBuff(blockFile* inFile,unsigned long blockID) {
  
    mFile	= inFile;
    mID		= blockID;
    mBuff	= NULL;
}


fileBuff::~fileBuff(void) {  }
  
  
// Papers please..
unsigned long fileBuff::getID(void) { return mID; }


// Inherited will overwrite this one.
unsigned long fileBuff::calculateBuffSize(void) { return 0; }


// Inherited will overwrite this one.
void fileBuff::writeToBuff(char* buffPtr,unsigned long maxBytes) {  }


// Inherited will overwrite this one.
unsigned long fileBuff::loadFromBuff(char* buffPtr,unsigned long maxBytes) { return 0; }


// Inherited MAY overwrite this one.
// IF inherited manages a group of subFileBuffs, this is the
// call for you to call saveToFile() on each of them.
bool fileBuff::saveSubFileBuffs(void) { return true; }


// There's always the chance we are no longer useful.
void fileBuff::eraseFromFile(void) { mFile->deleteBlock(mID); }


// This orchestrates the saving of a fileBuff. Some have subfiles and these
// are delt with here as well.
bool fileBuff::saveToFile(void) {
  
  unsigned long numBytes;
  bool          success;

	success = false;
	if (mFile) {
		if (saveSubFileBuffs()) {
			numBytes = calculateBuffSize();
			if (numBytes>0) {
				mBuff = (char*)malloc(numBytes);
				if (mBuff) {
					writeToBuff(mBuff,numBytes);
					success = mFile->writeBlock(mID,mBuff,numBytes);
					free(mBuff);
					mBuff = NULL;
				}
			}
		}
	}
	return success;
}


// Here we're orchestrating the reading of ourselves from long term storage.
// We don't have a call to pull the subBuffs from storage. This is left up
// to the inherited filebuff itself to know that it needs to do that and
// how.
bool fileBuff::readFromFile(void) {
  
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