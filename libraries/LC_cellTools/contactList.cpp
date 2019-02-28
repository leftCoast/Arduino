#include "contactList.h"
#include "resizeBuff.h"
#include "PNLabel.h"			// For the filterPN() function.

// **************************************************
// ********************  contact ********************
// **************************************************


// Creation constructor.
contact::contact(blockFile* inFile,unsigned long blockID,unsigned long msgID,char* PN)
  : linkListObj(), fileBuff(inFile,blockID) {          

	mPN				= NULL;
	mNickName		= NULL;
	mFirstName		= NULL;
	mLastName		= NULL;
	mCompanyName	= NULL;
	mMsgID			= msgID;
	mErased			= false;
	setPN(PN);					// This sets the "I've been changed" thing.
}


// Re-creation constructor.
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


contact::~contact(void) {

	if (mPN) free(mPN);
	if (mNickName) free(mNickName);
	if (mFirstName) free(mFirstName);
	if (mLastName) free(mLastName);
	if (mCompanyName) free(mCompanyName);
}

/*
void contact::setPN(char* PN) {

	int numChars;
	char*	temp;

	temp = NULL;												// Initialize..
	numChars = strlen(PN) + 1;								// Enough room.
	if (resizeBuff(numChars,(uint8_t**)&temp)) {		// Grab some RAM
		strcpy(temp,PN);										// Make local copy.
		filterPNStr(temp);									// Filter out the gunk.
		numChars = strlen(temp) + 1;						// Enough room, again.
		if (resizeBuff(numChars,(uint8_t**)&mPN)) {	// Grab some more RAM. We'll keep this chunk.
			strcpy(mPN,temp);									// Copy the number over.
			mChanged = true;									// And flag the change.
		}
		resizeBuff(0,(uint8_t**)&temp);					// Recycle temp;
	}
}
*/

void contact::setPN(char* PN) {

  int numChars;

  numChars = strlen(PN) + 1;
  if (resizeBuff(numChars,(uint8_t**)&mPN)) {
    strcpy(mPN,PN);
    filterPNStr(mPN);
    mChanged = true;
  }
}


void contact::setNickName(char* nickName) {

  int numChars;

  numChars = strlen(nickName) + 1;
  if (resizeBuff(numChars,(uint8_t**)&mNickName)) {
    strcpy(mNickName,nickName);
    mChanged = true;
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


void contact::writeToBuff(char* buffPtr,unsigned long maxBytes) {
  
	int				offset;
	int				numBytes;
	unsigned long*	longPtr;
	int				i;
		
	if (mErased) return;							// We're dead. We ain't playing this game anymore.	
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
	
	//longPtr = (unsigned long*)&(buffPtr[i+offset]);
	longPtr = (unsigned long*)&(buffPtr[offset]);
	*longPtr = mMsgID;
  
	mChanged = false;
}


// We've been FIRED?!?
void contact::eraseFromFile(void) {

	mFile->deleteBlock(mMsgID);
	fileBuff::eraseFromFile();
	mErased = true;
}


unsigned long contact::loadFromBuff(char* buffPtr,unsigned long maxBytes) {

	unsigned long buffIndex;
	unsigned long* longPtr;;
  
  	if (mErased) { return 0; }		// We're dead. We ain't playing this game anymore.	
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
  	mChanged = false;
	return buffIndex;   
}


void contact::printContact(void) {

	Serial.print(F("ID#        : "));Serial.println(mID);
	Serial.print(F("Nickname   : ["));Serial.print(mNickName);Serial.println(F("]"));
	Serial.print(F("Phone      : ["));Serial.print(mPN);Serial.println(F("]"));
	Serial.print(F("First name : ["));Serial.print(mFirstName);Serial.println(F("]"));
	Serial.print(F("Last name  : ["));Serial.print(mLastName);Serial.println(F("]"));
	Serial.print(F("Company    : ["));Serial.print(mCompanyName);Serial.println(F("]"));
	Serial.print(F("Message ID : "));Serial.println(mMsgID);
	Serial.print(F("Erased?    : "));Serial.println(mErased);
	Serial.print(F("BYTES      : "));Serial.println(calculateBuffSize());
	Serial.println(F("-------------------------------------"));Serial.flush();
}


// **************************************************
// *****************  contactList *******************
// **************************************************

          
contactList::contactList(blockFile* inFile)
  : linkList(), fileBuff(inFile) {
    
  readFromFile();   // If we have something, lets get it.
}

  
contactList::~contactList(void) {  }


contact* contactList::findByPN(char* phoneNum) {

  contact*  trace;

  trace = (contact*)getFirst();
  while(trace) {
    if (!strcmp(trace->mPN,phoneNum)) {
      return trace;
    }
    trace = (contact*)trace->next;
  }
  return trace;
}


// This is for creating a fresh new contact. Not re-creation.
contact* contactList::findOrAddContact(char* phoneNum) {

  contact*      contactPtr;
  unsigned long blockID;
  unsigned long msgID;

  contactPtr = findByPN(phoneNum);
  if (!contactPtr) {
    blockID = mFile->getNewBlockID();
    msgID = mFile->getNewBlockID();
    contactPtr = new contact(mFile,blockID,msgID,phoneNum);
    addToTop(contactPtr);
  }
  return contactPtr;
}


unsigned long contactList::calculateBuffSize(void) { return sizeof(unsigned long) * getCount(); }

  
bool contactList::saveSubFileBuffs(void) {

	contact*        trace;

	trace = (contact*)getFirst();       // Grab the first contact on our list.
	while(trace) {                      // While we have contacts..
		if (trace->mChanged) {            // If they have been edited (or new).
			trace->saveToFile();            // Save them to disk.
		}
		trace = (contact*)trace->next;    // Grab another contact. Or a NULL if we ran out.
	}  
	return true;                        // Easy peasy, return true;
}


 
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


void contactList::deleteContact(contact* theDoomed) {
			
	if (theDoomed) {						// Sanity, did they send us someting?
		theDoomed->eraseFromFile();	// Tell the contact to erase itself.
		unlinkObj(theDoomed);			// Now lets pull the node out of the list. (single link list)
		delete(theDoomed);				// Recycle the node.
	}
}