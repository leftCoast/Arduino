
#include <lists.h>
#include <lilParser.h>

enum commands { noCommand, writeA, writeB, writeN };

lilParser mParser;


void setup() {
    Serial.begin(9600);
    mParser.addCmd(writeA, "wa");
    mParser.addCmd(writeB, "wb");
    mParser.addCmd(writeN, "n");
    while (!Serial);
  }

void loop() {

    char      inChar;
    commands  command;
    char      param[20];
    
    if (Serial.available()) {
      inChar = Serial.read();
      Serial.print(inChar);
      command = mParser.addChar(inChar);
      switch ((int)command) {                       // Cheat: Allows -1 passed back as error.
        case noCommand : break;
        case writeA : Serial.println("A"); break;
        case writeB : Serial.println("B"); break;
        case writeN : 
          if (mParser.numParams()) {
            while(mParser.getParam(param)) {
              Serial.println(param);
            }
          } else {
            Serial.println("no params");
          }
          break;
        case -1 : Serial.println("???"); break;
      }
    }
  }


