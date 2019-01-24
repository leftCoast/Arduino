#include "IDnameBuff.h"

// Two ways to be created. Someone says, "lets make a new one called.. " And resurection from stasis.

// This is our "call into exsistance" constructor. Someone said, make a new one, with these things..
IDnameBuff::IDnameBuff(blockFile* inFile,unsigned long blockID,char* inName) {
    : fileBuff(inFile,blockID) {
      
  mName = NULL;
  mNumBytes = 0;
  mBuff = NULL;
  setName(inName);
}


IDnameBuff::IDnameBuff(blockFile* inFile,unsigned long blockID,char* inName,unsigned long numBytes,uint8_t* inBuff)
  : fileBuff(inFile,blockID) {

  mName = NULL;
  mNumBytes = 0;
  mBuff = NULL;
  setName(inName);
  setBuff(numBytes,inBuff);
}


// Umm.. This gives a blank, I'm assuming the next call would be loadFromBuff() from the calling entity.
IDnameBuff::IDnameBuff(blockFile* inFile,unsigned long blockID)
  : fileBuff(inFile,blockID) {

  mName = NULL;
  mNumBytes = 0;
  mBuff = NULL;
  
}


IDnameBuff::~IDnameBuff(void) {

  resizeName(0);
  resizeBuff(0);
}


unsigned long IDnameBuff::calculateBuffSize(void) { return strlen(mName)+1+mNumBytes; }


void IDnameBuff::writeToBuff(char* buffPtr,unsigned long maxBytes) {

  int offset;
  
  if (maxBytes>=calculateBuffSize()) {          // If we have enough room.
    strcpy(buffPtr,mName);                      // stuff in name with trailing \0.
    offset = strlen(mName)+1;                   // Offset to the first data byte.
    for(unsigned long  i=0;i<mNumBytes;i++) {   // Here's your databytes.
      buffPtr[offset+i] = mBuff[i];             // Stuff 'em in.
    }
  }
}


unsigned long IDnameBuff::loadFromBuff(char* buffPtr,unsigned long maxBytes) {

  int nameLen;
  
  nameLen = strlen(buffPtr)+1;                          // First thing in buff is a c string. Measure it.
  resizeName(nameLen);                                  // Ask for enough RAM to store it.
  if (mName) {                                       // If we got it..
    strcpy(mName,buffPtr);                           // Save off the name.
    mNumBytes = maxBytes - nameLen;                     // What's left over are the data bytes.
    resizeRecBuff(mNumBytes);                           // Ask for RAM to store the info.
    if (mBuff) {                                     // If we got the RAM.
      for(unsigned long i=nameLen;i<maxBytes;i++) {     // Grab the rest of the bytes;
        mBuff[i-nameLen] = buffPtr[i];               // Stuff in the bytes.
      }
    }
  }
  return maxBytes;                                      // Well, we grabbed all of them.
}


void IDnameBuff::setName(char* recName) {

  resizeName(strlen(recName)+1);
  if (mName)  {
    strcpy(mName,recName);
  }
}


char* IDnameBuff::getName(void) { return mName; }


void IDnameBuff::setRecBuff(unsigned long numBytes,uint8_t* recBuff) {

  resizeRecBuff(numBytes);
  if (mBuff) {
    mNumBytes = numBytes;
    for(unsigned long i=0;i<mNumBytes;i++) {
      mBuff[i]=recBuff[i];
    }
  }
}


bool IDnameBuff::resizeName(int numBytes) {

  if (mName) {
    free(mName);
    mName = NULL;
  }
  if (numBytes) {
    mName = (char*)malloc(numBytes);
    return(mName!=NULL);
  }
  return true;
}


bool IDnameBuff::resizeRecBuff(unsigned long numBytes) {

  if (mBuff) {
    free(mBuff);
    mBuff = NULL;
  }
  if (numBytes) {
    mBuff = (uint8_t*)malloc(numBytes);
    return(mBuff!=NULL);
  }
  return true;
}
