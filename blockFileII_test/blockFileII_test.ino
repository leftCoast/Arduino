#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Adafruit_ILI9341.h>
#include <adafruit_1947_Obj.h>  // ADAFRUIT_1947
#include <SD.h>

#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <PulseOut.h>
#include <runningAvg.h>
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

#include "blockFile.h"
#include "fileBuff.h"
#include "gameNode.h"

#define TFT_CS  10
#define TFT_RST -1  // I think I remember this is not used.
#define SD_CS   4

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
#define EB_Y    TB_Y+TB_H+8
#define EB_W    TB_W
#define EB_H    16

#define ET_INSET  3
#define ET_X      EB_X+ET_INSET
#define ET_Y      EB_Y+5
#define ET_W      EB_W-(2*ET_INSET)
#define ET_H      10

#define BATT_X    200
#define BATT_Y    3

#define out  theTextView->appendText

colorRect*    theTextBase;        // White rect behind the text indow.
textView*     theTextView;        // The text window.
colorRect*    theEditBase;        // White rect behind the text edit field.
editField*    theEditField;       // The text edit field.
IOandKeys*    ourKeyboard;        // Just like it sounds, our texting keyboard.


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
char        inBuff[200];  // When the user hits return, we'll stuff the answer here.
int         cIndex;

void setup() {

  
  lastNode = NULL;
  currentNode = NULL;
  objNode = NULL;
  qNode = NULL;
  inBuff[0]='\0';
  cIndex = 0;
  setupScreen();                        // Fire up the screen. If it fails it'll lock the process.
  
  if (SD.begin(SD_CS)) {
    out("File system ONLINE.\n");
    
  } else {
    out("File system FAIL.\n");
  }
  theFile = new blockFile(filename);
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

void setupScreen(void) {

  colorObj aColor(LC_GREY);
  
  if (!initScreen(ADAFRUIT_1947,TFT_CS,TFT_RST,PORTRAIT)) {
    while(true); // Kill the process.
  }
  
  screen->fillScreen(&aColor);
  
  theTextBase   = new colorRect(TB_X,TB_Y,TB_W,TB_H,2);
  theTextView   = new textView(TF_X,TF_Y,TF_W,TF_H);
  theEditBase   = new colorRect(EB_X,EB_Y,EB_W,EB_H,2);
  theEditField  = new editField(ET_X,ET_Y,ET_W,ET_H,"",1);
  //battIcon      = new battPercent(BATT_X,BATT_Y);
  
  
  viewList.addObj(theTextBase);
  viewList.addObj(theTextView);
  viewList.addObj(theEditBase);
  viewList.addObj(theEditField);
  //viewList.addObj(battIcon);

  ourKeyboard   = new IOandKeys(theEditField,theTextView);
  theTextView->setTextColors(&black,&white);
  //battIcon->setPercent(0);
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
  ID = theFile->readRootBlockID();
  currentNode = new gameNode(theFile, ID);
  currentNode->getQuestion(qBuff);
  out("\n");out("- - - - - - - - - - - - - -");
  out("\n"); out(qBuff);out(" ");
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


void newQChar(char inChar) {

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

void cantStop(void) {

  out("\nI've nothing else I can do you know.\n");
  out("How about another round?\n");
  out("Huh?\n");
}


void loop() {

  char  inChar;
  int   numChars;
  int   command;

  idle();

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
          }  else if (gameState==needRestart||gameState == waitingToStart) {
            cantStop();
            gameState = waitingToStart;
          }
          break;
        case quit :
        case stopx   :
          cantStop();
          gameState = waitingToStart;
          break;
        case swap :
          if (gameState==needYNAnswer) {
            swapAnswers();
          }
          break;
        case resetx  :
          out("\n");
          out("***Deleting file.  ***");
          out("\n");
          theFile->deleteBlockfile();
          delete theFile;
          theFile = new blockFile(filename);
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
