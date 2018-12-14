#include "textView.h"         
#include  <label.h>

#define DEF_LINESPACE   2   // 2 Its a multiplyer. Ex: 2 * text size.

#ifndef EOL
#define EOL '\n'
#endif

// ********************************************************************************
// ********************************************************************************
// lineMarker hold markers for line breaks in a text block.
// ********************************************************************************
// ********************************************************************************

// Ok, we do it all in the contructor, or at least a bunch.
lineMarker::lineMarker(int index,int* numChars,bool* hardBreak,bool* endOfText,int widthChars,char* buff) {

  int nextChar;
  int lastBlank;

  Serial.println("Making a line marker");
  mIndex = index;
  mNumChars = 0;
  mHardBreak = true;
  mEOT = false;
  mBlankLine = false;
  mStartIndex = index;

  if (buff==NULL||buff[index]=='\0') {  // Sanity! We have a buffer and we're not sitting at the end, right?
    mBlankLine = true;                  // Its a blank line for our purposes.
    mNumChars = 0;                      // We have zip for char count.
    mEOT = true;                        // This is the end.
    *numChars = 0;                      // Tell them we have zip!
    *endOfText = mEOT;                  // Tell them its the end of the text!
    *hardBreak = true;                  // Tell them.. Oh whatever!
    mHardBreak = *hardBreak;            // Save this for later. Used for list patching.
    return;                             // Blow this Taco stand!
  }
  mHardBreak = *hardBreak;            // They tell us if it was a hard break or not.
  if (!mHardBreak) {                  // Deal with the soft break case first.
    while(buff[index]==' ') {        // Soft breaks, eat all the leading blanks.
      index++;                        // bump our index.
      mNumChars++;                    // bump how many we span.
    }
    if (buff[index]=='\0') {          // Special case of a blank line.
      mBlankLine  = true;             // Note the special case.
      mNumChars = index-mIndex;       // We ate up this many.
      *numChars = mNumChars;          // Set up to pass this back.
      *endOfText = true;              // Tell them its the end of the text!
      mEOT = true;
      *hardBreak = true;              // Well, EOF is kinda' like a brick wall.
      mHardBreak = *hardBreak;        // Save this for later. Used for list patching.
      return;                         // Blow outta' here. 
    } else if (buff[index]==EOL) {    // Special case of EOL only.
      mBlankLine = true;              // Note the special case.
      mNumChars = index-mIndex;       // We ate up this many.
      *numChars = mNumChars;          // Set up to pass this back.
      *endOfText = false;             // Tell them its the end of the text!
      mEOT = false;
      *hardBreak = true;              // Well, EOL is kinda' like a brick wall.
      mHardBreak = *hardBreak;        // Save this for later. Used for list patching.
      return;                         // Blow outta' here. 
    }
  }
  mStartIndex = index;                // Now, either way our printiable text starts here.
  lastBlank = -1;                     // We may need this.
  while(buff[index]!=EOL &&
        buff[index]!='\0'&&
        mNumChars<widthChars) {
    if (buff[index]==' ') {          // Keep track of the last blank we saw.
      lastBlank = index;
    }
    index++;
    mNumChars++;       
  }
  if (buff[index]=='\0') {            // Ran out of text.
    Serial.println("Ran out of text");
    mNumChars = index-mIndex;       // We ate up this many.
    *numChars = mNumChars;          // Set up to pass this back.
    *endOfText = true;              // Tell them its the end of the text
    mEOT = true;
    *hardBreak = true;              // Well, EOF is kinda' like a brick wall.
    mHardBreak = *hardBreak;        // Save this for later. Used for list patching.
    return;                         // Blow outta' here. 
  } else if (buff[index]==EOL) {    // Hit an EOL.
    index++;                        // Ok, pop it past the EOL.
    mNumChars = index-mIndex;       // We ate up this many.
    *numChars = mNumChars;          // Set up to pass this back.
    *endOfText = false;             // Tell them its the end of the text!
    mEOT = false;
    *hardBreak = true;              // Well, EOL is kinda' like a brick wall.
    mHardBreak = *hardBreak;        // Save this for later. Used for list patching.
    return;                         // Blow outta' here. 
  } else if (buff[index]==' ') {   // Its a blank so break here.
    mNumChars = index-mIndex;       // We ate up this many.
    *numChars = mNumChars;          // Set up to pass this back.
    *endOfText = false;             // Tell them its the end of the text!
    mEOT = false;
    *hardBreak = false;             // Well, blank is the ultimate soft break.
    mHardBreak = *hardBreak;        // Save this for later. Used for list patching.
    return;                         // Done, lets go.
  } else {                          // Looking at a printable charactor.
    nextChar = index+1;             // We need to look ahead to decide.
    if (buff[nextChar]==' '||      // If the look-ahead is some sort of a break.
        buff[nextChar]==EOL ||         
        buff[nextChar]=='\0') {
      mNumChars = index-mIndex;     // We ate up this many.
      *numChars = mNumChars;        // Set up to pass this back.
      *endOfText = false;           // Tell them its the end of the text!
      mEOT = false;
      *hardBreak = false;           // Well, EOL is kinda' like a brick wall.
      return;                       // Done, lets go.
    } else if (lastBlank>-1) {      // Either we go back to the last blank or chop it there.
      mNumChars = lastBlank-mIndex; // We ate up this many.
      *numChars = mNumChars;        // Set up to pass this back.
      *endOfText = false;           // Tell them its the end of the text!
      mEOT = false;
      *hardBreak = false;           // 
      mHardBreak = *hardBreak;      // Save this for later. Used for list patching.
      return;                       // Done, lets go.
    } else {                        // in this last case..
      mNumChars = index-mIndex;     // Nothing but a big text block. Just chop it here.
      *numChars = mNumChars;        // Set up to pass this back.
      *endOfText = false;           // Tell them its the end of the text!
      mEOT = false;
      *hardBreak = false;           //
      mHardBreak = *hardBreak;      // Save this for later. Used for list patching.
    }
  }
  Serial.println("Creating line marker");
  Serial.print("mIndex : ");Serial.println(mIndex);
  Serial.print("mNumChars : ");Serial.println(mNumChars);
  Serial.print("mHardBreak : ");Serial.println(mHardBreak);
}


lineMarker::~lineMarker(void) {  }


bool lineMarker::hasIndex(int index) { return index>=mIndex && index<mIndex+mNumChars; }


void  lineMarker::formatLine(char* text,char* line,int maxChars) {

  int i;
  int index;
  int endIndex;
  
  if(mBlankLine) {
    strcpy(line,"");
  } else {
    index = 0;
    endIndex = mIndex + mNumChars;
    for(i=mStartIndex;i<endIndex;i++) {
      if (index<maxChars) {
        line[index] = text[i];
        index++;
      }
    }
    line[index] = '\0';
  }
}


int lineMarker::nextIndex(void) { return mIndex+mNumChars; }


bool lineMarker::getBreak(void) { return mHardBreak; }


bool lineMarker::getEnd(void) { return mEOT; }



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


// Add this cstring to the end of our text.
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


// Stick a NULL terminated substring IN FRONT of this index.
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


// Remove some text from middle, beginning? End?
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


// Index all the text.
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


// Index a subset of lines.
void lineManager::indexSet(int endLineNum) {

   int          numLines;
   int          lineNum;
   lineMarker*  aLine;
   bool         endOfText;
   int          index;
   bool         hardBreak;

   if (mTextBuff) {
      numLines = getCount();
      if (!numLines) {
        index = 0;
        hardBreak = true;
        endOfText = false;
      } else {
        lineNum = numLines = 1;
        aLine = (lineMarker*)getByIndex(lineNum);
        index = aLine->nextIndex();
        hardBreak = aLine->getBreak();
        endOfText = aLine->getEnd();
      }
      Serial.print("numLines : ");Serial.println(numLines);
      Serial.print("lineNum : ");Serial.println(lineNum);
      Serial.print("index : ");Serial.println(index);
      Serial.print("hardBreak : ");Serial.println(hardBreak);
      Serial.print("endOfText : ");Serial.println(endOfText);
      Serial.println("Looping : ");
      while(lineNum<endLineNum&&!endOfText) {
        endOfText = newMarker(&index,&hardBreak);
        lineNum++;
        Serial.print("lineNum : ");Serial.println(lineNum);
         Serial.print("index : ");Serial.println(index);
        Serial.print("hardBreak : ");Serial.println(hardBreak);
        Serial.print("endOfText : ");Serial.println(endOfText);
        Serial.println("-------------");
      }
      Serial.println("Loop complete : ");
      Serial.print("lineNum : ");Serial.println(lineNum);
      Serial.print("index : ");Serial.println(index);
      Serial.print("hardBreak : ");Serial.println(hardBreak);
      Serial.print("endOfText : ");Serial.println(endOfText);
   }
}


char* lineManager::formatLine(int lineNum) {

  lineMarker* aLine;
  int         endLineNum;
  
  endLineNum = getCount()-1;                          // Index of the last line we have on the list.
  if (endLineNum<lineNum) {                           // Not indexed? Really?! 
    indexAll();                                       // Don't bother bing clever, do the lot!
    endLineNum = getCount()-1;                        // Recheck it.
    if (endLineNum<lineNum) {                         // There's no line!
      mOutBuff[0] = '\0';                             // No text for you!
      return mOutBuff;                                // Take your empty string!
    }
  }
  aLine = (lineMarker*)getByIndex(lineNum);           // We can get a line.
  aLine->formatLine(mTextBuff,mOutBuff,mWidthChars);
  return mOutBuff;
}


// Starting at index, make a new marker. Add it to our list
// and return true for more text available.
bool lineManager::newMarker(int* index,bool* hardBreak) {        

  lineMarker* aMarker;
  int         numChars;
  int         locIndex;
  bool        endOfText;

  endOfText = true;
  if (mTextBuff) {                    // FIRST we have text.
    if (mTextBuff[*index]!='\0') {    // And we're not at the end of the line.
      locIndex = *index;
      aMarker = new lineMarker(locIndex,&numChars,hardBreak,&endOfText,mWidthChars,mTextBuff);
      addToEnd(aMarker);
      *index = locIndex+numChars;
    }
  }
  return endOfText;
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


// Find the line that index is in, delete it and its tail.
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
  mManager.indexSet(mFirstLine+mNumLines);
  screen->setTextColor(&mTextColor);
  screen->setTextSize(mTextSize);
  screen->setTextWrap(false);
  for (int i=0;i<mNumLines;i++) {  
    textPtr =  mManager.formatLine(i+mFirstLine);
    screen->setCursor(x,locY+(i*mTHeight));
    //screen->drawText("Well?:");
    screen->drawText(textPtr);
  }
}
  
         
