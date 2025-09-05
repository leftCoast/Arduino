#include <numStream.h>
#include <resizeBuff.h>
#include <timeObj.h>



uint8_t		rx_buffer[255-64];	// On teensy you can add memory to the incoming serial message buffer.


numStreamIn::numStreamIn(Stream* inStream,int tokenBuffBytes) {
   
   ourPort		= inStream;
   spew			= true;
   tokenBytes	= 0;
   tokenBuff	= NULL;
	if (resizeBuff(tokenBuffBytes,&tokenBuff)) {
		tokenBuff[0] = '\0';
      tokenBytes = tokenBuffBytes;
   }
   msgBufBytes = 0;
   msgBuf		= NULL;
   if (resizeBuff(MAX_MSG_BYTES,&msgBuf)) {
   	msgBufBytes = MAX_MSG_BYTES;
   }
   Serial1.addMemoryForRead(rx_buffer, sizeof(rx_buffer));
   reset();
}


numStreamIn::~numStreamIn(void) {

	resizeBuff(0,&tokenBuff);
	
}


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


bool numStreamIn::checkTheSum(void) {

	int	i;
	byte	chkSumVal;
	char	sumStr[4];
	byte	sumStrVal;
	
	if (strchr(msgBuf,'*')) {								// If we find an astrick..
		i=0;														// We start at first byte.
		chkSumVal = 0;											// Zero out our check sum.
		while(msgBuf[i]!='*') {								// For every byte 'till the astrick.
			chkSumVal ^= msgBuf[i];							// XOR the incoming char.
			i++;													// bump index.
		}															//
		i++;														// bump index past astrick.
		sumStr[0] = msgBuf[i++];							// Grab first char after.
		sumStr[1] = msgBuf[i];								// Second char.
		sumStr[2] = '\0';										// Set in the null char.
		sumStrVal = (byte)strtol(sumStr, NULL, 16);	// Decode these two bytes as hex text.
		return chkSumVal==sumStrVal;						// Return if they match.
	}																//
	return false;												// No astrick!	
}


// True we send every char we see out the serial port. False, we don't.
void numStreamIn::setSpew(bool onOff) { spew = onOff; }


// Spin though the data looking for the synk char.
exitStates numStreamIn::findSynkChar(void) {
	
	timeObj	timoutTimer(MAX_MS);
	char		aChar;
	
	while(!timoutTimer.ding()) {				// Looping for a time..
		if (ourPort->available()) {			// If there are bytes to be had..
			aChar = ourPort->read();			// We grab one..
			if (spew) Serial.print(aChar);	// If spewing, lets see it.
			if (aChar==SYNK_CHAR) {				// If it's the synk char..
				msgIndex = 0;						// Setup to read a message.
				msgBuf[0] = '\0';					// Used to compare checksums.
				return(completed);				// Return that we found it!
			}											// 
		} else {										// Else, we ran out of data..
			return(noData);						// Return we ran out of data.	
		}												//
	}
	errMsg(synkTimOut);
	return erroredOut;							// Timed out? Just toss an error and reset.
}


// We're (think) we are sitting at the start or middle of a data string. Grab a parameter!
exitStates numStreamIn::readToken(void) {

	timeObj	timoutTimer(MAX_MS);
	char		aChar;
	
	while(!timoutTimer.ding()) {						// Looping for a time..
		if (ourPort->available()) {					// If there are bytes to be had..
			aChar = ourPort->read();					// We grab one..
			if (spew) Serial.print(aChar);			// If spewing, lets see it.									
			if (msgIndex<msgBufBytes-2) {				// If we have enough room..
				msgBuf[msgIndex] = aChar;				// Add the char to the checksum buffer.
				msgIndex++;									// Bump up the char index.
				msgBuf[msgIndex] = '\0';				//	Add a null afer it.
			} else {											// Else we ran out of room!
				errMsg(msgBufOverflow);					// Ran out of room.
				return erroredOut;						// So, error out for a reset.
			}													//
			if (dataChar(aChar)) {						// If it's a data char?
				if (tokenIndex<tokenBytes-2) {		// If we have enough room for it.
					tokenBuff[tokenIndex] = aChar;	// Stuff in the char.
					tokenIndex++;							// Bump up the char index.
					tokenBuff[tokenIndex] = '\0';		// Keep a running end-of-string thing going.
				} else {										// Else we ran out of room!
					errMsg(tokenOverflow);				// Ran out of room.
					return erroredOut;					// We error out for a reset.
				}												//
			} else if (aChar==DELEM_CHAR) {			// Got the delimiter char.
				return completed;							// We return that got a complete parameter.
			} else if (aChar==END_CHAR) {				// We got the end of set char.
				if (checkTheSum()) {						// If the checksum matches.
					return finalValue;					// Return success. But last value.
				} else {										// else the checksum failed.
					errMsg(badChecksum);					// Tell 'em checksum failed.
					return erroredOut;					// Toss an error and reset.
				}												
			} else {											// Anything else? Who knows at this point?
				errMsg(unknownErr);						// No idea what went wrong.
				return erroredOut;						// Just toss an error and reset.			
			}													//
		} else {												// Else, we have no data to read.
			return noData;									// No problem, be back later.
		}														//
	}															// 
	errMsg(tokenTimOut);
	return erroredOut;									// If we time out we end up here. Toss error and give up.
}
			


// This is what we do to earn our living. Sip on a stream and parcel out tokens.
void numStreamIn::idle(void) {
   
   switch(ourState) {													// What are we up to?
   	case lookForSynk	:												// We're looking for the synk char.
   		switch(findSynkChar()) {									// Why did we stop looking for a synk char?
   			case noData 		:	break;							// No problem, we'll come back later.		
   			case completed		:										// We had success. And there's more to come.
   				synk = true;											// We are now in synk!
            	paramIndex = 0;										// Just to make sure, we reset the paramIndex to zero.
            	tokenBuff[0] = '\0';									// Clear out the token buffer.
            	tokenIndex = 0;										//	Set where next char goes in the token string.
            	ourState = readingType;								// Next task is to read out the type of data we are.
            break;														//
   			case finalValue	:	break;							// Found terminator. Keep going
   			case erroredOut	:	break;							// Can we actually have an error here? Keep running.
         }																	//
   	break;																//
   	case readingType	:												// We are reading out the initial data type text.
   		switch(readToken()) {										// We'll read a token from the stream..
   			case noData 		:	break;							// No data? Again, no problem. We'll swing back later.
   			case completed		:										// Success with more to follow.
   				if (canHandle(tokenBuff)) {						// If we can handle this type of data..
						paramIndex++;										// Bump up the paramIndex.
						tokenBuff[0] = '\0';								// Clear out the tolken buffer.
						tokenIndex = 0;									//	Set where next char goes in the token string.
						ourState = readingParam;						// We slide into reading params.
					} else {													// else we can't handle that type?
						errMsg(noHandler);								// If debugging, toss a message.
						reset();												// reset for synk again.
					}															//
            break;														//
            case finalValue	:										// Well here is a conundrum. Good read but no data.
            case erroredOut	:										// We'll toss it into the error pot.
   				reset();													// reset to start over again.
   			break;														//
         }																	//
      break;																//
      case readingParam	:												// We are busy reading out a param..
   		switch(readToken()) {										// Why'd we exit?
   			case noData 		:	break;							// Ran out of data, it happens.
   			case completed		:										// Got a parameter!
   				if (addValue(tokenBuff,paramIndex,false)) {	// If we can add this data value... (There's more!)
						paramIndex++;										// Bump up the paramIndex.
						tokenBuff[0] = '\0';								// Clear out the tolken buffer.
						tokenIndex = 0;									//	Set where next char goes in the token string.
					} else {													// Else something went horribly wrong..
						errMsg(addValueFail);							// If debugging, toss a message.
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


void numStreamIn::errMsg(errType inErr) {

#ifdef SHOW_DATA																// Are we allowing debugging data today?
	switch(inErr) {															// Choose the message..
		case unknownErr		:
			Serial.println("***** Unknown error?");
		break;
		case synkTimOut		:													
			Serial.println("***** findSynkChar() timed out.");	
		break;
		case tokenTimOut		:													
			Serial.println("***** readToken() timed out.");	
		break;																	
   	case noHandler			:
   		Serial.print("***** Cannot find handler for [");
			Serial.print(tokenBuff);
			Serial.println("]");
		break;
		case addValueFail		:
   		Serial.print("***** addValue() failed for [");
			Serial.print(tokenBuff);
			Serial.println("]");
		break;
		case badChecksum		:
			Serial.print("***** checkTheSum() failed for [");
			Serial.print(msgBuf);
			Serial.println("]");
		break;
		case tokenOverflow	:
			Serial.println("***** tokenBuff overflow.");	
		break;
		case msgBufOverflow	:
			Serial.println("***** msgBuf overflow.");	
		break;
		default					:
			Serial.println("***** Some new kind of error we don't know about yet?");
		break;	
	}
#endif
}
   
   
   
   
   