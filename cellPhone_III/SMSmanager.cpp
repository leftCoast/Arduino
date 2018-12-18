#include "SMSmanager.h"

SMSmanager::SMSmanager(editField* inEditField,textView* inTextField) 
  : keyboard(inEditField) { mTextField = inTextField; }             // Do we need to delete this later?


SMSmanager::~SMSmanager(void) {  }

void SMSmanager::handleKey(keyCommands inEditCom) {

  char* buff;
  int   numChars;
  int   index;
  
  if (inEditCom == enter) {
    numChars = mEditField->getNumChars();
    buff = (char*)malloc(numChars+2);
    if (buff) {
      mEditField->getText(buff);
      index = strlen(buff);
      buff[index]='\n';
      buff[index+1]='\0';
      mTextField->appendText(buff);
      Serial.print(buff);
      buff[0] = '\0';
      mEditField->setValue(buff);
      free(buff);
    }
  } else {
    keyboard::handleKey(inEditCom);
  }
}
