
#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <PulseOut.h>
#include <runningAvg.h>
#include <timeObj.h>
#include <lilParser.h>

#include "blockFile.h"
#include "fileBuff.h"
#include "gameNode.h"

#define SOUND_SDCS  21  // Chip select pin for the SD drive.

/*
class qGameMgr : public blockFile {

  public:

            qGameMgr(char* inFilePath);
  virtual   ~qGameMgr(void);

  virtual void  sendMessage(void);
  virtual
  
};
*/

enum commands { noCommand, start, y, yes, ok, sure, n, no, nope, naw, quit, stopx, resetx, swap };
lilParser mParser;

enum  gameStates { waitingToStart, needYNAnswer, needNewObj, needNewQ, newQYNAnswer, needRestart } gameState;

char        filename[] {"/objects.que"};
blockFile*  theFile;
gameNode*   lastNode;
gameNode*   currentNode;
gameNode*   objNode;
gameNode*   qNode;
char        qBuff[200];
int         buffIndex;

void setup() {

  lastNode = NULL;
  currentNode = NULL;
  objNode = NULL;
  qNode = NULL;
  
  Serial.begin(115200);
  if (!SD.begin(SOUND_SDCS)) {
    Serial.println(F("** File system FAIL."));
    while(1);
  }
  theFile = new blockFile(filename);
  if (theFile->isEmpty()) {
    setupSeedFile();
  }
  setupParser();
  Serial.println("HEY! Lets try a little challenge.");
  Serial.println("You think of something, and I'll try to guess it.");
  Serial.print("Type start when you're ready. ");
  gameState = waitingToStart;
}


// Setup a new file and seed it with a few nodes.
void setupSeedFile(void) {

  unsigned long blockID;
  gameNode*     startNode;
  gameNode*     yesNode;
  gameNode*     noNode;
  
  blockID = theFile->getNewBlockID();
  startNode = new gameNode(theFile, blockID, "Is it an animal?");
  blockID = theFile->getNewBlockID();
  yesNode = new gameNode(theFile, blockID, "an elephant");
  blockID = theFile->getNewBlockID();
  noNode = new gameNode(theFile, blockID, "a pine tree");
  startNode->setYes(yesNode->getID());
  startNode->setNo(noNode->getID());
  startNode->saveToFile();
  yesNode->saveToFile();
  noNode->saveToFile();
  delete(yesNode);
  delete(noNode);
  delete(startNode);
}


// List of things the parser can "see".
void setupParser(void) {

  mParser.addCmd(start, "start");
  mParser.addCmd(y, "y");
  mParser.addCmd(yes, "yes");
  mParser.addCmd(ok, "ok");
  mParser.addCmd(sure, "sure");
  mParser.addCmd(n, "n");
  mParser.addCmd(no, "no");
  mParser.addCmd(nope, "nope");
  mParser.addCmd(naw, "naw");
  mParser.addCmd(quit, "quit");
  mParser.addCmd(stopx, "stop");
  mParser.addCmd(resetx, "reset");
  mParser.addCmd(swap, "swap");
}


void startGame(void) {

  unsigned long ID;

  if (lastNode) {
    delete lastNode;
    lastNode = NULL;
  }
  if (currentNode) {
    delete currentNode;
    currentNode = NULL;
  }
  if (objNode) {
    delete objNode;
    objNode = NULL;
  }
  if (qNode) {
    delete qNode;
    qNode = NULL;
  }
  ID = theFile->readInitialBlockID();
  currentNode = new gameNode(theFile, ID);
  currentNode->getQuestion(qBuff);
  Serial.println();Serial.println("- - - - - - - - - - - - - -");
  Serial.println(); Serial.print(qBuff);Serial.print(" ");
  buffIndex = 0;
  gameState = needYNAnswer;
}


void dealWithYes(void) {

  unsigned long yesID;

  yesID = currentNode->yesResponse(qBuff);
  if (lastNode) {
    delete lastNode;
  }
  lastNode = currentNode;
  currentNode = NULL;
  if (yesID) {
    currentNode = new gameNode(theFile,yesID);
    currentNode->getQuestion(qBuff);
    if(currentNode->isAnswer()) {
      Serial.println();Serial.print("Is it ");Serial.print(qBuff);Serial.print("? ");
    } else {
      Serial.println();Serial.print(qBuff);
    }
    gameState = needYNAnswer;
  } else {
    Serial.println(); Serial.print(qBuff);
    Serial.print(" So, would ya' like another go? ");
    gameState = needRestart;
  }
}


void dealWithNo(void) {

  unsigned long noID;

  noID = currentNode->noResponse(qBuff);
  if (noID) {
    if (lastNode) {
      delete(lastNode);
    }
    lastNode = currentNode;
    currentNode = new gameNode(theFile,noID);
    currentNode->getQuestion(qBuff);
    if(currentNode->isAnswer()) {
      Serial.println();Serial.print("Is it ");Serial.print(qBuff);Serial.print("? ");
    } else {
      Serial.println();Serial.print(qBuff);
    }
  } else {
    Serial.println();Serial.print(qBuff);Serial.print(" ");
    buffIndex = 0;
    gameState = needNewObj;
  }
}


void swapAnswers(void) {

  unsigned long tempID;

  tempID = currentNode->getYes();
  currentNode->setYes(currentNode->getNo());
  currentNode->setNo(tempID);
  currentNode->saveToFile();
}


void newObjChar(char inChar) {

  unsigned long blockID;
  
  if (inChar==EOL) {
    qBuff[buffIndex] = '\0';
    blockID = theFile->getNewBlockID();
    objNode = new gameNode(theFile,blockID,qBuff);
    Serial.println();
    Serial.print("Now, what could we ask to distiguish ");
    Serial.print(qBuff); Serial.print(" from ");
    currentNode->getQuestion(qBuff);
    Serial.print(qBuff); Serial.print("? ");
    buffIndex = 0;
    gameState = needNewQ;
  } else {
    qBuff[buffIndex] = inChar;
    buffIndex++;
  }
}


void newQChar(char inChar) {

  unsigned long blockID;
  
  if (inChar==EOL) {
    qBuff[buffIndex] = '\0';
    blockID = theFile->getNewBlockID();
    qNode = new gameNode(theFile,blockID,qBuff);
    Serial.println();
    Serial.print("And the answer to choose ");
    objNode->getQuestion(qBuff);
    Serial.print(qBuff); Serial.print(" would be yes or no? ");
    buffIndex = 0;
    gameState = newQYNAnswer;
  } else {
    qBuff[buffIndex] = inChar;
    buffIndex++;
  }
}


void hookinNewNodes(bool yesNo) {

    if (yesNo) {
      qNode->setYes(objNode->getID());
      qNode->setNo(currentNode->getID());
    } else {
      qNode->setNo(objNode->getID());
      qNode->setYes(currentNode->getID());
    }
    if (currentNode->getID()==lastNode->getYes()) {
      lastNode->setYes(qNode->getID());
    } else {
      lastNode->setNo(qNode->getID());
    }
    currentNode->saveToFile();  // An Elephant
    lastNode->saveToFile();     // is animal
    qNode->saveToFile();        // Is it smaller than a breadbox?
    objNode->saveToFile();      // A rat.
    Serial.println();
    Serial.println("There ye go laddie. All your changes and updates have been added to the machine.");
    Serial.print("Would you like to have another go and try it out? ");
    delete(currentNode); currentNode=NULL;
    delete(lastNode); lastNode=NULL;
    delete(qNode); qNode=NULL;
    delete(objNode); objNode=NULL;
    buffIndex = 0;
    gameState = waitingToStart;
}


void loop() {

  char  inChar;
  int   command;
  
  if (Serial.available()) {
    inChar = Serial.read();
    Serial.print(inChar);
    if (gameState == needNewObj) {
      newObjChar(inChar);
    } else if (gameState == needNewQ) {
      newQChar(inChar);
    } else {
      command = mParser.addChar(inChar);
      switch (command) {                    //gameStates { waitingToStart, needYNAnswer, needNewObj, needNewQ, newQYNAnswer, needRestart } gameState;
        case noCommand : break;
        case start : 
          if (gameState == waitingToStart) {
            startGame();
          }
          break;
        case y :
        case yes :
        case ok :
        case sure :
          if (gameState==needYNAnswer) { 
            dealWithYes();
          } else if (gameState==needRestart||gameState == waitingToStart) {
            startGame();
          } else if (gameState==newQYNAnswer) {
            hookinNewNodes(true);
          }
          break;
        case n :
        case no :
        case nope :
        case naw :
          if (gameState==needYNAnswer) { 
            dealWithNo();
          } else if (gameState==newQYNAnswer) {
            hookinNewNodes(false);
          }
          break;
        case quit :
        case stopx   :
          delay(1500);
          Serial.println("I have nothng else I can do you know.");
          delay(1500);
          Serial.println("Lets do another round?");
          gameState = waitingToStart;
          break;
        case swap :
          if (gameState==needYNAnswer) {
            swapAnswers();
          }
          break;
        case resetx  :
          Serial.println();
          Serial.println("***     Deleting file.    ***");
          Serial.println();
          theFile->deleteBlockfile();
          delete theFile;
          theFile = new blockFile(filename);
          Serial.println("*** Setting up seed file. ***");
          Serial.println();
          setupSeedFile();
          Serial.println("***    Restarting game.   ***");
          Serial.println();
          Serial.print("Type start when you're ready. ");
          gameState = waitingToStart; 
        break;
      }
    }
  }
}
