
#include <SD.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <timeObj.h>
#include <lilParser.h>

#include <colorRect.h>
#include <displayObj.h>
#include <drawObj.h>
#include <label.h>
#include <lineObj.h>
#include <screen.h>
#include <textView.h>
#include <editField.h>
#include <IOandKeys.h>
#include <blockFile.h>
#include <fileBuff.h>
//#include <litlOS.h>

#include	"../../cellOS.h"
#include "gameNode.h"
#include "qGame.h"


#define TB_X    10
#define TB_Y    20
#define TB_W    220
#define TB_H    148

#define TF_INSET  3
#define TF_X      TB_X+TF_INSET
#define TF_Y      TB_Y+TF_INSET
#define TF_W      TB_W-(2*TF_INSET)
#define TF_H      TB_H-(2*TF_INSET)

#define EB_X    TB_X
#define EB_Y    TB_Y+TB_H+18
#define EB_W    TB_W
#define EB_H    16

#define ET_INSET  3
#define ET_X      EB_X+ET_INSET
#define ET_Y      EB_Y+4
#define ET_W      EB_W-(2*ET_INSET)
#define ET_H      10

#define BATT_X    200
#define BATT_Y    3

#define FILENAME  "/objects.que"

#define out  theTextView->appendText

qGame::qGame(void)
  : panel(qGameApp,false) {  }


qGame::~qGame(void) {

  //if (theTextBase) delete theTextBase;
  if (theTextView) delete theTextView;
  if (theEditBase) delete theEditBase;
  if (theEditField) delete theEditField;
  if (ourKeyboard) delete ourKeyboard;
  if (theFile) delete theFile;
  if (lastNode) delete lastNode;
  if (currentNode) delete currentNode;
  if (objNode) delete objNode;
  if (qNode) delete qNode;
}


void qGame::setup() {

  
  lastNode = NULL;
  currentNode = NULL;
  objNode = NULL;
  qNode = NULL;
  inBuff[0]='\0';
  cIndex = 0;
  setupScreen();                        // Fire up the screen. If it fails it'll lock the process.
  
  theFile = new blockFile(FILENAME);
  if (theFile->isEmpty()) {
    setupSeedFile();
  }
  setupParser();
  out("HEY! Lets try a little challenge.\n");
  out("You think of something, and I'll try to guess it.\n");
  out("Type start when you're ready.\n");
  gameState = waitingToStart;
}


// Setup a new file and seed it with a few nodes.
void qGame::setupSeedFile(void) {

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
void qGame::setupParser(void) {

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
  mParser.addCmd(exitGame, "exit");
}

void qGame::setupScreen(void) {
  
  screen->fillScreen(&backColor);
  
  theTextView   = new textView(TF_X,TF_Y,TF_W,TF_H);
  theEditBase   = new colorRect(EB_X,EB_Y,EB_W,EB_H);
  theEditField  = new editField(ET_X,ET_Y,ET_W,ET_H,"",1);
  
  theEditField->setColors(&textSelectColor,&editFieldBColor);
  theEditBase->setColor(&editFieldBColor);
 
  viewList.addObj(theTextView);
  viewList.addObj(theEditBase);
  viewList.addObj(theEditField);

  ourKeyboard   = new IOandKeys(theEditField,theTextView);
  theTextView->setTextColors(&textColor,&backColor);

  screen->fillRect(TF_X,EB_Y-16,TF_W,3,&red);
}


void qGame::startGame(void) {

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
  ID = theFile->readRootBlockID();
  currentNode = new gameNode(theFile, ID);
  currentNode->getQuestion(qBuff);
  out("\n");out("- - - - - - - - - - - - - -");
  out("\n"); out(qBuff);out(" ");
  buffIndex = 0;
  gameState = needYNAnswer;
}


void qGame::dealWithYes(void) {

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
      out("\n");out("Is it ");out(qBuff);out("? ");
    } else {
      out("\n");out(qBuff);out(" ");
    }
    gameState = needYNAnswer;
  } else {
    out("\n"); out(qBuff);
    out(" So, would ya' like another go? ");
    gameState = needRestart;
  }
}


void qGame::dealWithNo(void) {

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
      out("\n");out("Is it ");out(qBuff);out("? ");
    } else {
      out("\n");out(qBuff);
    }
  } else {
    out("\n");out(qBuff);out(" ");
    buffIndex = 0;
    gameState = needNewObj;
  }
}


void qGame::swapAnswers(void) {

  unsigned long tempID;

  tempID = currentNode->getYes();
  currentNode->setYes(currentNode->getNo());
  currentNode->setNo(tempID);
  currentNode->saveToFile();
}


void qGame::newObjChar(char inChar) {

  unsigned long blockID;
  
  if (inChar==EOL) {
    qBuff[buffIndex] = '\0';
    blockID = theFile->getNewBlockID();
    objNode = new gameNode(theFile,blockID,qBuff);
    out("\n");
    out("Now, what could we ask to distiguish ");
    out(qBuff); out(" from ");
    currentNode->getQuestion(qBuff);
    out(qBuff); out("? ");
    buffIndex = 0;
    gameState = needNewQ;
  } else {
    qBuff[buffIndex] = inChar;
    buffIndex++;
  }
}


void qGame::newQChar(char inChar) {

  unsigned long blockID;
  
  if (inChar==EOL) {
    qBuff[buffIndex] = '\0';
    blockID = theFile->getNewBlockID();
    qNode = new gameNode(theFile,blockID,qBuff);
    out("\n");
    out("And the answer to choose ");
    objNode->getQuestion(qBuff);
    out(qBuff); out(" would be yes or no? ");
    buffIndex = 0;
    gameState = newQYNAnswer;
  } else {
    qBuff[buffIndex] = inChar;
    buffIndex++;
  }
}


void qGame::hookinNewNodes(bool yesNo) {

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
    out("\n");out("\n");
    out("There ye go laddie. All your changes and updates have been added to the machine.\n");
    out("Would you like to have another go and try it out? ");
    delete(currentNode); currentNode=NULL;
    delete(lastNode); lastNode=NULL;
    delete(qNode); qNode=NULL;
    delete(objNode); objNode=NULL;
    buffIndex = 0;
    gameState = waitingToStart;
}

void qGame::shutDown(void) {
	
	out("\nOk, thanks for playing!\n");
	delay(2000);
	close();
}


void qGame::loop() {

  char  inChar;
  int   numChars;
  int   command;

  numChars = ourKeyboard->haveBuff();     // We got new string to parse?
  if (numChars) {                         // Grab byte count.
    ourKeyboard->getBuff(inBuff,200);     // Grab the string itself..
    numChars = strlen(inBuff);
    inBuff[numChars++] = '\n';
    inBuff[numChars] = '\0';
    cIndex = 0;                            // reset index to the start.
  }
  if (cIndex<strlen(inBuff)) {             // Pretend its coming in from the serisl port.
    inChar = inBuff[cIndex++];
    if (gameState == needNewObj) {
      newObjChar(inChar);
    } else if (gameState == needNewQ) {
      newQChar(inChar);
    } else {
      command = mParser.addChar(inChar);
      switch (command) {              
        case noCommand  : break;
        case start    : 
          if (gameState == waitingToStart) {
            startGame();
          }
        break;
        case y        :
        case yes      :
        case ok       :
        case sure     :
          if (gameState==needYNAnswer) { 
            dealWithYes();
          } else if (gameState==needRestart||gameState == waitingToStart) {
            startGame();
          } else if (gameState==newQYNAnswer) {
            hookinNewNodes(true);
          }
        break;
        case n        :
        case no       :
        case nope     :
        case naw      :
          if (gameState==needYNAnswer) { 
            dealWithNo();
          } else if (gameState==newQYNAnswer) {
            hookinNewNodes(false);
          }  else if (gameState==needRestart||gameState == waitingToStart) {
            shutDown();
            gameState = waitingToStart;
          }
        break;
        case quit     :
        case stopx    :
        case exitGame :
          shutDown();
          gameState = waitingToStart;
        break;
        case swap     :
          if (gameState==needYNAnswer) {
            swapAnswers();
          }
        break;
        case resetx   :
          out("\n");
          out("***Deleting file.  ***");
          out("\n");
          theFile->deleteBlockfile();
          delete theFile;
          theFile = new blockFile(FILENAME);
          out("***Setup seed file.***");
          out("\n");
          setupSeedFile();
          out("***Restarting game.***");
          out("\n");
          out("Type start when you're ready. ");
          gameState = waitingToStart; 
        break;
      }
    }
  }
}
