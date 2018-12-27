#include "gameNode.h"


// in this case you are created from memory, not a file.
// Create and save youself.
gameNode::gameNode(blockFile* inFile, unsigned long blockID, char* inQuestion) : fileBuff(inFile, blockID) {
//  tracer trace("gameNode::gameNode( from RAM )");

  question = (char*)malloc(strlen(inQuestion) + 1);
  if (question) {
    strcpy(question, inQuestion);
  }
  yesID = 0;
  noID = 0;
  saveToFile();
}


// Handed a blockFile object & ID. Create ourselves from a file buffer.
gameNode::gameNode(blockFile* inFile, unsigned long blockID) : fileBuff(inFile, blockID) {
//  tracer trace("gameNode::gameNode( from file )");

  readFromFile();
}


gameNode::~gameNode(void) {
//  tracer trace("gameNode::~gameNode()");

  if (question) {
    free(question);
  }
}


// returns next node ID or 0 on completion.
unsigned long gameNode::yesResponse(char* buff) {
//  tracer trace("gameNode::yesResponse()");

  if (!yesID) {
    strcpy(buff, "There ye' go laddie.");
  }
  return yesID;
}


// returns next node ID or 0 on completion.
unsigned long gameNode::noResponse(char* buff) {
//  tracer trace("gameNode::noResponse()");

  if (!noID) {
    strcpy(buff, "Its not? Well, I'll be wondering. What was it you had in mind?");
  }
  return noID;
}


unsigned long gameNode::getYes(void) { return yesID; }
unsigned long gameNode::getNo(void) { return noID; }

void gameNode::setYes(unsigned long ID)  {
//  tracer trace("gameNode::setYes()");

  yesID = ID;
}

void gameNode::setNo(unsigned long ID)  {
//  tracer trace("gameNode::setNo()");
  noID = ID;
}


// Leaf nodes are answers. Bot pointers at zero means this is a leaf.
bool gameNode::isAnswer(void) { return !(yesID||noID); }


void gameNode::getQuestion(char* buff) {
//  tracer trace("gameNode::getQuestion()");
  strcpy(buff, question);
}


// How large does the buffer have to be to hold us?
unsigned long gameNode::calculateBuffSize(void) {
//  tracer trace("gameNode::calculateBuffSize()");

  unsigned long numBytes;

  numBytes = sizeof(yesID);
  numBytes = numBytes + sizeof(noID);
  numBytes = numBytes + strlen(question) + 1;
  return numBytes;
}


void gameNode::writeToBuff(char* buffPtr, unsigned long maxBytes) {
//  tracer trace("gameNode::writeToBuff()");

  unsigned long* longPtr;

  longPtr = (unsigned long*)buffPtr;    // Point at the buffer as if we were a number;
  *longPtr = yesID;                     // Stuff this value into the buffer.
  buffPtr = (char*)longPtr;             // Bring out byte pointer up to date.
  buffPtr = buffPtr + sizeof(yesID);    // Move the pointer up for next write.

  longPtr = (unsigned long*)buffPtr;    // Point at the buffer as if we were a number;
  *longPtr = noID;                      // Stuff them into the buffer.
  buffPtr = (char*)longPtr;             // Bring out byte pointer up to date.
  buffPtr = buffPtr + sizeof(noID);     // Move the pointer up for next write.

  strcpy(buffPtr, question);             // And we're done.
}


unsigned long gameNode::loadFromBuff(char* buffPtr, unsigned long maxBytes) {
//  tracer trace("gameNode::loadFromBuff()");

  unsigned long*  longPtr;

  longPtr = (unsigned long*)buffPtr;                // Point at the buffer as if we were a number.
  yesID = *longPtr;                                 // Read 'em.
  buffPtr = (char*)longPtr;                         // Bring out byte pointer up to date.
  buffPtr = buffPtr + sizeof(yesID);                // Move the pointer up for next write.

  longPtr = (unsigned long*)buffPtr;               // Point at the buffer as if we were a number.
  noID = *longPtr;                                 // Read 'em.
  buffPtr = (char*)longPtr;                        // Bring out byte pointer up to date.
  buffPtr = buffPtr + sizeof(noID);                // Move the pointer up for next write.

  question = (char*)malloc(strlen(buffPtr) + 1);  // Make room.
  if (question) {                                 // If we got room.
    strcpy(question, buffPtr);                    // Copy out the string.
  }
}
