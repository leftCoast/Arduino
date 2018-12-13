#include "textView.h"         
#include  <label.h>

#define DEF_LINESPACE   2   // 2 Its a multiplyer. Ex: 2 * text size.

#ifndef EOL
#define EOL '\n'
#endif

// ********************************************************************************
// ********************************************************************************
// lineMarker hold markrs for line breaks in a text block.
// ********************************************************************************
// ********************************************************************************


lineMarker::lineMarker(int index,int numChars, bool hardBreak) {

  mIndex = index;
  mNumChars = numChars;
  mHardBreak = hardBreak;

  Serial.println("Creating line marker");
  Serial.print("mIndex : ");Serial.println(mIndex);
  Serial.print("mNumChars : ");Serial.println(mNumChars);
  Serial.print("mHardBreak : ");Serial.println(mHardBreak);
}


lineMarker::~lineMarker(void) {  }


// Did we end with a hard or soft break?
bool lineMarker::hardBreak(void) { return mHardBreak; }


// Index of the first charactor after us.
int lineMarker::nextIndex(void) { return mIndex+mNumChars; }


// Our text block. Or at least the beginning and length.
void  lineMarker::limits(int* index,int* numChars) {

  *index = mIndex;
  *numChars = mNumChars;
  Serial.print("Our limits, index : ");Serial.print(mIndex);Serial.print("  nNumChars : ");Serial.println(mNumChars);
}


bool lineMarker::hasIndex(int index) { return index>=mIndex && index<mIndex+mNumChars; }



// ********************************************************************************
// ********************************************************************************
// lineManager uses lineMarkers to manage line breaks in a text block.
// ********************************************************************************
// ********************************************************************************


lineManager::lineManager(void) : linkList() {

  mTextBuff = NULL;
  mOutBuff = NULL;
  mWidthChars = 0;
}


lineManager::~lineManager(void) {

  if (mTextBuff) free(mTextBuff);
  if (mOutBuff) free(mOutBuff);
 }


// Set how many chars wide the lines are.
void lineManager::setWidth(int widthChars) {

  dumpList();
  if (mOutBuff) {
    free(mOutBuff);
  }
  mOutBuff = (char*)malloc(widthChars+1);
  mWidthChars = widthChars;
}


// A possibly giant NULL terminated c string.
void lineManager::setText(char* text) {

    dumpList();
    if (mTextBuff) {
      free(mTextBuff);
    }
    mTextBuff = (char*)malloc(sizeof(text)+1);
    if (mTextBuff) {
      strcpy(mTextBuff,text);
    }
}


// Add this cstring to the end of our text. CK
void lineManager::appendText(char* text) {                         

  char* buff;
  int   numNewChars;
  int   numOurChars;
  int   totalChars;

  if(text) {                                    // Sanity, did they pass us a NULL?
    if (text[0]!='\0') {                        // I don't like the look of these people. Did they pass us an empty string?
      numNewChars = strlen(text);               // save off how many charactors they gave us.
      numOurChars = strlen(mTextBuff);          // save off how many charactors we have.
      totalChars = numOurChars + numNewChars;   // Figure total buffer length.
      buff = (char*)malloc(totalChars+1);       // Allocate the buffer.
      if (buff) {                               // If we got one.
        trimList(numOurChars-1);                // Trim outdated line.
        if (mTextBuff) {
          strcpy(buff,mTextBuff);                 // Copy over our text.
          strcpy(&buff[numOurChars],text);        // Then their text. (on top of the last '\0')
          free(mTextBuff);                        // loose our old text buffer.
          mTextBuff = buff;                       // Point to this shiny new one.
        } else {                                  // Wait, we have no buffer at all?
          strcpy(buff,text);                      // No wrries mate! We do this kind of thing all the time!
          mTextBuff = buff;                       // And there you go, your own text buffer. We won't tell anyone.
        }
      }
    }
  }
}


// Stick a NULL terminated substring IN FRONT of this index. CK
void lineManager::insertText(int index,char* text) {
              
  char*           buff;
  int             numNewChars;
  int             numOurChars;
  int             totalChars;
  int             i;
  
  if(text) {                                          // Sanity, did they pass us a NULL?
    if (text[0]!='\0') {                              // I don't like the look of these people. Did they pass us an empty string?
      numOurChars = strlen(mTextBuff);                // save off how many charactors we have.
      if (index<=numOurChars) {                       //
        trimList(index);                              // The list is corrupt from here on.
        numNewChars = strlen(text);                   // save off how many charactors they gave us.
        totalChars = numOurChars + numNewChars;       // Figure total buffer length.
        buff = (char*)malloc(totalChars+1);           // Allocate the buffer.
        if (buff) {                                   // If we got one.
          for(i=0;i<index;i++) {                      // Copy over the first piece of our text.
            buff[i] = mTextBuff[i];
          }                
          strcpy(&buff[i],text);                      // Then their text.
          strcpy(&buff[i+numNewChars],&mTextBuff[i]); // And the rest of our text.
          free(mTextBuff);                            // loose our old text buffer.
          mTextBuff = buff;                           // Point to this shiny new one.
          trimList(index);                            // Trim outdated lines.                                          
        }
      } else {
        appendText(text);                             // Trying to put text beyond us? Just append it and hope they are happy.
      }
    }
  }
}


// Remove some text from middle, beginning? End? CK
void lineManager::deleteText(int startIndex,int numChars) {

  int   textLen;
  int   endIndex;
  char* buff;
  
  if (mTextBuff && numChars>0) {                      // Sanity, we do have some right? And something to cut off?
    textLen = strlen(mTextBuff);                      // Get a char count.
    if(startIndex<textLen) {                          // Is the chop spot within the text?
      trimList(startIndex);                           // In any case we'll need to chop at least this much off.
      endIndex = startIndex+numChars-1;               // Calculate the ending index.
      if (endIndex>=textLen) {                        // No trailing to deal with.
        mTextBuff[startIndex]='\0';                   // Mark the end of the buffer.
        buff = (char*)malloc(startIndex+1);           // Allocate new buffer.
        strcpy(buff,mTextBuff);                       // copy in remeains of old buffer.
        free(mTextBuff);                              // Dispose of the old buffer.
        mTextBuff = buff;                             // Point at the fresh new buffer.
      } else {                                        // We have trailing text to move up.
        endIndex++;                                   // Point past to the first saved char.
        while(mTextBuff[endIndex]!='\0') {            // We do this 'till we hit the end.
          mTextBuff[startIndex]=mTextBuff[endIndex];  // Hop over the chars one by one.
          startIndex++;                               // Updaiting the pointers.
          endIndex++;                                 //
        }
        mTextBuff[startIndex]='\0';                   // On exit startIndex is pointing to the next place beyond text. Stamp in null char.
        textLen = strlen(mTextBuff);                  // Grab lenth.
        buff = (char*)malloc(textLen+1);              // Allocate buffer.
        strcpy(buff,mTextBuff);                       // Fill buffer.
        free(mTextBuff);                              // Dump old buffer.
        mTextBuff = buff;                             // replace with new buffer.
      }
    }
  }
}


// Index all the text. CK
void lineManager::indexAll(void) {

  int   index;
  bool  hardBreak;
  bool  endOfText;
  
  dumpList();
  if (mTextBuff) {
    if (strlen(mTextBuff)>0) {
      index = 0;
      hardBreak = true;
      endOfText = false;
      while(!endOfText) {
        endOfText = newMarker(&index,&hardBreak);
      }
    }
  }
}


// Index a subset of lines. CK
void lineManager::indexSet(int startLineNum,int numLines) {

  int         numChars;
  int         endLineNum;
  int         i;
  int         n;
  int         index;
  bool        hardBreak;
  bool        endOfText;
  lineMarker* temp;

  Serial.println("Index set");
  if (mTextBuff) {                                          // We do have text right? CK
    numChars = strlen(mTextBuff);                           // We have "this many". CK
    Serial.print("numChars : ");Serial.println(numChars);
    if (numChars>0) {                                       // More than just a null charactor? CK
      endLineNum = getCount()-1;                            // Index of the last line we have on the list. CK
      Serial.print("endLineNum : ");Serial.println(endLineNum);
      Serial.flush();
      if (endLineNum<startLineNum) {                        // If we have stuff to do BEFORE startLineNum. CK
        numLines = numLines + (startLineNum-endLineNum-1);  // We'll have to do more than we thought. CK
        temp = (lineMarker*)getByIndex(endLineNum);         // Grab the last valid line marker. CK
        if(temp) {                                          // Whatever, might as well check. CK
          index = temp->nextIndex();                        // Set next index. CK
          hardBreak = temp->hardBreak();                    // Same for hardbreak. CK
          temp->limits(&i,&n);                              // Need to calculate if we are past the end of the text. CK
          endOfText = (i+n)>=numChars;                      // Its true if i+n our end char is at or past numChars. CK
        } else {
          return;                                           // Something is very wrong. Lets just give up and not do anything.
        }
      } else {                                              // Ok, we have more than enough lines. CK
        temp = (lineMarker*)getByIndex(startLineNum-1);     // Grab the line before our start point.
        if(temp) {                                          // Whatever, might as well check. CK
          index = temp->nextIndex();                        // Set next index. CK
          hardBreak = temp->hardBreak();                    // Same for hardbreak. CK
          endOfText = false;                                // We had lines after this. CK
          trimList(startLineNum);                           // We have everyting we want. CLip off the list. CK
        } else {                                            // Didn't get what we were promised to get? CK
          return;                                           // Something is very wrong. Lets just give up and not do anything. CK
        }
      }
      while(!endOfText && numLines>0) {                     // FINALLY We have the info we need. Rebuild the section they called for. 
        endOfText = newMarker(&index,&hardBreak);
        numLines--;
      }
    }
  }
  Serial.println("Exiting indexSet()");
  Serial.flush();
}


char* lineManager::formatLine(int lineNum) {

  lineMarker* aLine;
  int         endLineNum;
  int         startIndex;
  int         numChars;
  int         i;
  
  endLineNum = getCount()-1;                      // Index of the last line we have on the list. CK
  if (endLineNum<lineNum) {                       // Not indexed? Really?! 
    indexAll();                                   // Don't bother bing clever, do the lot!
    endLineNum = getCount()-1;                    // Recheck it.
    if (endLineNum<lineNum) {                     // There's no line!
      mOutBuff[0] = '\0';                         // No text for you!
      return mOutBuff;                            // Take your empty string!
    }
  }
  aLine = (lineMarker*)getByIndex(lineNum);                   // We can get a line.
  startIndex = aLine->mIndex;                     // It starts here..
  numChars = aLine->mNumChars;                    // It burns this much text..
  if (!aLine->mHardBreak) {                       // It started with a soft break?
    while(mTextBuff[startIndex]==' '&&numChars) { // Skip leading blanks.
      startIndex++;
      numChars--;
    }
  }
  i = 0;                                          // i is the index for the output buffer.
  while((mTextBuff[startIndex]!='\0'||mTextBuff[startIndex]!='\n')&&numChars) { // Long line! We don't see the end, or a return and we ain't run out..
    mOutBuff[i]=mTextBuff[startIndex];            // Stamp in this charactor.
    startIndex++;                                 // bump.
    numChars--;                                   // bump
    i++;                                          // bump.
  }
  mOutBuff[i] = '\0';
  return mOutBuff;
}


// Starting at index, make a new marker. Return true for more text available. CK
bool lineManager::newMarker(int* index,bool* hardBreak) {        

  int         i;
  lineMarker* newLine;
  int         lastSpace;
  bool        seenPrint;

  Serial.println("New marker");
  i = *index;                                                             // Starting at index..
  lastSpace = -1;                                                         // Last place we've seen a space.
  if(!hardBreak) {                                                        // If its a soft break coming in.
    seenPrint = false;                                                    // Ok, not seen a printable charactor yet.
    while(!seenPrint) {                                                   // And until we do see one..
      if (mTextBuff[i]==EOL || mTextBuff[i]=='\0') {                      // Are we pointing at a terminator?
        *hardBreak = true;                                                // That means its a hard break you know.
        newLine = new lineMarker(*index,i-*index+1,*hardBreak);           // If so, create a new line marker.
        addToEnd(newLine);                                                // Save it in our list.
        *index = i+1;                                                     // Set index to the next char in line.
        return(mTextBuff[i]!='\0');                                       // i is still pointing to our last char. NULL char means no more.
      } else {
        if (isspace(mTextBuff[i])) {                                      // Are we pointing at a white space?
          i++;                                                            // On soft starts we eat up the leading spaces.
        } else {
          seenPrint = true;
        }
      }
    }
    i = *index;                                                         // We need to deduct the leading whitespace when leading from a soft start.
  }
  while(1) {                                                            // Lets just do this forever..
    if (mTextBuff[i]==EOL || mTextBuff[i]=='\0') {                      // Are we pointing at a terminator?
      *hardBreak = true;                                                // That means its a hard break you know.
      newLine = new lineMarker(*index,i-*index+1,*hardBreak);           // If so, create a new line marker.
      addToEnd(newLine);                                                // Save it in our list.
      *index = i+1;                                                     // Set index to the next char in line.
      return(mTextBuff[i]!='\0');                                       // i is still pointing to our last char. NULL char means no more.
    } else if (i==mWidthChars-1) {                                      // We didn't see an EOL or NULL char, but we are out of room. (mWidthChars was set by calling fx)
      if (isspace(mTextBuff[i])) {                                      // Are we looking at whitespace?
        *hardBreak = false;                                             // That means its a soft break.
        newLine = new lineMarker(*index,i-*index+1,*hardBreak);         // Make the line..
        addToEnd(newLine);                                              // Add to the list.
        *index = i+1;                                                   // Point index to the next charactor in line.                                      
        return(true);                                                   // There's more to come, lets go.
      } else if (lastSpace>=0) {                                        // Not at white space, did run out of room. But we did see one..
        *hardBreak = false;                                             // That means its a soft break.
        newLine = new lineMarker(*index,i-lastSpace+1,*hardBreak);      // Make the line..
        addToEnd(newLine);                                              // Add to the list.
        *index = lastSpace+1;                                           // Point index to the next charactor in line.                                      
        return(true);                                                   // There's more to come, lets go.
      } else {                                                          // Ok, were out of room, it was just one big block of text, chop it here!
        *hardBreak = true;                                              // Lets call this a hard break.
        newLine = new lineMarker(*index,i-*index+1,*hardBreak);         // Make the line..
        addToEnd(newLine);                                              // Add to the list.
        *index = i+1;                                                   // Point index to the next charactor in line.                                      
        return(true);                                                   // There's more to come, lets go.
      }
    } else {                                                            // Just cruising along..
      if (isspace(mTextBuff[i])) {                                      // Are we pointing at a white space?                                 
        lastSpace = i;                                                  // Keep track of the last space we saw.
      }
      i++;                                                              // Next!
    }
  }
}


// Given an index into the text buffer, find the line number
// of it. return -1 if not found.
int lineManager::getLineNum(int index) {

  lineMarker* temp;
  int         lineNum;
  
  lineNum = 0;
  temp = (lineMarker*)getList();
  while(temp) {
    if (temp->hasIndex(index)) {
      return lineNum;
    }
    lineNum++;
    temp = (lineMarker*)temp->getNext();
  }
  return(-1);
}


// Find the line that index is in, delete it and its tail. CK
void  lineManager::trimList(int index) {

  int         lineNum;
  lineMarker* theLine;
  
  lineNum = getLineNum(index);                    // See if we can find the line number.
  if (lineNum>0) {                                // If we found a number and its not the top.
      lineNum--;                                  // Ok, we want the previos one.
      theLine = (lineMarker*)getByIndex(lineNum); // Grab the line before the one that changed.
      theLine->deleteTail();                      // Delete its tail.
  } else if (lineNum==0) {                        // Its the head of the list.
    dumpList();                                   // Dump it all!
  }
}


           
// ********************************************************************************
// ********************************************************************************
// textView pulls all these bits together to manage text in a rectangle.
// ********************************************************************************
// ********************************************************************************


textView::textView(int inLocX, int inLocY, word inWidth,word inHeight,bool inClicks)
  : drawObj(inLocX,inLocY,inWidth,inHeight,inClicks) {

  setTextSize(DEF_TEXT_SIZE);
  setTextColor(&black);
  setFirstLine(0);
}


textView::~textView(void) {  }


void textView::calculate(void) {

  int tWidth;        // Width, in pixels of our font. (monospaced)
  int lineSpace;     // Pixels between lines.

          
  mTHeight = mTextSize * CHAR_HEIGHT;                   // Do the math..
  tWidth = mTextSize * CHAR_WIDTH;                      // Pixels across per charactor
  lineSpace = mTextSize * DEF_LINESPACE;                // Pixles between lines.
  mManager.setWidth(width/tWidth);                      // Solve for number of chars across.
  mNumLines = (height+lineSpace)/(mTHeight+lineSpace);  // I did the math on scratch paper, we'll see..
  needRefresh = true;                                   // We're going to need to redraw this.
  /*
  Serial.print("mTextSize : ");Serial.println(mTextSize);
  Serial.print("mTHeight  : ");Serial.println(mTHeight);
  Serial.print("tWidth    : ");Serial.println(tWidth);
  Serial.print("lineSpace : ");Serial.println(lineSpace);
  Serial.print("numChars  : ");Serial.println(width/tWidth);
  Serial.print("mNumLines : ");Serial.println(mNumLines);
  */
}



// Set the size of the text we are dealing with.
void textView::setTextSize(int tSize) {

  mTextSize = tSize;
  calculate();
}
  
// Well, what about a color?
void textView::setTextColor(colorObj* tColor) {
  
  mTextColor.setColor(tColor);
  needRefresh = true;
}


// Set the first line at the top of our rectangle.
void textView::setFirstLine(int lineNum) {

  mFirstLine = lineNum;
  needRefresh = true;
}


// Replace our text buff with a copy of this.
void textView::setText(char* text) {
  
  mManager.setText(text); 
  needRefresh = true;
}


// Add this to the end of our text.
void textView::appendText(char* text) {
  
  mManager.appendText(text); 
  needRefresh = true;
}


// Stick a NULL terminated substring in at this index.
void textView::insertText(int index,char* text)  {
  
  mManager.insertText(index,text); 
  needRefresh = true;
}


// Remove some text from middle, beginning? End?
void textView::deleteText(int startIndex,int numChars)  {

  mManager.deleteText(startIndex,numChars);
  needRefresh = true;
}

  
void textView::drawSelf(void) {

  char* textPtr;
  int   locY;

  locY = y;
  mManager.indexSet(mFirstLine,mNumLines);
  screen->setTextColor(&mTextColor);
  screen->setTextSize(mTextSize);
  screen->setTextWrap(false);
  for (int i=0;i<mNumLines;i++) {  
    textPtr =  mManager.formatLine(i+mFirstLine);
    screen->setCursor(x,locY+(i*mTHeight));
    screen->drawText(textPtr);
  }
}
  
         
