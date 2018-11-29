
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



enum commands { noCommand, startx, y, yes, ok, sure, n, no, nope, naw, quit, stopx, resetx };
lilParser mParser;

char        filename[] {"/question.fle"};
blockFile*  theFile;
gameNode*   currentNode;
char        qBuff[200];

void setup() {

  currentNode = NULL;
  
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
  Serial.println("Type start when you're ready. ");
  Serial.flush();
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
  yesNode = new gameNode(theFile, blockID, "Is it an elephant?");
  blockID = theFile->getNewBlockID();
  noNode = new gameNode(theFile, blockID, "Is it a.. Pine tree?");
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

  mParser.addCmd(startx, "start");
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
}


void startGame(void) {

  unsigned long ID;

  ID = theFile->readInitialBlockID();
  if (currentNode) {
    delete currentNode;
  }
  currentNode = new gameNode(theFile, ID);
  currentNode->getQuestion(qBuff);
  Serial.println(); Serial.print(qBuff);
}


void dealWithYes(void) {

  unsigned long yesID;

  yesID = currentNode->yesResponse(qBuff);
  if (currentNode) {
    delete currentNode;
    currentNode = NULL;
  }
  if (yesID) {
    currentNode = new gameNode(theFile,yesID);
    currentNode->getQuestion(qBuff);
    Serial.println(); Serial.print(qBuff);
  } else {
    Serial.println(); Serial.print(qBuff);
    Serial.print("So, would ya' like another go? ");
  }
}


void dealWithNo(void) {

  unsigned long noID;

  noID = currentNode->noResponse(qBuff);
  if (noID) {
    if (currentNode) {
      delete(currentNode);
    }
    currentNode = new gameNode(theFile,noID);
    currentNode->getQuestion(qBuff);
    Serial.println(); Serial.print(qBuff);
  } else {
    Serial.println(); Serial.print(qBuff);
    dealwithNewQuestion();
  }
}


void dealwithNewQuestion(void) {

  bool          done;
  char          inChar;
  int           index;
  gameNode*     newNode;
  unsigned long blockID;

  index = 0;
  done = false;
  do {
    if (Serial.available()) {
      inChar = Serial.read();
      Serial.print(inChar);
      done = inChar == '\r';
      if (!done) {
        qBuff[index] = inChar;
        index++;
      }
    }
  } while (!done);
  qBuff[index] = '\0';
  Serial.println("Alright, we'll go with that, should the answer be yes or no? (Y/N) ");
  done = false;
  do {
    if (Serial.available()) {
      inChar = Serial.read();
      Serial.print(inChar);
      inChar = toupper(inChar);
      if (inChar == 'Y') {
      blockID = theFile->getNewBlockID();
        newNode = new gameNode(theFile, blockID, qBuff);
        currentNode->addNode(newNode, true);
        done = true;
      } else if (inChar == 'N') {
      blockID = theFile->getNewBlockID();
        newNode = new gameNode(theFile, blockID, qBuff);
        currentNode->addNode(newNode, false);
        done = true;
      } else {
        Serial.println("No no no, you misunderstand you need to type a Y or an N.");
      }
    }
  } while (!done);
  delete newNode;
  delete currentNode;
  currentNode = NULL;
  Serial.println("Very very good! Lets go again, shall we?");
}


void loop() {

  //unsigned long initialID;
  char  inChar;
  int   command;
  
  if (Serial.available()) {
    inChar = Serial.read();
    Serial.print(inChar);
    command = mParser.addChar(inChar);
    switch (command) {
      case noCommand : break;
      case startx : startGame(); break;
      case y :
      case yes :
      case ok :
      case sure : dealWithYes(); break;
      case n :
      case no :
      case nope :
      case naw :  dealWithNo(); break;
      case quit :
      case stopx   :
        delay(1500);
        Serial.println("I have nothng else I can do you know.");
        delay(1500);
        Serial.println("Lets do another round!");
        startGame();
        break;
      case resetx  : startGame(); break;
    }
  }
}
