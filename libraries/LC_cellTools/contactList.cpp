#include "contactList.h"
#include "resizeBuff.h"
#include "PNLabel.h"			// For the filterPN() function.

// **************************************************
// ********************  contact ********************
// **************************************************


// Creation constructor. This is for creating a fresh new, never been seen
// before contact. Like you just met someone and they gave you their number.
// Or a SPAM call came in and your phone wants to log it in your call log.
contact::contact(blockFile* inFile,unsigned long blockID,unsigned long msgID,char* PN)
  : linkListObj(), fileBuff(inFile,blockID) {          

	mPN				= NULL;
	mNickName		= NULL;
	mFirstName		= NULL;
	mLastName		= NULL;
	mCompanyName	= NULL;
	mMsgID			= msgID;
	mErased			= false;
	mHaveMSG 		= false;
	setPN(PN);					// This sets the "I've been changed" thing.
}


// Re-creation constructor. This is for pulling an existing contact off the
// SD card. (Disk dive, what have you.)
contact::contact(blockFile* inFile,unsigned long blockID)
  : linkListObj(), fileBuff(inFile,blockID) {

	mPN				= NULL;
	mNickName		= NULL;
	mFirstName		= NULL;
	mLastName		= NULL;
	mCompanyName	= NULL;
	mErased			= false;
	readFromFile();			// This sets the "I've been changed" thing.
}


// Recycle the memory we used.
contact::~contact(void) {

	if (mPN) free(mPN);
	if (mNickName) free(mNickName);
	if (mFirstName) free(mFirstName);
	if (mLastName) free(mLastName);
	if (mCompanyName) free(mCompanyName);
}


// These next few are just for setting strings to differen't fields. The
// phone number string is actually stripped of un-dial-able characters.
void contact::setPN(char* PN) {

  int numChars;

  numChars = strlen(PN) + 1;							// Enough room.
  if (resizeBuff(numChars,(uint8_t**)&mPN)) {	// Grab some RAM
    strcpy(mPN,PN);										// Copy the number over.
    filterPNStr(mPN);									// Filter out the gunk.
    mChanged = true;										// And flag the change.
  }															// No RAM, no change. Bigger troubles coming anyway.
}


void contact::setNickName(char* nickName) {

  int numChars;

  numChars = strlen(nickName) + 1;
  if (resizeBuff(numChars,(uint8_t**)&mNickName)) {
    strcpy(mNickName,nickName);
    mChanged = true;
  }
}


// If we have no nick name, we can set our nick name to a formatted version of our phone
// number.
void contact::setPNNick() {

	PNLabel	numField(1,1,1,1,1);
	int		numBytes;
	char*		buff;
	
	if (!strlen(mNickName)) {									// If we have no nickname.
		numField.setValue(mPN);									// Set the phone number into the formatter.
		numBytes = numField.getNumChars() + 1;				// How big is the result?
		if (resizeBuff(numBytes,(uint8_t**)&buff)) {		// Stretch out the buffer.
			numField.getText(buff);								// Grab the formatted string.
      	setNickName(buff);									// Set out nick name to the formatted phone number.
      	resizeBuff(0,(uint8_t**)&buff);					// Recycle the buffer.
   	 }
	}
}


void contact::setFirstName(char* firstName) {

  int numChars;

  numChars = strlen(firstName) + 1;
  if (resizeBuff(numChars,(uint8_t**)&mFirstName)) {
    strcpy(mFirstName,firstName);
    mChanged = true;
  }
}


void contact::setLastName(char* lastName) {

  int numChars;

  numChars = strlen(lastName) + 1;
  if (resizeBuff(numChars,(uint8_t**)&mLastName)) {
    strcpy(mLastName,lastName);
    mChanged = true;
  }
}


void contact::setCompanyName(char* companyName) {

  int numChars;

  numChars = strlen(companyName) + 1;
  if (resizeBuff(numChars,(uint8_t**)&mCompanyName)) {
    strcpy(mCompanyName,companyName);
    mChanged = true;
  }
}


// How many bytes is our block of text messages?
unsigned long contact::sizeOfMsgBlock(void) { return mFile->getBlockSize(mMsgID); }


// Someone wants the block of text messages. Lets read them out of the file for them.
bool contact::readMsgBlock(char* buff,unsigned long bytes) { return mFile->getBlock(mMsgID,buff,bytes); }

// Here's the updated block of text messages. We need to write them out to the SD drive to
// save them. Now, trying to write a zero number of bytes is actually an error. So, in
// this case, we ignore it and just pass back false.
bool contact::saveMsgBlock(char* buff,unsigned long bytes) {

	if (bytes) {
		mHaveMSG = true;
		return mFile->writeBlock(mMsgID,buff,bytes);
	}	
	else return false;
}			


// We're going to toss out our message block. As of this writing, you can still reuse the
// file ID. So we just leave it and reuse it later if we like.
void contact::deleteMsgBlock(void) { mFile->deleteBlock(mMsgID); }


// We are being handed a c-string with a new message to add to the message block.				
void  contact::addMsg(char* strBuff,bool us) {
	
	unsigned long 	buffBytes;
	unsigned long 	newBuffBytes;
	unsigned long 	numChars;
	char*				fileBuff;
	
	numChars = strlen(strBuff);												// How many chars is this new message?
	if (numChars>0) {																// Lets make sure we have some bytes to add. 
		buffBytes = mFile->getBlockSize(mMsgID);							// Lets see if we can grab the file block size.
		newBuffBytes = buffBytes + numChars + 2;							// The new buff size, old buff, chars, leading flag & ending null.
		fileBuff = NULL;															// Init the file buff pointer.
		if (resizeBuff(newBuffBytes,(uint8_t**)&fileBuff)) {			// Allocate the block buffer. Enough for everything.
			mFile->getBlock(mMsgID,fileBuff,newBuffBytes);				// Grab the block out of the file. empty block throws error, ignore error.
			if (us) {																// Pop in the leading flag. Us or them?
				fileBuff[buffBytes] = 'U';
			} else {
				fileBuff[buffBytes] = 'T';										// Show that this is "theirs".
			}
			strcpy(&(fileBuff[buffBytes+1]),strBuff);						// Stuff in the new message.
			mFile->writeBlock(mMsgID,fileBuff,newBuffBytes);			// Write out the updated buffer to the file.
			resizeBuff(0,(uint8_t**)&fileBuff);								// Recycle the buffer.
		}
	}
}

			
// We are being asked how much of a buffer will we need to save ourselves to
// a block of bytes. Currently its the length of all the strings strung
// together and 4 bytes for our message list file ID.
unsigned long contact::calculateBuffSize(void) {

  int numBytes;

	if (mErased) return 0;						// We're dead. We ain't playing this game anymore.							
	numBytes =  strlen(mPN) + 1 +
				  strlen(mNickName) + 1 +
				  strlen(mFirstName) + 1 +
				  strlen(mLastName) + 1 +
				  strlen(mCompanyName) + 1;
	numBytes = numBytes + sizeof(mMsgID);
	return numBytes;
}


// Write ourselves to a buffer in such a way that, given this buffer again
// we will be able to re-create ourselves. Currently its a list of c strings
// with the terminating '\0's. Then a file ID for our text message list.
void contact::writeToBuff(char* buffPtr,unsigned long maxBytes) {
  
	int				offset;
	int				numBytes;
	unsigned long*	longPtr;
	int				i;
		
	if (mErased) return;							// I said, we're dead. You not listening?	
	offset = 0;                       
	numBytes = strlen(mPN) + 1;             // Write out phone number.
	for(i=0;i<numBytes;i++) {
		buffPtr[i] = mPN[i];
	}
	offset = offset + i;

	numBytes = strlen(mNickName) + 1;       // Write out nick name. 
	for(i=0;i<numBytes;i++) {
		buffPtr[i+offset] = mNickName[i];
	}
	offset = offset + i;

	numBytes = strlen(mFirstName) + 1;       // First name.
	for(i=0;i<numBytes;i++) {
		buffPtr[i+offset] = mFirstName[i];
	}
	offset = offset + i;

	numBytes = strlen(mLastName) + 1;       // Last name.
	for(i=0;i<numBytes;i++) {
		buffPtr[i+offset] = mLastName[i];
	}
	offset = offset + i;

	numBytes = strlen(mCompanyName) + 1;    // Company name.
	for(i=0;i<numBytes;i++) {
		buffPtr[i+offset] = mCompanyName[i];
	}
	offset = offset + i;
	
	longPtr = (unsigned long*)&(buffPtr[offset]);	// Our message list file ID.
	*longPtr = mMsgID;
  
	mChanged = false;
}


// We've been FIRED?!?
void contact::eraseFromFile(void) {

	mFile->deleteBlock(mMsgID);	// This deletes our list of text messages.
	fileBuff::eraseFromFile();		// This deletes ourselves from the file.
	mErased = true;					// This marks us as deleted for the rest of this code.
}


// Remember I said we need to be able to re-create ourselves from a byte
// buffer? Here's that buffer. Currently we pull the strings out, one after
// another, then grab our message list file ID off the end.
unsigned long contact::loadFromBuff(char* buffPtr,unsigned long maxBytes) {

	unsigned long buffIndex;
	unsigned long* longPtr;;
  
  	if (mErased) { return 0; }		// Again, we're dead. Not doing this.	
	buffIndex = 0;
	setPN(&(buffPtr[buffIndex]));
	buffIndex = buffIndex + strlen(mPN) + 1;
	setNickName(&(buffPtr[buffIndex]));
	buffIndex = buffIndex + strlen(mNickName) + 1;
	setFirstName(&(buffPtr[buffIndex]));
	buffIndex = buffIndex + strlen(mFirstName) + 1;
	setLastName(&(buffPtr[buffIndex]));
	buffIndex = buffIndex + strlen(mLastName) + 1;
	setCompanyName(&(buffPtr[buffIndex]));
	buffIndex = buffIndex + strlen(mCompanyName) + 1;
	longPtr = (unsigned long*)&(buffPtr[buffIndex]);
	mMsgID = *longPtr;
	buffIndex = buffIndex + sizeof(mMsgID);
	mHaveMSG  = sizeOfMsgBlock()>0;
  	mChanged = false;
	return buffIndex;   
}


// Prints to the serailport, for debugging.
void contact::printContact(void) {

	Serial.print(F("ID#        : "));Serial.println(mID);
	Serial.print(F("Nickname   : ["));Serial.print(mNickName);Serial.println(F("]"));
	Serial.print(F("Phone      : ["));Serial.print(mPN);Serial.println(F("]"));
	Serial.print(F("First name : ["));Serial.print(mFirstName);Serial.println(F("]"));
	Serial.print(F("Last name  : ["));Serial.print(mLastName);Serial.println(F("]"));
	Serial.print(F("Company    : ["));Serial.print(mCompanyName);Serial.println(F("]"));
	Serial.print(F("Msg ID  	: "));Serial.println(mMsgID);
	Serial.print(F("Erased?    : "));Serial.println(mErased);
	Serial.print(F("BYTES      : "));Serial.println(calculateBuffSize());
	Serial.println(F("-------------------------------------"));Serial.flush();
}



// **************************************************
// *****************  contactList *******************
// **************************************************


// Deep inside we are only a fileBuff object that lives in a blockFile. In
// our RAM state we are a linkListObj manager.           
contactList::contactList(blockFile* inFile)
  : linkList(), fileBuff(inFile) {
    
  readFromFile();   // If we have something, lets get it.
}

  
contactList::~contactList(void) {  }


// Given a phone number this'll find the contact object from the list. If it
// can't find one, it returns NULL as a flag.
contact* contactList::findByPN(char* phoneNum) {

	contact*	trace;
	int		numChars;
	char*		strBuff;
	
	strBuff = NULL;													// Init the string buffer.
	numChars = strlen(phoneNum);									// Count the chars.
	if (numChars>0) {													// Make sure we actually got something to work with.
		if (resizeBuff(numChars+1,(uint8_t**)&strBuff)) {	// Allocate a buffer. (We can't accidentally pass in a constant, this avoids that.)
			strcpy(strBuff,phoneNum);								// Make the dynamic copy of the phone number.
			filterPNStr(strBuff);									// Now, rip the junk from the number.
			if (strlen(strBuff)>0) {								// Make sure we have anything left to work with.
				trace = (contact*)getFirst();						// Set up our standard tracer.
				while(trace) {											// While we ain't sitting on a NULL.
					if (!strcmp(trace->mPN,strBuff)) {			// Do the compare. If we got it?
						resizeBuff(0,(uint8_t**)&strBuff);		// Recycle the string buffer.
						return trace;									// Return the contact pointer.
					}
					trace = (contact*)trace->next;				// Or we just move on. Nothing to see here.
				}
			}
			resizeBuff(0,(uint8_t**)&strBuff);					// Recycle the string buffer.
		}
	}
	return NULL;														// If we've arrived here, we couldn't find anything.
}


// This is for creating a fresh new contact. First it looks to see if it
// can find this phone number in the list. If so, it'll just pass this
// back. (Does not make duplicats. NO Duplucates!) Otherwise it will
// create a new contact with this phone number and pass that back.
contact* contactList::findOrAddContact(char* phoneNum) {

	contact*      contactPtr;
	unsigned long blockID;
	unsigned long msgID;

	contactPtr = findByPN(phoneNum);
	if (!contactPtr && strlen(phoneNum)>0) {							// If it wasn't there and we actually have some sort of phone number..
		blockID = mFile->getNewBlockID();								// Grab a new issued block ID.
		msgID = mFile->getNewBlockID();									// And another block ID for a message block.
		contactPtr = new contact(mFile,blockID,msgID,phoneNum);	// Put it all together to create a new contact.
		addToTop(contactPtr);												// Add this new guy to the list.
	}
  	return contactPtr;														// return our result. NULL for complete failure.
}


// How much of a RAM buffer will we need to save ourselves? Well, for
// ourselves, we only save a list of contact file IDs. So its just
// ID size * count.
unsigned long contactList::calculateBuffSize(void) { return sizeof(unsigned long) * getCount(); }


// This is an inherited call we fill out. We have a list of objects we need
// to save. I our case its the changed contact objects. This is where we do
// the "thing" to save them. Basically write them out into a buffer to ship
// off to the SD card. We don't worry about order or not saving unchanged
// ones. Its all sorted out by the contact file IDs. Pretty slick actually.
bool contactList::saveSubFileBuffs(void) {

	contact*        trace;

	trace = (contact*)getFirst();			// Grab the first contact on our list.
	while(trace) {								// While we have contacts..
		if (trace->mChanged) {				// If they have been edited (or new).
			trace->saveToFile();				// Save them to disk.
		}
		trace = (contact*)trace->next;	// Grab another contact. Or a NULL if we ran out.
	}  
	return true;								// Easy peasy, return true;
}


// Another inherited call. What do we save as the list manager? We only save
// a list of contact IDs. Its enough to do our part in recreating this data
// structure in RAM next time we need it. The contacts we manage will be
// saved in the inherited saveSubFileBuffs() method above.
void contactList::writeToBuff(char* buffPtr,unsigned long maxBytes) {

	contact*        trace;
	int             index;
	unsigned long*  longPtr;

	index = 0;                          // Starting at zero, usually a good plan.
	longPtr = (unsigned long*)buffPtr;  // If we are a pointer to X, we index by X. So they say.
	trace = (contact*)getFirst();       // Grab the first contact on our list.
	while(trace) {                      // While we have contacts..
		longPtr[index] = trace->mID;      // Stuff their little IDs into the buffer. At point "index".
		index++;                          // Bump up index by one.
		trace = (contact*)trace->next;    // Grab another contact. Or a NULL if we ran out.
	}
	  
}


// This is where we're handed a buffer and we recreate our database of
// contacts in RAM. Now remember, our buffer is only a list of file ID
// numbers. So we read out a number, use that to create a contact, add it to
// our list then read the next.
unsigned long contactList::loadFromBuff(char* buffPtr,unsigned long maxBytes) {

  unsigned long*  longPtr;
  int             numIDs;
  contact*        newContact;

  longPtr = (unsigned long*)buffPtr;                  // If we are a pointer to X, we index by X. So they say
  numIDs = maxBytes / sizeof(unsigned long);          // Figure out how many are in there.
  for (int i=0;i<numIDs;i++) {                        // Loop through them all..
    newContact = new contact(mFile,longPtr[i]);  		// Create contact node from the file.
    addToTop(newContact);                             // Add 'em to our list.
  }
  return numIDs * sizeof(unsigned long);              // That's how much we burned up.
}


// We manage the list of contacts in RAM. This on has been marked for
// deletion. We need to do whatever it takes to delete it.
void contactList::deleteContact(contact* theDoomed) {
			
	if (theDoomed) {						// Sanity, did they send us someting?
		theDoomed->eraseFromFile();	// Tell the contact to erase itself.
		unlinkObj(theDoomed);			// Now lets pull the node out of the list. (single link list)
		delete(theDoomed);				// Recycle the node.
	}
}


// We have incoming raw text message coming in from the outside world. Save it to the
// contact.
void contactList::addMessage(char* PN, char* msg) {

	contact*	aContact;
	
	if (strlen(PN)>0 && strlen(msg)>0) {	// Lets just be sure we got something to work with.
		aContact = findOrAddContact(PN);		// Using the phone number, whip up a contact. Possibly a new one?
		aContact->setPNNick();					// If its a new one, set the nick name to the phone number.
		aContact->addMsg(msg,false);			// Add the message to the contact.
	}
}


