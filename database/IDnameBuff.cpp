#include "IDnameBuff.h"

IDnameBuff::IDnameBuff(blockFile* inFile,unsigned long blockID,char* recName,unsigned long numBytes,uint8_t* recBuff)
  : fileBuff(inFile,blockID) {

  mRecName = NULL;
  mNumBytes = 0;
  mRecBuff = NULL;
  setName(recName);
  setRecBuff(numBytes,recBuff);
}


IDnameBuff::IDnameBuff(blockFile* inFile,unsigned long blockID)
  : fileBuff(inFile,blockID) {

  mRecName = NULL;
  mNumBytes = 0;
  mRecBuff = NULL;
}


IDnameBuff::~IDnameBuff(void) {

  resizeName(0);
  resizeRecBuff(0);
}


unsigned long IDnameBuff::calculateBuffSize(void) { return strlen(mRecName)+1+mNumBytes; }


void IDnameBuff::writeToBuff(char* buffPtr,unsigned long maxBytes) {

  int offset;
  
  if (maxBytes>=calculateBuffSize()) {            // If we have enough room.
    strcpy(buffPtr,mRecName);                   // stuff in name with trailing \0.
    offset = strlen(mRecName)+1;                // Offset to the first data byte.
    for(unsigned long  i=0;i<mNumBytes;i++) {   // Here's your databytes.
      buffPtr[offset+i] = mRecBuff[i];          // Stuff 'em in.
    }
  }
}


unsigned long IDnameBuff::loadFromBuff(char* buffPtr,unsigned long maxBytes) {

  int nameLen;
  
  nameLen = strlen(buffPtr)+1;                          // First thing in buff is a c string. Measure it.
  resizeName(nameLen);                                  // Ask for enough RAM to store it.
  if (mRecName) {                                       // If we got it..
    strcpy(mRecName,buffPtr);                           // Save off the name.
    mNumBytes = maxBytes - nameLen;                     // What's left over are the data bytes.
    resizeRecBuff(mNumBytes);                           // Ask for RAM to store the info.
    if (mRecBuff) {                                     // If we got the RAM.
      for(unsigned long i=nameLen;i<maxBytes;i++) {     // Grab the rest of the bytes;
        mRecBuff[i-nameLen] = buffPtr[i];               // Stuff in the bytes.
      }
    }
  }
  return maxBytes;                                      // Well, we grabbed all of them.
}


void IDnameBuff::setName(char* recName) {

  resizeName(strlen(recName)+1);
  if (mRecName)  {
    strcpy(mRecName,recName);
  }
}


char* IDnameBuff::getName(void) { return mRecName; }


void IDnameBuff::setRecBuff(unsigned long numBytes,uint8_t* recBuff) {

  resizeRecBuff(numBytes);
  if (mRecBuff) {
    mNumBytes = numBytes;
    for(unsigned long i=0;i<mNumBytes;i++) {
      mRecBuff[i]=recBuff[i];
    }
  }
}


bool IDnameBuff::resizeName(int numBytes) {

  if (mRecName) {
    free(mRecName);
    mRecName = NULL;
  }
  if (numBytes) {
    mRecName = (char*)malloc(numBytes);
    return(mRecName!=NULL);
  }
  return true;
}


bool IDnameBuff::resizeRecBuff(unsigned long numBytes) {

  if (mRecBuff) {
    free(mRecBuff);
    mRecBuff = NULL;
  }
  if (numBytes) {
    mRecBuff = (uint8_t*)malloc(numBytes);
    return(mRecBuff!=NULL);
  }
  return true;
}
