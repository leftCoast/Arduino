#include <numStream.h>
#include <resizeBuff.h>
#include <timeObj.h>

numStreamIn::numStreamIn(Stream* inStream,int tokenBuffBytes)
	: extSerial(inStream) {
   
   tokenBytes	= 0;
   tokenBuff	= NULL;
	if (resizeBuff(tokenBuffBytes,&tokenBuff)) {
		tokenBuff[0] = '\0';
      tokenBytes = tokenBuffBytes;
   }
   reset();
}


numStreamIn::~numStreamIn(void) { resizeBuff(0,&tokenBuff); }


void numStreamIn::begin(void) {

	if (tokenBuff) {	// If we were able to allocate our token buffer..
		hookup();		// We can fire up the machine.
	}						//
}


void numStreamIn::reset(void) {
	 
	synk			= false;
   tokenIndex 	= 0;
   paramIndex	= 0;
   ourState 	= lookForSynk;
}


// Default is no we can't. Override to handle different data sets.
bool numStreamIn::canHandle(const char* inType) { return false; }


// Override to handle different data sets.
bool numStreamIn::addValue(char* param,int paramIndex,bool isLast) { return false; }


// If it's nothing special..
bool numStreamIn::dataChar(char inChar) {

	if (inChar=='\0') return false;
	if (inChar==SYNK_CHAR) return false;
	if (inChar==DELEM_CHAR) return false;
	if (inChar==END_CHAR) return false;
	return true;
}


// Spin though the data looking for the synk char.
exitStates numStreamIn::findSynkChar(void) {
	
	timeObj	timoutTimer(MAX_MS);
	char		aChar;
	
	while(!timoutTimer.ding()) {					// Looping for a time..
		if (available()) {							// If there are bytes to be had..
			aChar = read();							// We grab one..
			if (aChar==SYNK_CHAR) {					// It its the synk char..
#ifdef SHOW_DATA	
				Serial.println("Got SYNK");
#endif
				return(completed);					// Return that we found it!
			}												// 
		} else {											// Else, we ran out of data..
			//Serial.println("findSynkChar - noData");
			return(noData);							// Return we ran out of data.	
		}												//
	}
#ifdef SHOW_DATA														//
	Serial.println("**** findSynkChar() timed out. ****");
#endif
	return erroredOut;								// Timed out? Just toss an error and reset.
}


// We're (think) we are sitting at the start of a data string. Grab it!
exitStates numStreamIn::readToken(void) {

	timeObj	timoutTimer(MAX_MS);
	char		aChar;
	
	while(!timoutTimer.ding()) {						// Looping for a time..
		if (available()) {								// If there are bytes to be had..
			aChar = read();								// We grab one..
			if (dataChar(aChar)) {						// If it's a data char?
				if (tokenIndex<tokenBytes-2) {		// If we have enough room for it.
					tokenBuff[tokenIndex] = aChar;	// Stuff in the char.
					tokenIndex++;							// Bump up the char index.
					tokenBuff[tokenIndex] = '\0';		// Keep a running end-of-string thing going.
				} else {										// Else we ran out of room!
					return erroredOut;					// We error out for a reset.
				}												//
			} else if (aChar==DELEM_CHAR) {			// Got the delimiter char.
#ifdef SHOW_DATA
				Serial.print("**** readToken()[");
				Serial.print(tokenBuff);
				Serial.println("]****");
#endif
				return completed;							// We return that got a complete parameter.
			} else if (aChar==END_CHAR) {				// We got the end of set char.
#ifdef SHOW_DATA
				Serial.print("**** readToken()[");
				Serial.print(tokenBuff);
				Serial.println("] FINAL ****");
#endif
				return finalValue;						// Return success. But last value.
			} else {											// Anything else?
#ifdef SHOW_DATA
				Serial.print("**** messed up token() **** Got : ");
				if (aChar=='\0') {
					Serial.println("NULL Char");
				} else if (aChar==SYNK_CHAR) {
					Serial.println("SYNK_CHAR");
				} else {
					Serial.println(aChar);
				}
#endif
				return erroredOut;						// Just toss an error and reset.			
			}													//
		} else {												// Else we have no data to read.
			//Serial.println("readToken - noData");
			return noData;									// No problem, be back later.
		}														//
	}
#ifdef SHOW_DATA															//
	Serial.println("**** readToken() timed out. ****");
#endif
	return erroredOut;									// Shouldn't be here. Just toss an error and reset.
}
			


// This is what we do to earn our living. Sip on a stream and parcel out tokens.
void numStreamIn::idle(void) {
   
   extSerial::idle();											// First we let the old folks go. Respect and all.
   if (!tokenBuff) return;										// If we don't have a token buffer, we can't do this.
   switch(ourState) {											// What are we up to?
   	case lookForSynk	:										// We're looking for the synk char.
   		switch(findSynkChar()) {							// Why did we stop looking for a synk char?
   			case noData 		:	break;					// No problem, we'll come back later.		
   			case completed		:								// We had success. And there's more to come.
   				synk = true;									// We are now in synk!
            	paramIndex = 0;								// Just to make sure, we reset the paramIndex to zero.
            	tokenBuff[0] = '\0';							// Clear out the token buffer.
            	tokenIndex = 0;								//	Set where next char goes in the token string.
            	ourState = readingType;						// Next task is to read out the type of data we are.
            break;												//
   			case finalValue	:	break;					// Found terminator. Keep going
   			case erroredOut	:	break;					// Can we actually have an error here? Keep running.
         }															//
   	break;														//
   	case readingType	:										// We are reading out the initial data type text.
   		switch(readToken()) {								// We'll read a token from the stream..
   			case noData 		:	break;					// No data? Again, no problem. We'll swing back later.
   			case completed		:								// Success with more to follow.
   				if (canHandle(tokenBuff)) {				// If we can handle this type of data..
						paramIndex++;								// Bump up the paramIndex.
						tokenBuff[0] = '\0';						// Clear out the tolken buffer.
						tokenIndex = 0;							//	Set where next char goes in the token string.
						ourState = readingParam;				// We slide into reading params.
					} else {											// else we can't handle that type?
						reset();										// reset for synk again.
					}													//
            break;												//
            case finalValue	:								// Well here is a conundrum. Good read but no data.
            case erroredOut	:								// We'll toss it into the error pot.
   				reset();											// reset to start over again.
   			break;												//
         }															//
      break;														//
      case readingParam	:										// We are busy reading out a param..
   		switch(readToken()) {								// Why'd we exit?
   			case noData 		:	break;					// Ran out of data, it happens.
   			case completed		:										// Got a parameter!
   				if (addValue(tokenBuff,paramIndex,false)) {	// If we can add this data value... (There's more!)
						paramIndex++;										// Bump up the paramIndex.
						tokenBuff[0] = '\0';								// Clear out the tolken buffer.
						tokenIndex = 0;									//	Set where next char goes in the token string.
					} else {													// Else something went horribly wrong..
						reset();												// We reset and look for the next one.
					}															//
            break;														//
   			case finalValue	:										// Got a value AND it's the last of this set.
   				addValue(tokenBuff,paramIndex,true);			// Here's a data value for you! (The last!)
					reset();													// No matter if it failed or not, we're looking for synk again.
            break;														//
   			case erroredOut	:										// Had an error..
   				reset();													// Again, reset for the next set.
   			break;														//
         }																	//
   	break;																//
   }																			//
}

   		
   
   
   
   
   
   
   