#include <IOandKeys.h>

#define out  mTextField->appendText

IOandKeys::IOandKeys(editField* inEditField,textView* inTextField) 
  : keyboard(inEditField) {
    
  mTextField = inTextField;
  mHaveBuff = false;
  mOutBuff = NULL;
  mNumBytes = 0;
}         


IOandKeys::~IOandKeys(void) {  }


void IOandKeys::handleKey(keyCommands inEditCom) {

  int   numChars;
  
  if (inEditCom == enter) {                 // Got the return key.
    numChars = mEditField->getNumChars();   // How big is this new message?
    numChars++;
    if (resizeOutBuff(numChars)) {          // If we got the room..
      mEditField->getText(mOutBuff);        // Grab the message from the edit field.
      mEditField->setValue("");             // Clear the editField.
      mHaveBuff = true;
    }

  } else {
    keyboard::handleKey(inEditCom);         // Or, do the default suff.
  }
}


int IOandKeys::haveBuff(void) {

	if (mHaveBuff) {
		return mNumBytes;
	}
	return 0;
}


bool IOandKeys::getBuff(char* buff,int maxBytes) {
	
	for (int i=0;i<mNumBytes;i++) {
		if (i<maxBytes) {
			buff[i] = mOutBuff[i];
		}
	}
	resizeOutBuff(0);
	mHaveBuff = false;
  return true;
}



bool IOandKeys::resizeOutBuff(byte numBytes) {

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
