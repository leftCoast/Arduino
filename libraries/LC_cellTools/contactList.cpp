#include "contactList.h"
#include "resizeBuff.h"



// **************************************************
// ********************  contact ********************
// **************************************************


// Creation constructor.
contact::contact(blockFile* inFile,unsigned long blockID,char* PN)
  : linkListObj(), fileBuff(inFile,blockID) {          

	mPN				= NULL;
	mNickName		= NULL;
	mFirstName		= NULL;
	mLastName		= NULL;
	mCompanyName	= NULL;
	mTextList		= NULL;
	mShortBuff		= false;
	mTextBytes		= 0;
	setPN(PN);
	mChanged			= true;
}


// Re-creation constructor.
contact::contact(blockFile* inFile,unsigned long blockID,bool shortBuff)
  : linkListObj(), fileBuff(inFile,blockID) {

	mPN				= NULL;
	mNickName		= NULL;
	mFirstName		= NULL;
	mLastName		= NULL;
	mCompanyName	= NULL;
	mTextList		= NULL;
	mShortBuff		= shortBuff;
	mTextBytes		= 0;
	readFromFile();
	mChanged			= false;
}


contact::~contact(void) {

	if (mPN) free(mPN);
	if (mNickName) free(mNickName);
	if (mFirstName) free(mFirstName);
	if (mLastName) free(mLastName);
	if (mCompanyName) free(mCompanyName);
	if (mTextList) free(mTextList);      
}


void contact::setPN(char* PN) {

  int numChars;


  numChars = strlen(PN) + 1;
  if (resizeBuff(numChars,(uint8_t**)&mPN)) {
    strcpy(mPN,PN);
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


void contact::setTextList(char* textList) {

  int numChars;

  numChars = strlen(textList) + 1;
  if (resizeBuff(numChars,(uint8_t**)&mTextList)) {
    strcpy(mTextList,textList);
    mShortBuff = false;
    mChanged = true;
  }
}


unsigned long contact::calculateBuffSize(void) {

  int numBytes;

  numBytes =  strlen(mPN) + 1 +
              strlen(mNickName) + 1 +
              strlen(mFirstName) + 1 +
              strlen(mLastName) + 1 +
              strlen(mCompanyName) + 1;
              if (mShortBuff) {
                numBytes =  numBytes + mTextBytes;
              } else {
                numBytes =  numBytes + strlen(mTextList) + 1;
              }
  return numBytes;
}


void contact::writeToBuff(char* buffPtr,unsigned long maxBytes) {
  
  contact*  ourClone;   // The clone idea. If we don't have a full read, use a clone to grab what's missing.
  int       offset;
  int       numBytes;
  int       i;
  bool      savedShort;

  savedShort = mShortBuff;                // We may loose this bit.
  if (mShortBuff) {                       // If were missing the message list, we need to get it!
      ourClone = new contact(mFile,mID);  // Create a clone to grab it from the SD card.
      setTextList(ourClone->mTextList);   // Copy it over to us.
      delete ourClone;                    // Recycle the clone.
  }
  mShortBuff = savedShort;                // Just in case..
  offset = 0;                             // Now, proceed as if nothing happened..
  
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

  numBytes = strlen(mTextList) + 1;       // The text message list.
  for(i=0;i<numBytes;i++) {
    buffPtr[i+offset] = mTextList[i];
  }
  offset = offset + i;
  if (mShortBuff) {                       // If we didn't have it before.
    resizeBuff(0,(uint8_t**)&mTextList);  // Toss it out now.
  }
  mChanged = false;
}


unsigned long contact::loadFromBuff(char* buffPtr,unsigned long maxBytes) {

  unsigned long buffIndex;

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
  setTextList(&(buffPtr[buffIndex]));
  buffIndex = buffIndex + strlen(mTextList) + 1;
  mTextBytes = strlen(mTextList) + 1;
  buffIndex = buffIndex + mTextBytes;               // We may not keep them, but we need to know how many there are.
  if (mShortBuff) {                                 // Asking only for indexing data.
    resizeBuff(0,(uint8_t**)&mTextList);            // Dump the message text.
  }
  mChanged = false;
  return buffIndex;   
}


void contact::printContact(void) {

	Serial.print("ID#        : ");Serial.println(mID);
	Serial.print("Nickname   : [");Serial.print(mNickName);Serial.println("]");
	Serial.print("Phone      : [");Serial.print(mPN);Serial.println("]");
	Serial.print("First name : [");Serial.print(mFirstName);Serial.println("]");
	Serial.print("Last name  : [");Serial.print(mLastName);Serial.println("]");
	Serial.print("Company    : [");Serial.print(mCompanyName);Serial.println("]");
	Serial.print("Messages   : [");Serial.print(mTextList);Serial.println("]");
	Serial.print("BYTES      : ");Serial.println(calculateBuffSize());
	Serial.println("-------------------------------------");Serial.flush();
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

  contactPtr = findByPN(phoneNum);
  if (!contactPtr) {
    blockID = mFile->getNewBlockID();
    contactPtr = new contact(mFile,blockID,phoneNum);
    addToTop(contactPtr);
  }
  return contactPtr;
}


unsigned long contactList::calculateBuffSize(void) { return sizeof(unsigned long) * getCount(); }

  
bool contactList::saveSubFileBuffs(void) {

  contact*        trace;

	Serial.println("-------------------------------------");
	Serial.println("------ Saving contacts to file ------");
	Serial.println("-------------------------------------");
  trace = (contact*)getFirst();       // Grab the first contact on our list.
  while(trace) {                      // While we have contacts..
    if (trace->mChanged) {            // If they have been edited (or new).
    	trace->printContact();
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

	Serial.println("-------------------------------------");
	Serial.println("---- Saving contact list to file ----");
	Serial.println("-------------------------------------");
  index = 0;                          // Starting at zero, usually a good plan.
  longPtr = (unsigned long*)buffPtr;  // If we are a pointer to X, we index by X. So they say.
  trace = (contact*)getFirst();       // Grab the first contact on our list.
  while(trace) {                      // While we have contacts..
  		Serial.print("ID# : ");Serial.println(trace->mID);Serial.flush();
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
    newContact = new contact(mFile,longPtr[i],true);  // Just indexing please.
    addToTop(newContact);                             // Add 'em to our list.
  }
  return numIDs * sizeof(unsigned long);              // That's how much we burned up.
}


void contactList::deleteContact(contact* theDoomed) {
	
	unsigned long	blockID;	
		
	if (theDoomed) {						// Sanity, did they send us someting?
		blockID = theDoomed->mID;		// Do it simple, save off the ID.
		mFile->deleteBlock(blockID);	// Using the ID, erase it from our file.
		unlinkObj(theDoomed);			// Now lets pull the node out of the list. (single link list)
		delete(theDoomed);				// Recycle the node.
		saveToFile();						// Make the file reflect what we have.
	}
}