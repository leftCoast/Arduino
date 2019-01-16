#include "dataTable.h"


// Just like IDnameBuff who we inherit from. There are two ways to be created. Someone says,
// "Lets make a new one called.." And resurection from stasis.

// This would be the call into exsistance of a new dataTable from mere thought..
dataTable::dataTable(blockFile* inFile,unsigned long blockID,char* inName)
  : IDnameBuff(inFile,blockID,inName);

  mDelete       = false;
  
  mNextFieldID  = 0;
  mFieldListID  = inFile->getNewBlockID();
  
  mRecordListID = inFile->getNewBlockID();
}


// This would be the call to bring one out of stasis.
dataTable::dataTable(blockFile* inFile,unsigned long blockID) 
  : IDnameBuff(inFile,blockID) { mDelete = false; }
  


dataTable::~dataTable(void) {

  if (mDelete) { eraseTable(); }
}
  
// How much of a buffer will we need to allocate to store off all of our
// information. This is just templates, file IDs etc. Not the actual user data.
unsigned long dataTable::calculateBuffSize(void) {

  numBytes = IDnameBuff::calculateBuffSize();   // Good starting point. Now add our bits in.
  
}


void dataTable::writeToBuff(char* buffPtr,unsigned long maxBytes) {
  
}


unsigned long dataTable::loadFromBuff(char* buffPtr,unsigned long maxBytes);


// This will be to add a field to the field list. Adds no data, but is a template
// for the records to start storing these.
void dataTable::addField(char* fieldName,fieldType newType) {

}


void dataTable::renameField(char* oldName,char* newName);
void dataTable::retypeField(char* fieldName,,fieldType newType);
void dataTable::deleteField(char* fieldName);

              int           mNextFieldID;
              unsigned long mFieldListID;
              unsigned long mRecordListID;
