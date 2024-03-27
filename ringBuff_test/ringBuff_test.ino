#include "lilParser.h"
#include "textBuff.h"
#include "strTools.h"

lilParser   mParser(80);
textBuff*   ourRingBuff;

enum textBuffComSet  {
                      noCommand,
                      createRing,
                      deleteRing,
                      ringStats,
                      pushChar,
                      popChar,
                      writeStr,
                      readStr,
                      strLen,
                      clearBuff,
                      paramX,
                      commands
                    };


                  
void setup() {

   Serial.begin(115200);
   
   ourRingBuff = new textBuff(5);      // We'll start off with 5 chars.
   
   mParser.addCmd(createRing, "new");
   mParser.addCmd(deleteRing,"dump");
   mParser.addCmd(ringStats,"see");
   mParser.addCmd(pushChar,"push");
   mParser.addCmd(popChar,"pop");
   mParser.addCmd(writeStr,"writeStr");
   mParser.addCmd(readStr,"readStr");
   mParser.addCmd(strLen,"strlen");
   mParser.addCmd(clearBuff,"clear");
   mParser.addCmd(paramX,"params");
   mParser.addCmd(commands,"???");
   
   showCommands();
   Serial.println("ready!");
}


void loop() {

  char  inChar;
  int   command;

   if (Serial.available()) {     // We have something in the queue?
      inChar = Serial.read();    // Read out a data charactor.
      Serial.print(inChar);      // If using development machine, echo the charactor.
      command = mParser.addChar(inChar);
      switch (command) {
         case noCommand    : break;
         case createRing   : doCreateRing(); break;
         case deleteRing   : doDeleteRing(); break;
         case ringStats    : doRingStats();  break;
         case pushChar     : doPushChar();   break;
         case popChar      : doPopChar();    break;
         case writeStr     : doWriteStr();   break;
         case readStr      : doReadStr();    break;
         case strLen       : doStrlen();     break;
         case clearBuff    : doClearBuff();  break;
         case paramX       : doParams();     break;
         case commands     : showCommands(); break;
         case PARAM_ERR    : Serial.println("Param string is too long.");           break;
         case CONFIG_ERR   : Serial.println("Config error, coudn't set up shop.");  break;
         default           : 
            Serial.println();
            Serial.println("Sorry, have no idea what you want. Type ??? for the list.");
      }
   }
}

void showCommands(void) {
   
   Serial.println( "                           Command list");
   Serial.println("     --------------------------------------------------------------");
   Serial.println("new & NUMBER  : Gives you a new textBuff of size NUMBER.");
   Serial.println("dump          : Deletes the current textBuff.");
   Serial.println("see           : Outputs info on the current textBuff.");
   Serial.println("push CHAR     : Pushes the charactor CHAR into the current textBuff.");
   Serial.println("pop           : Pops and displays the next charactor out of the textBuff.");
   Serial.println("writeStr TEXT : Writes TEXT as a string into the current textBuff.");
   Serial.println("readStr       : Reads a string out of the current textBuff.");
   Serial.println("strlen        : Returns the number of chars for the next string.");
   Serial.println("clear         : Clears the textBuff.");
   Serial.println("params        : Returns a count of its following params.");
   Serial.println("     --------------------------------------------------------------");
   Serial.println();
}


void doCreateRing() {

  int   newVal;
  char* paramBuff;

  if (mParser.numParams()) {
    paramBuff = mParser.getNextParam();
    newVal = atoi (paramBuff);
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


void doPushChar(void) {

  char* paramBuff;

  if (mParser.numParams()) {
    paramBuff = mParser.getNextParam();
    if (ourRingBuff) {
      if (ourRingBuff->addChar(paramBuff[0])) {
        Serial.print("Pushed in [");Serial.print(paramBuff[0]);Serial.println("]");
      } else {
        Serial.println("Seems like its full.");
      }
    } else {
      Serial.println("No param buff to push into.");
    }
  } else {
    Serial.println("No char to push in.");
  }
}


void doPopChar(void) {

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


void doWriteStr(void) {
    
   if (mParser.numParams()) {
      if (ourRingBuff) {
         if (!ourRingBuff->addStr(mParser.getParamBuff())) {
            Serial.println("Problem, probably ran out of buffer space?");
         }
      } else {
         Serial.println("Sorry, no ring buffer at all!");
      }
   } else {
      Serial.println("You need to type in a string after \"writeStr\"");
   }
 }

 
void doReadStr(void) {
   
   if (ourRingBuff) {
      Serial.print("Read [");
      Serial.print(ourRingBuff->readStr());
      Serial.println("]");
   }
}

      
void doClearBuff(void) {

  if (ourRingBuff) {
    ourRingBuff->clear();
    Serial.println("Buff cleared.");
  } else {
    Serial.println("No buff to clear!");
  }
}


void doStrlen(void) {

   if (ourRingBuff) {
      Serial.print("strlen() next read = ");
      Serial.print(ourRingBuff->strlen());
      Serial.println(" chars");
   } else {
      Serial.println("Umm.. No buffer to read from.");
   }
}


void doParams(void) {

   int num;

   num = (mParser.numParams());
   Serial.print("Reading ");
   Serial.print(num);
   Serial.println(" params.");
   for (int i=1;i<=num;i++) {
      Serial.print("param ");
      Serial.print(i);
      Serial.print(" -- ");
      Serial.println(mParser.getNextParam());
   }
   Serial.println();
}
