#include "numStream.h"
#include "resizeBuff.h"

/*
numStreamOut::numStreamOut(int numParams) {  mNumParams = numParams; }


numStreamOut::~numStreamOut(void) {  }


void numStreamOut::writeStream(void) {

   for(int i=0;i<mNumParams;i++) {
      if (i==0) {
         outPort.print(SYNK_CHAR);
      }
      writeVar(i);
      if (i<mNumParams-1) {
         outPort.print(DELEM_CHAR);
      }
   }
   outPort.print(END_CHR);
}


void numStreamOut::writeVar(int index) {  } 
*/


// **************************************  //


numStreamIn::numStreamIn(Stream* inStream,int tokenBuffBytes) {
   
   mStream			= inStream;
   mNumTokenBytes	= 0;
   mTokenBuff		= NULL;
   if (resizeBuff(tokenBuffBytes,&mTokenBuff)) {
      mTokenBuff[0] = '\0';
      mNumTokenBytes = tokenBuffBytes;
   }
   reset();
}


numStreamIn::~numStreamIn(void) { resizeBuff(0,&mTokenBuff); }


void numStreamIn::copyStream(numStreamIn* inNumStream) {

	mStream			= inNumStream->mStream;
   mNumTokenBytes	= 0;
   if (resizeBuff(inNumStream->mNumTokenBytes,&mTokenBuff)) {
      mTokenBuff[0]	= '\0';
      mNumTokenBytes = inNumStream->mNumTokenBytes;
   }
   mSynk			= inNumStream->mSynk;
   mIndex		= inNumStream->mIndex;
}


void numStreamIn::reset(void) {
	 
	mSynk			= false;
   mIndex		= 0;
}


void numStreamIn::readStream(void) {

   char  aChar;
   int   i;

   if (!mTokenBuff) return;                  // If we don't have a token buffer, we can't do this.
   Serial.println("Checking for a char to read.. ");
   if (mStream->available()) {               // Ok, If we have any bytes to read..
      aChar = mStream->read();               // Read one.
      if (!mSynk) {                          // If we're not in synk..
         if (aChar==SYNK_CHAR) {             // If this char is the syk char..
            mSynk = true;                    // We are now in synk.
            mIndex = 0;                      // Just to make sure, we reset the index to zero.
         }                                   // Basically we'll exit now.
      } else {                               // Else, we are in synk..
         if (aChar == DELEM_CHAR) {          // If the char is a delemiter..
            readVar(mIndex++,false);        	// We send index out for buffer decoded. (bump up index)
            mTokenBuff[0] = '\0';            // Reset the token buffer.
         } else if (aChar == END_CHR) {      // If it is the end char..
            readVar(mIndex,true);            // We send index out for buffer decoded. (Don't bump up index)        
            mTokenBuff[0] = '\0';            // Reset the token buffer
            mSynk = false;                   // String of data is complete, de-synk.
         } else {                            // Else, its just a char..
            i = 0;                           // Starting at zero.
            while(mTokenBuff[i] != '\0') {   // We find the end of the token buffer string.
               i++;                          // Bumping along..
            }
            mTokenBuff[i] = aChar;           // Stomp in the char here.
            mTokenBuff[i+1] = '\0';          // Stomp in an end of string in this next char.
         }
      }
   }
}


void numStreamIn::readVar(int index,bool lastField) {  }
