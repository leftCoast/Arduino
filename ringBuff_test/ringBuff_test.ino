#include "lilParser.h"
#include "textBuff.h"

lilParser mParser;
textBuff*  ourRingBuff;

enum textBuffComSet  {
                      noCommand,
                      createRing,
                      deleteRing,
                      ringStats,
                      pushChar,
                      popChar,
                      clearBuff
                    };


                  
void setup() {

  ourRingBuff = new textBuff(5);      // We'll start off with 5 chars.
  
  mParser.addCmd(createRing, "new");
  mParser.addCmd(deleteRing,"dump");
  mParser.addCmd(ringStats,"see");
  mParser.addCmd(pushChar,"push");
  mParser.addCmd(popChar,"pop");
  mParser.addCmd(clearBuff,"clear");
  
  Serial.println("ready!");
}


void doCreateRing() {

  int   newVal;
  char* paramBuff;

  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    newVal = atoi (paramBuff);
    free(paramBuff);
    if (newVal>0) {
      if (ourRingBuff) delete ourRingBuff;
      ourRingBuff = new textBuff(newVal);
      if (ourRingBuff) {
        Serial.println("New textBuff created.");
      } else {
        Serial.println("Not enough RAM for that.");
      }
    } else {
      Serial.println("Bad inputted byte value.");
    }
  } else {
    Serial.println("No inputted byte value.");
  }
}


void doDeleteRing() {
  
  if (ourRingBuff) {
    delete ourRingBuff;
    ourRingBuff = NULL;
  }
  Serial.println("No more ring buff.");
}


void doRingStats() {
  
  if (ourRingBuff) {
    Serial.print("Number of bytes : ");Serial.println(ourRingBuff->buffSize());
    Serial.print("Number of chars : ");Serial.println(ourRingBuff->numChars());
    Serial.print("Peek head       : ");Serial.println(ourRingBuff->peekHead());
    Serial.print("Is full         : ");Serial.println(ourRingBuff->full());
    Serial.print("Is empty        : ");Serial.println(ourRingBuff->empty());
  } else {
    Serial.println("NO buffer to check.");
  }
}


void doPushChar() {

  char* paramBuff;

  if (mParser.numParams()) {
    paramBuff = mParser.getParam();
    if (ourRingBuff) {
      if (ourRingBuff->addChar(paramBuff[0])) {
        Serial.print("Pushed in [");Serial.print(paramBuff[0]);Serial.println("]");
      } else {
        Serial.println("Seems like its full.");
      }
    free(paramBuff);
    } else {
      Serial.println("No param buff to push into.");
    }
  } else {
    Serial.println("No char to push in.");
  }
}


void doPopChar() {

  char  aChar;
  
  if (ourRingBuff) {
    if (!ourRingBuff->empty()) {
      aChar = ourRingBuff->readChar();
      Serial.print("Popped [");Serial.print(aChar);Serial.println("]");
    } else {
      Serial.println("Sorry, empty buffer.");
    }
  } else {
    Serial.println("Sorry, no buffer at all!");
  }
}


void doClearBuff(void) {

  if (ourRingBuff) {
    ourRingBuff->clear();
    Serial.println("BUff cleared.");
  } else {
    Serial.println("No buff to clear!");
  }
}


void loop() {

  char  inChar;
  int   command;

  if (Serial.available()) {     // We have something in the queue?
    inChar = Serial.read();   // Read out a data charactor.
    Serial.print(inChar);   // If using development machine, echo the charactor.
    command = mParser.addChar(inChar);
    switch (command) {
      case noCommand    : break;
      case createRing   : doCreateRing(); break;
      case deleteRing   : doDeleteRing(); break;
      case ringStats    : doRingStats();  break;
      case pushChar     : doPushChar();   break;
      case popChar      : doPopChar();    break;
      case clearBuff    : doClearBuff();    break;
      default           : 
        Serial.println("Sorry, have no idea what you want.");
        Serial.println("new NUMBER gives you a new textBuff of size NUMBER.");
        Serial.println("dump deletes the current textBuff.");
        Serial.println("see outputs info on the current textBuff.");
        Serial.println("push CHAR pusing the charactor CHAR into the current textBuff.");
        Serial.println("pop pops the next charactor out of the current textBuff. Then prints it to the screen.");
    }
  }
}
