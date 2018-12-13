#ifndef textView_h
#define textView_h


// A rectangle of "flowing" formatted text. You can delete, insert, append,
// and scroll the text and it'll "do the right thing".
//
// We'll see..
//
// For now we'll just do the Adafruit monospaced stuff. If everything works
// out? Maybe this can be upgraded to deal with fancy text.


#include "lists.h"
#include "drawObj.h"


// This is all about numbers. There is actually no link to the actual text.
class lineMarker : public linkListObj {

  public:
          lineMarker(int index,int numChars, bool hardBreak);
  virtual ~lineMarker(void);

          bool  hardBreak(void);                    // Did we end with a hard or soft break?
          int   nextIndex(void);                    // Index of the first charactor after us.
          void  limits(int* index,int* numChars);   // Our text block. Or at least the beginning and length.
          bool  hasIndex(int index);                // Do we contain the text at this index? 
          
          int   mIndex;           // The point in the text where we start.
          int   mNumChars;        // The amount of charactors in our line.
          bool  mHardBreak;       // Were started with a hard break?
};


// This guy has the text and does the math to maintain the line breaks. No links to
// The displaying of the text beyond knowing hom many chars make up a line. And the 
// rule of text flow and formatting.
class lineManager : public linkList {

  public:
          lineManager(void);
  virtual ~lineManager(void);

          void        setWidth(int widthChars);                       // Set how many chars wide the lines are.
          void        setText(char* text);                            // A possibly giant NULL terminated c string.
          void        appendText(char* text);                         // Add this to the end of our text.
          void        insertText(int index,char* text);               // Stick a NULL terminated substring in at this index.
          void        deleteText(int startIndex,int endIndex);        // Remove some text from middle, beginning? End?
          void        indexAll(void);                                 // Index all the text.
          void        indexSet(int startLine,int numLines);           // Index a subset of lines.
          char*       formatLine(int lineNum);                        // And the why of alll this nonsense. Format us a line, this one.
          bool        newMarker(int* index,bool* hardBreak);          // Starting at index, make a new marker. Return true for more text available.
          int         getLineNum(int index);                          // Given an index into the text buffer, find the line number of it. Return -1 if not found.
          void        trimList(int index);                            // Find the line that includes this index. Delete it and its tail.
          
          char*       mTextBuff;                                      // Our string of text to manage.
          int         mWidthChars;
          char*       mOutBuff;
};


// I know I'm a rectangular draw abject. I have a manager object with a 
// list of text lines to keep updated on the screen.
class textView :  public drawObj {

  public:
          textView(int inLocX, int inLocY, word inWidth,word inHeight,bool inClicks=false);
  virtual ~textView(void);

          void        calculate(void);                            // Something changed, repaginate.
          void        setTextSize(int size);                      // Set the size of the text we are dealing with.
          void        setTextColor(colorObj* tColor);             // Well, what about a color?
          void        setFirstLine(int lineNum);                  // Set the first line at the top of our rectangle.
  virtual void        setText(char* text);                        // Replace our text buff with a copy of this.
  virtual void        appendText(char* text);                     // Add this to the end of our text.
  virtual void        insertText(int index,char* text);           // Stick a NULL terminated substring in at this index.
  virtual void        deleteText(int startIndex,int numChars);    // Remove some text from middle, beginning? End?
  
  virtual void        drawSelf(void);                             // Its draw time!

          lineManager mManager;       // The guy that manages the line breaks etc.
          int         mTextSize;
          int         mTHeight;
          int         mNumLines;
          int         mFirstLine;
          colorObj    mTextColor;
};

#endif
