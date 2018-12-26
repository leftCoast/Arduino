#include "SMSmanager.h"

#define out  mTextField->appendText

SMSmanager::SMSmanager(editField* inEditField,textView* inTextField) 
  : keyboard(inEditField) {
    
  mTextField = inTextField;
  mOutBuff = NULL;
  mNumBytes = 0;
}         


SMSmanager::~SMSmanager(void) {  }


void SMSmanager::handleKey(keyCommands inEditCom) {

  int   numChars;
  
  if (inEditCom == enter) {                 // Got the return key.
    numChars = mEditField->getNumChars();   // How big is this new message?
    numChars++;
    out("resizing buffer\n");
    if (resizeOutBuff(numChars)) {          // If we got the room..
      mEditField->getText(mOutBuff);        // Grab the message from the edit field.
      mEditField->setValue("");             // Clear the editField.
      out("*** Hand off the string to be sent here. ***\n");
    }

  } else {
    out("key\n");
    keyboard::handleKey(inEditCom);         // Or, do the default suff.
  }
}


bool  SMSmanager::resizeOutBuff(byte numBytes) {

  mNumBytes = 0;
  if (mOutBuff) {
    free(mOutBuff);
    mOutBuff = NULL;
  }
  if (numBytes>0) {
    mOutBuff = (char*)malloc(numBytes);
    if (mOutBuff) {
      mNumBytes = numBytes;
      return true;
    } else {
      return false;
    }
  }
  return true;              // Because they asked for none.
}
