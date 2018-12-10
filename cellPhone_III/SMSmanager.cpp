#include "SMSmanager.h"

SMSmanager::SMSmanager(editField* inEditField,label* inTextField) 
  : keyboard(inEditField) { mTextField = inTextField; }             // Do we need to delete this later?


SMSmanager::~SMSmanager(void) {  }

void SMSmanager::handleKey(keyCommands inEditCom) {

  char* buff;
  int   numEditChars;
  int   numListChars;
  int   totalChars;
  
  if (inEditCom == enter) {
    numEditChars = mEditField->getNumChars();
    numListChars = mTextField->getNumChars();
    totalChars = numEditChars + numListChars;
    buff = (char*)malloc(totalChars+2);                // We want to add a '\n' to it.
    if (buff) {
      mTextField->getText(buff);
      mEditField->getText(&buff[numListChars]);
      buff[totalChars]='\n';
      buff[totalChars+1]='\0';
      mEditField->setValue("");
      mTextField->setValue(buff);
      free(buff);
    }
  } else {
    keyboard::handleKey(inEditCom);
  }
}
