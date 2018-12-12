#include "textEdit.h"         
#include  <label.h>

#define DEF_LINESPACE   2   // 2 Its a multiplyer. Ex: 2 * text size.



// ********************************************************************************
// ********************************************************************************
// lineMarker hold markrs for line breaks in a text block.
// ********************************************************************************
// ********************************************************************************


lineMarker::lineMarker(int index,int numChars, bool hardBreak) {

  mIndex = index;
  mNumChars = numChars;
  mHardBreak = hardBreak;
}


lineMarker::~lineMarker(void) {  }


// Did we end with a hard or soft break?
bool lineMarker::hardBreak(void) { return mHardBreak; }


// Index of the first charactor after us.
int lineMarker::nextIndex(void) { return mIndex+mNumChars; }


// Our text block. Or at least the beginning and length.
void  lineMarker::limits(int* index,int* numChars) {

  index* = mIndex;
  numChars* = mNumChars;
}



// ********************************************************************************
// ********************************************************************************
// lineManager uses lineMarkers to manage line breaks in a text block.
// ********************************************************************************
// ********************************************************************************


lineManager::lineManager(void) {

  mTextBuff = NULL;
  mWidthChars = 0;
  mList = NULL;
}


lineManager::~lineManager(void) {

  if (mTextBuff) free(mTextBuff);
  if (mList) dumpList();
}


// Set how many chars wide the lines are.
void lineManager::setWidth(int widthChars) {

  dumpList();
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
  dblLinkListObj* lastLine;
  
  if(text) {                                    // Sanity, did they pass us a NULL?
    if (text[0]!='\0') {                        // I don't like the look of these people. Did they pass us an empty string?
      numNewChars = strlen(text);               // save off how many charactors we have.
      numOurChars = strlen(mTextBuff);          // save off how many charactors they gave us.
      totalChars = numOurChars + numNewChars;   // Figure total buffer length.
      buff = (char*)malloc(totalChars+1);       // Allocate the buffer.
      if (buff) {                               // If we got one.
        strcpy(buff,mTextBuff);                 // Copy over our text.
        strcpy(&buff[numOurChars],text);        // Then their text. (on top of the last '\0')
        free(mTextBuff);                        // loose our old text buffer.
        mTextBuff = buff;                       // Point to this shiny new one.
        trimList(numOurChars-1);                // Trim outdated line.
      }
    }
  }
}


// Stick a NULL terminated substring in at this index.
void lineManager::insertText(int index,char* text) {
              
  char*           buff;
  int             numNewChars;
  int             numOurChars;
  int             totalChars;
  dblLinkListObj* theLine;
  
  if(text) {                                        // Sanity, did they pass us a NULL?
    if (text[0]!='\0') {                            // I don't like the look of these people. Did they pass us an empty string?
      numNewChars = strlen(text);                   // save off how many charactors we have.
      numOurChars = strlen(mTextBuff);              // save off how many charactors they gave us.
      totalChars = numOurChars + numNewChars;       // Figure total buffer length.
      buff = (char*)malloc(totalChars+1);           // Allocate the buffer.
      if (buff) {                                   // If we got one.
        for(int i=0;i<index;i++) {                  // Copy over the first piece of our text.
          buff[i] = mTextBuff[i];
        }                
        strcpy(&buff[i],text);                      // Then their text.
        strcpy(&buff[i+numNewChars],&mTextBuff[i]); // And the rest of our text.
        free(mTextBuff);                            // loose our old text buffer.
        mTextBuff = buff;                           // Point to this shiny new one.
        trimList(index);                            // Trim outdated lines.                                          
      }
    }
  }
}


// Remove some text from middle, beginning? End?
void lineManager::deleteText(int startIndex,int numChars) {

  int   textLen;
  int   endIndex;
  char* buff;
  int   i;
  int   j;
  
  if (mTextBuff) {
    textLen = strlen(mTextBuff);
    if(startIndex<textLen) {
      trimList(startIndex);
      endIndex = startIndex+numChars;
      if (endIndex>=textLen) {
        mTextBuff[startIndex]='\0';
        buff = malloc(startIndex+1);
        strcpy(buff,mTextBuff);
        free(mTextBuff);
        mTextBuff = buff;
      } else {
        while(mTextBuff[endIndex]!='\0') {
          mTextBuff[startIndex]=mTextBuff[endIndex];
          startIndex++;
          endIndex++;
        }
        mTextBuff[startIndex]='\0';
        textLen = strlen(mTextBuff);
        buff = malloc(textLen+1);
        strcpy(buff,mTextBuff);
        free(mTextBuff);
        mTextBuff = buff;
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
void lineManager::indexSet(int startLineNum,int numLines) {

  int         endLineNum;
  int         lineCount;
  int         index;
  bool        hardBreak;
  bool        endOfText;
  lineMarker* temp;

  if (mTextBuff) {                                  // We do have text right?
    if (strlen(mTextBuff)>0) {                      // More than just a null charactor?
      endLineNum = startLineNum + numLines-1;       // Just in case..
      lineCount = getCount();                       // Like arrays, zero based.
      if (endLineNum<lineCount-1)  *********GETTING BEFUDDLED!! START HERE TOMORROW!!**************
      index = startLine->mIndex;                    // Pull out our starting index;
      if (startLine->dllPrev) {                     // See if its the first on the list.
        temp = (lineMarker*)startLine->dllPrev;     // Not first, grab previous.
        hardBreak = temp->hardBreak();              // Find if it ended in a hard break or not.
      } else {                                      // It was the first.
        hardBreak = true;                           // First always starts with a hard break.
      }
      trimList(startLine);                          // Starting with the passed in line, trim the list. (Passed in pointer is no longer valid!)
      endOfText = false;                            // We know there is some. startLine had some.
      while(!endOfText && numLines) {               // Loop 'till we run out of text or lines they wanted.
        endOfText = newMarker(&index,&hardBreak);
        numLines--;
      }
    }
  }
}


// Starting at index, make a new marker. Return true for more text available.
bool lineManager::newMarker(int* index,bool* hardBreak);          

  int         i;
  lineMarker* newLine;
  int         lastSpace;
  bool        seenPrint;
  
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
    } else if (i==mNumChars-1) {                                        // We didn't see an EOL or NULL char, but we are out of room.
      if (isspace(mTextBuff[i]) {                                       // Are we looking at whitespace?
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
      if (isspace(mTextBuff[i]) {                                       // Are we pointing at a white space?                                 
        lastSpace = i;                                                  // Keep track of the last space we saw.
      }
      i++;                                                              // Next!
    }
  }
}


void  lineManager::trimList(int index) {

  lineMarker* theLine;
  
  theLine = mList->getLine(index);  // Grab the line that changed.
  if (theLine) {                    // We get it?
    theLine->deleteTail();          // Delete its tail.
    delete (lastLine);              // And itself.
  }
}


 // And there are times it would be nice to grab one by index.
// Like an array. Returns NULL if not found.
lineMarker* lineManager::getByIndex(int index) {

  lineMarker* trace;
  
  if (index>=0) {             // Sanity, may be a Bozo calling.
    trace = mList;
    while(trace && index) {
      trace = (lineMarker*)trace->dllNext;
      index--;
    }
    return trace;
  }
  return NULL;
}


// How many items are in my tail?
int lineManager::getCount(void) {

  lineMarker* trace;
  int         count;
  
  count = 0;
  trace = mList;
  while(trace) {
    trace = (lineMarker*)trace->dllNext();
    count++;
  }
  return trace;
}


           
// ********************************************************************************
// ********************************************************************************
// textEdit pulls all these bits together to manage text in a rectangle.
// ********************************************************************************
// ********************************************************************************


textEdit::textEdit(int inLocX, int inLocY, word inWidth,word inHeight,bool inClicks)
  : drawObj(inLocX,inLocY,inWidth,inHeight,inClicks) {

   mTextBuff = NULL;
   mStartLine = NULL;
   setTextSize(DEF_TEXT_SIZE);
}















// Want to display this line? Draw these.
bool ???::printText(char* theText,char* buff) {

  int index;
  int iOut;
  
  index = mIndex;
  if (!mHardBreak) {
    while(theText[index]=='\ ' && index<numChars) {
      index++;
    }
  }
  iOut = 0;
  while(ndex<=numChars) {
    buff[iOut] = theText[index];
    iOut++;
    index++;
  }
}


textEdit::~textEdit(vid) {

  if (mTextBuff) {
    free(mTextBuff);
  }
}

// Something's changed, dump the old list.
textEdit::dumpList() {

  linkList::dumpList();
  mStartLine = NULL;
}


// Set the size of the text we are dealing with.
void textEdit::setTextSize(int size) {

  mTHeight = size * CHAR_HEIGHT;      // Do the math..
  mTWidth = size * CHAR_WIDTH;
  mLineSpace = size * DEF_LINESPACE;
  indexAll();                         // Changed everything recalculaate.
  needRefresh = true;                 // We're going to need to redraw this.
}


/


// Index all the text.
void textEdit::indexAll(void) {

  int         index;
  bool        hardBreak;
  bool        endOfText;
  
  dumpList();                                   // No matter what, this list is toast.
  if (mTextBuff) {                              // We have anything? Otherwise don't bother.
    mNumChars = getLineWidth();                 // Always assume it changed. 'Cause it might, we can't control this.
    index = 0;                                  // We're doing it all, start at zero.
    hardBreak = true;                           // The first line marker always starts with a hard break.
    endOfText = false;
    while(!endOfText) {                         // Not seen the NULL charactor yet.
      endOfText = newMarker(&index,&hardBreak); // Read and add the new line marker. 
    }
  }                              
}


// Index a subset of lines. (Ignores previous lines, removes subsequent lines.)
void textEdit::indexFrom(lineMarker* startLine,int numLines) {

  int         index;
  bool        hardBreak;
  bool        endOfText;
  
  if (mTextBuff && numLines>0) {                  // We have anything? Otherwise don't bother.
    if (mNumChars==getLineWidth()) {              // If width has not changed, otherwise this exercise is pointless.
      startLine->deleteTail();                    // Ok, loose your tail..
      index =  = startLine->nextIndex();          // We're doing it from here.
      hardBreak = startLine->hardBreak;           // Starting with a hard break or not.
      endOfText = startLine->endOfText();         // Is there more to come?
      while(!endOfText && numLines) {             // When we are pointing at the NULL charactor, we're done.
        endOfText = newMarker(&index,&hardBreak); // Read and add the new line marker.
        numLines--;
      }
    }
  }                              
}


// Starting point for our editing window.
void textEdit::setStart(lineMarker* marker) { mStartLine = marker; }


// Run an index for just our frame.                   
void textEdit::indexFrame() {

                                        
}
          
          lineMarker*  firstIndex(void);                          // Return our first editing line.
          lineMarker*  lastIndex(void);                           // Return our last editing line.
          lineMarker*  getLine(int index);                        // Find the line this index maps to. NULL means out of indexed area.

          int         getNumLines(void);                          // How many lines did we end up with?
          int         getLineWidth(int line);                     // How many charactors in a line?

  virtual void        setText(char* text);                        // Replace our text buff with a copy of this.
  virtual void        appendText(char* text);                     // Add this to the end of our text.
  virtual void        insertText(int index,char* text);           // Stick a NULL terminated substring in at this index.
  virtual void        deleteText(int startIndex,int endIndex);    // Remove some text from middle, beginning? End?
          
  virtual void        drawSelf(void);
  
         
