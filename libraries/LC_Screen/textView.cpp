#include "textView.h"
#include  <label.h>

#define DEF_LINESPACE   2   // 2 Its a multiplyer. Ex: 2 * text size.

#ifndef EOL
#define EOL '\n'
#endif

#define TOOL_BUFF_SIZE	80	// Needed for formating numbers
char tool[TOOL_BUFF_SIZE];

char  mapChar(char aChar) {

  switch(aChar) {
    case '\0' :return ('~');
    case EOL : return('>');
    case ' ' : return('*');
    case '\r': return('^');
    default  : return(aChar);
  }
}

// ********************************************************************************
// ********************************************************************************
// lineMarker hold markers for line breaks in a text block.
// ********************************************************************************
// ********************************************************************************

// Ok, we do it all in the contructor, or at least a bunch.
lineMarker::lineMarker(int index, bool hardBreak, int widthChars, char* buff) { parseText(index,hardBreak,widthChars,buff); }


lineMarker::~lineMarker(void) {  }


void lineMarker::parseText(int index, bool startHardBreak, int widthChars, char* buff) {

  int lastBlank;

  mStartIndex = index;
  mEndHBreak = false;
  if (buff) {
    if(buff[index]!='\0') {
      if (widthChars>0) {
        if(!startHardBreak) {
          while(buff[index]==' ') index++;
        }
        mPrintIndex = index;                    // If we're to print anything it'll start here.
        lastBlank = mStartIndex - 1;            // Set to impossibility as a flag.
        widthChars--;                           // bump down allowed space.
        while ( buff[index] != '\0' &&          // Loop 'till.. End of text
                buff[index] != EOL &&           // end of line.
                widthChars) {                   // Ran out of printable chars.
          if (buff[index] == ' ') {             // See a space?
            lastBlank = index;                  // Keep track of it. May need it later.
          }
          index++;                              // bump up index.
          widthChars--;                         // bump down allowed space.
        }
        switch( buff[index]) {
          case '\0' : index--;                  // Ran of the end. Step back and..
          case EOL  : mEndHBreak = true; break; // '\0' or carage return. Hard break.
          case ' '  : break;                    // Space, do nothing.
          default :                             // Stopped in txt block.
            switch (buff[index+1]) {            // We need to see if there's a line break after.
              case  '\0'  : 
              case EOL    :
              case ' '    : break;              // Line or word break, don't sweat it.
              default :                         // In a text block.
                if (lastBlank>=mStartIndex) {   // If there was a break back there..
                  index = lastBlank;            // Just use it and we are done.
                }
              break;                            // If not? We're done anyway.
            }
          break;
        }
      }
    }
  }
  mEndIndex = index;                            // In the end, this is where we stop.
}

  
void  lineMarker::formatLine(char* inText, char* outLine, int maxChars) {

  int inIndex;
  int outIndex;
  
  //rawText(inText);Serial.println();
  inIndex = mPrintIndex;
  outIndex = 0;
  while(inIndex<=mEndIndex&&maxChars) {
    if(inText[inIndex]!=EOL) {
      outLine[outIndex] = inText[inIndex];
      outIndex++;
      maxChars--;
    }
    inIndex++;
  }
  outLine[outIndex] = '\0';
}


// This is WAY handy as a debug tool!
void lineMarker::rawText(char* text) {
  
  for(int i=mPrintIndex;i<=mEndIndex;i++) {
    Serial.print(mapChar(text[i]));
  }
}


bool lineMarker::hasIndex(int index) { return (mStartIndex<=index) && (mEndIndex>=index); }


bool lineMarker::getBreak(void) { return mEndHBreak; }


int lineMarker::getEndIndex(void) { return mEndIndex; }



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
  mOutBuff = (char*)malloc(widthChars + 1);
  mWidthChars = widthChars;
}


// A possibly giant NULL terminated c string.
void lineManager::setText(char* text) {

  dumpList();
  if (mTextBuff) {
    free(mTextBuff);
  }
  mTextBuff = (char*)malloc(strlen(text) + 1);
  if (mTextBuff) {
    strcpy(mTextBuff, text);
  }
}


// Add this cstring to the end of our text.
void lineManager::appendText(char* text) {

  char* buff;
  int   numNewChars;
  int   numOurChars;
  int   totalChars;

  if (text) {                                   // Sanity, did they pass us a NULL?
    if (text[0] != '\0') {                      // I don't like the look of these people. Did they pass us an empty string?
      numNewChars = strlen(text);               // save off how many charactors they gave us.
      numOurChars = strlen(mTextBuff);          // save off how many charactors we have.
      totalChars = numOurChars + numNewChars;   // Figure total buffer length.
      buff = (char*)malloc(totalChars + 1);     // Allocate the buffer.
      if (buff) {                               // If we got one.
        trimList(numOurChars-1);                // Trim outdated line.
        if (mTextBuff) {
          strcpy(buff, mTextBuff);                // Copy over our text.
          strcpy(&buff[numOurChars], text);       // Then their text. (on top of the last '\0')
          free(mTextBuff);                        // loose our old text buffer.
          mTextBuff = buff;                       // Point to this shiny new one.
        } else {                                  // Wait, we have no buffer at all?
          strcpy(buff, text);                     // No worries mate! We do this kind of thing all the time!
          mTextBuff = buff;                       // And there you go, your own text buffer. We won't tell anyone.
        }
      }
    }
  }
}


// Stick a NULL terminated substring IN FRONT of this index.
void lineManager::insertText(int index, char* text) {

  char*           buff;
  int             numNewChars;
  int             numOurChars;
  int             totalChars;
  int             i;

  if (text) {                                         // Sanity, did they pass us a NULL?
    if (text[0] != '\0') {                            // I don't like the look of these people. Did they pass us an empty string?
      numOurChars = strlen(mTextBuff);                // save off how many charactors we have.
      if (index <= numOurChars) {                     //
        trimList(index);                              // The list is corrupt from here on.
        numNewChars = strlen(text);                   // save off how many charactors they gave us.
        totalChars = numOurChars + numNewChars;       // Figure total buffer length.
        buff = (char*)malloc(totalChars + 1);         // Allocate the buffer.
        if (buff) {                                   // If we got one.
          for (i = 0; i < index; i++) {               // Copy over the first piece of our text.
            buff[i] = mTextBuff[i];
          }
          strcpy(&buff[i], text);                     // Then their text.
          strcpy(&buff[i + numNewChars], &mTextBuff[i]); // And the rest of our text.
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
void lineManager::deleteText(int startIndex, int numChars) {

  int   textLen;
  int   endIndex;
  char* buff;

  if (mTextBuff && numChars > 0) {                    // Sanity, we do have some right? And something to cut off?
    textLen = strlen(mTextBuff);                      // Get a char count.
    if (startIndex < textLen) {                       // Is the chop spot within the text?
      trimList(startIndex);                           // In any case we'll need to chop at least this much off.
      endIndex = startIndex + numChars - 1;           // Calculate the ending index.
      if (endIndex >= textLen) {                      // No trailing to deal with.
        mTextBuff[startIndex] = '\0';                 // Mark the end of the buffer.
        buff = (char*)malloc(startIndex + 1);         // Allocate new buffer.
        strcpy(buff, mTextBuff);                      // copy in remeains of old buffer.
        free(mTextBuff);                              // Dispose of the old buffer.
        mTextBuff = buff;                             // Point at the fresh new buffer.
      } else {                                        // We have trailing text to move up.
        endIndex++;                                   // Point past to the first saved char.
        while (mTextBuff[endIndex] != '\0') {         // We do this 'till we hit the end.
          mTextBuff[startIndex] = mTextBuff[endIndex]; // Hop over the chars one by one.
          startIndex++;                               // Updaiting the pointers.
          endIndex++;                                 //
        }
        mTextBuff[startIndex] = '\0';                 // On exit startIndex is pointing to the next place beyond text. Stamp in null char.
        textLen = strlen(mTextBuff);                  // Grab lenth.
        buff = (char*)malloc(textLen + 1);            // Allocate buffer.
        strcpy(buff, mTextBuff);                      // Fill buffer.
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
    if (strlen(mTextBuff) > 0) {
      index = 0;
      hardBreak = true;
      endOfText = false;
      while (!endOfText) {
        endOfText = newMarker(&index, &hardBreak);
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

  lineNum = 0;														// Ok, shut up compiler.
  if (mTextBuff) {                                    // We do have a text buffer.
    if (mTextBuff[0]!='\0') {                         // And at least some text.
      numLines = getCount();                          // Count up the lines we currently have..
      if (!numLines) {                                // We have no lines?
        index = 0;                                    // Setup for a good parsing.
        hardBreak = true;
        endOfText = false;
      } else {                                        // We have some lines, I guess.
        lineNum = numLines - 1;                       // Finding index to the last one.
        aLine = (lineMarker*)getByIndex(lineNum);     // Getting a pointer to the last one.
        index = aLine->getEndIndex()+1;               // Getting the starting point from this line.
        hardBreak = aLine->getBreak();                // Was it ended on a hard or soft break.
        endOfText = mTextBuff[index]=='\0';           // Was it the end of the text?
      }
      if(endLineNum==-1) {                            // Flag for all lines.
        while (!endOfText) {                          // Until we see the end of the text.
          endOfText = newMarker(&index, &hardBreak);  // Punch out line markers.
        }
      } else {
        while (lineNum < endLineNum && !endOfText) {    // Until we see enough, or we're at the end of the text.
          endOfText = newMarker(&index, &hardBreak);    // Punch out line markers.
          lineNum++;                                    // Keep track of how many.
        }
      }
    }
  }
}


// Before wasting yet more time. Lets just see if we can
// sort this any more than it is.
bool lineManager::upToDate(void) {

  lineMarker* aLine;
  int         lastIndex;
  
  if (mTextBuff) {                            // We have a text buffer?
    if (mTextBuff[0]!='\0') {                 // We do! Is it empty?
        aLine = (lineMarker*)getLast();       // Grab the last one.
        if (aLine) {
          lastIndex = aLine->getEndIndex()+1; // Index after this one *is* the same as char count.
          return mTextBuff[lastIndex]=='\0';
        } else {                               // Not seen the end?
          return false;                        // Probably not sorted then.
        }
      }
    }
  return true;                                  // These cases, no text. Any answer is the corect sorted answer.
}


char* lineManager::formatLine(int lineNum) {

  lineMarker* aLine;
  int         endLineNum;

  endLineNum = getCount() - 1;                        // Index of the last line we have on the list.
  if (endLineNum < lineNum) {                         // Not indexed? Really?!
    if (!upToDate()) {                                // Hold on, lets just see if we can sort it any more.
      indexAll();                                     // Don't bother bing clever, do the lot!
    }
    endLineNum = getCount() - 1;                      // Recheck it.
    if (endLineNum < lineNum) {                       // There's no line!
      mOutBuff[0] = '\0';                             // No text for you!
      return mOutBuff;                                // Take your empty string!
    }
  }
  aLine = (lineMarker*)getByIndex(lineNum);           // We can get a line.
  aLine->formatLine(mTextBuff, mOutBuff, mWidthChars);
  return mOutBuff;
}


// Starting at index, make a new marker. Add it to our list
// and return true for more text available.
bool lineManager::newMarker(int* index, bool* hardBreak) {

  lineMarker* aMarker;
  bool        endOfText;

  endOfText = true;
  if (mTextBuff) {                    // FIRST, we have text?
    if (mTextBuff[*index] != '\0') {  // And we're not at the end of the line?
      aMarker = new lineMarker(*index,*hardBreak,mWidthChars,mTextBuff);
      addToEnd(aMarker);
      *index = aMarker->getEndIndex() + 1;
      *hardBreak = aMarker->getBreak();
      endOfText = mTextBuff[*index] == '\0';
    }
  }
  return endOfText;
}


// Want to know if this lineMarker exists.
// Return it's index if found. -1 if not.
int lineManager::getExistingLineNum(int index) {

  lineMarker* temp;
  int         lineNum;

  lineNum = 0;
  temp = (lineMarker*)getFirst();
  while (temp) {
    if (temp->hasIndex(index)) {
      return lineNum;
    }
    lineNum++;
    temp = (lineMarker*)temp->getNext();
  }
  return (-1);
}


// Want the line this guy is in, Index as neccisary.
int lineManager::getLineNum(int index) {

  int lineNum;
  
  lineNum = getExistingLineNum(index);    // First try with what we have.
  if (lineNum==-1) {                      // No?
    indexSet(-1);                         // Fine. Index the lot of them.
    lineNum = getExistingLineNum(index);  // Grab the line number, or error, now.
  }
  return lineNum;                         // Off it goes!
}


 
// Find the line that index is in, delete it and its tail.
void  lineManager::trimList(int index) {

  int         lineNum;
  lineMarker* theLine;

  if(index>0) {                                   // Zero woud be everything. Less thatn zero? Lazy coding.
    lineNum = getExistingLineNum(index);         // See if we already have this line number.
    if (lineNum > 0) {                            // If we found a number and its not the top.
      lineNum--;                                  // Ok, we want the previos one.
      theLine = (lineMarker*)getByIndex(lineNum); // Grab the line before the one that changed.
      theLine->deleteTail();                      // Delete its tail.
    }
  } else {                                        // Its the head of the list. Or negitive? Whatever!
    dumpList();                                   // When in doubt, dump it all!
  }  
}


// Need to be sure the entire list has been indexed.
// This returns how many lines we have.
int lineManager::getNumLines(void) {

  indexSet(-1);       // -1, make sure they are all indexed.
  return getCount();  // rerurn the count.
}


// ********************************************************************************
// ********************************************************************************
// textView pulls all these bits together to manage text in a rectangle.
// ********************************************************************************
// ********************************************************************************


textView::textView(int inLocX, int inLocY, int inWidth, int inHeight,eventSet inEventSet)
  : drawObj(inLocX, inLocY, inWidth, inHeight, inEventSet) {

  setTextSize(DEF_TEXT_SIZE);
  setTextColor(&black);
  mFirstLine = 0;
}


textView::~textView(void) {  }


void textView::calculate(void) {

  int tWidth;        // Width, in pixels of our font. (monospaced)


  mTHeight = mTextSize * CHAR_HEIGHT;                   // Do the math..
  tWidth = mTextSize * CHAR_WIDTH;                      // Pixels across per charactor
  mLineSpace = mTextSize * DEF_LINESPACE;                // Pixles between lines.
  mManager.setWidth(width / tWidth);                    // Solve for number of chars across.
  mNumLines = (height+mLineSpace)/(mTHeight+mLineSpace); // I did the math on scratch paper, we'll see..
  needRefresh = true;                                   // We're going to need to redraw this.

  /*
  Serial.print("Rect width  : ");Serial.println(width);
  Serial.print("Rect height : ");Serial.println(height);
  Serial.print("mTextSize   : ");Serial.println(mTextSize);
  Serial.print("mTHeight    : ");Serial.println(mTHeight);
  Serial.print("tWidth      : ");Serial.println(tWidth);
  Serial.print("mLineSpace   : ");Serial.println(mLineSpace);
  Serial.print("numChars    : ");Serial.println(width/tWidth);
  Serial.print("mNumLines   : ");Serial.println(mNumLines);
  */
}



// Set the size of the text we are dealing with.
void textView::setTextSize(int tSize) {

  mTextSize = tSize;
  calculate();
}

// Well, what about a color? Single = transparent.
void textView::setTextColor(colorObj* tColor) {

  mTextColor.setColor(tColor);
  mTransp = true;
  needRefresh = true;
}


// Well, what about a color? Double = not transparent.
void textView::setTextColors(colorObj* tColor, colorObj* bColor) {

  mTextColor.setColor(tColor);
  mBackColor.setColor(bColor);
  mTransp = false;
  needRefresh = true;
}


void textView::setScroll(scrollCom choice,int inNum) {

  int numLines;
  int newEditLine;

  numLines = mManager.getNumLines();
  if (numLines<=mNumLines) {           // Don't have enough anyway.
    newEditLine = 0;
  } else {
    switch (choice) {
      case topOfText      : newEditLine = 0;                                      break;
      case endOfText      : newEditLine = numLines-mNumLines;                     break;
      case lineAtBottom   : newEditLine = inNum-mNumLines;                        break;
      case lineAtTop      : newEditLine = inNum;                                  break;
      case indexAtBottom  : newEditLine = mManager.getLineNum(inNum) - mNumLines; break;
      case indexAtTop     : newEditLine = mManager.getLineNum(inNum);             break;
      case upOne          : newEditLine = inNum--;                                break;	// These two, possibly backwards
      case downOne        : newEditLine = inNum++;                                break;	// Untested as yet..
      default				  : newEditLine = inNum;											 break;  // Shut up compiler.
    }
    if (newEditLine>=numLines) newEditLine = numLines-1;
    if (newEditLine<0) newEditLine = 0;
  }
  if (newEditLine!=mFirstLine) {
    mFirstLine = newEditLine;
    needRefresh = true;
  }
}

  
// Replace our text buff with a copy of this.
void textView::setText(char* text) {

  mManager.setText(text);
  needRefresh = true;
}


// Add this to the end of our text.
void textView::appendText(char* text) {

  mManager.appendText(text);
  setScroll(endOfText);
  needRefresh = true;
}

void textView::appendText(int inVal) {
	
	snprintf (tool,TOOL_BUFF_SIZE,"%d",inVal);
  	appendText(tool);
}


void textView::appendText(char inChar) {
	
	char buff[2];
	
	if (inChar!='\0') {
		buff[0] = inChar;
		buff[1] = '\0';
		appendText(buff);
	}
}

	
// Stick a NULL terminated substring in at this index.
void textView::insertText(int index, char* text)  {

  mManager.insertText(index, text);
  needRefresh = true;
}


// Remove some text from middle, beginning? End?
void textView::deleteText(int startIndex, int numChars)  {

  mManager.deleteText(startIndex, numChars);
  needRefresh = true;
}


void textView::drawSelf(void) {

  char* textPtr;
  int   locY;
  
  mManager.indexSet(mFirstLine + mNumLines);
  screen->setTextColor(&mTextColor);
  screen->setTextSize(mTextSize);
  screen->setTextWrap(false);
  for (int i = 0; i < mNumLines; i++) {
    textPtr =  mManager.formatLine(i + mFirstLine);
    locY = y + (i * (mTHeight+mLineSpace));
    screen->fillRect(x,locY,width,mTHeight+mLineSpace,&mBackColor);
    screen->setCursor(x,locY);
    screen->drawText(textPtr);
  }
  //screen->drawRect(x,y,width,height,&blue);
}

char* textView::seeText(void) {
  return mManager.mTextBuff;
}
