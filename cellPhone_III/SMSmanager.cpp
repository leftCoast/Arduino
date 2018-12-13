#include "SMSmanager.h"

SMSmanager::SMSmanager(editField* inEditField,textView* inTextField) 
  : keyboard(inEditField) { mTextField = inTextField; }             // Do we need to delete this later?


SMSmanager::~SMSmanager(void) {  }

void SMSmanager::handleKey(keyCommands inEditCom) {

  char* buff;
  int   numChars;
  
  if (inEditCom == enter) {
    numChars = mEditField->getNumChars();
    buff = (char*)malloc(numChars+2);
    if (buff) {
      mEditField->getText(&buff[1]);
      buff[0]='\n';
      Serial.print("Sending : ");Serial.println(buff);
      mTextField->appendText(buff);
      buff[0] = '\0';
      mEditField->setValue(buff);
      free(buff);
    }
  } else {
    keyboard::handleKey(inEditCom);
  }
}
