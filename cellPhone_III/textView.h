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


class lineMarker : public linkListObj {

  public:
          lineMarker(index,bool hardBreak);
  virtual ~lineMarker(void);

          int   mIndex;
          bool  hardBreak;
};


class textView :  public drawObj,
                  public linkList {

  public:
          textEdit(int inLocX, int inLocY, word inWidth,word inHeight,bool inClicks=false);
  virtual ~textEdit(vid);

          void  indexAll(void);                                   // Index all the text.
          void  setStart(lineMarker* marker);                     // Starting point for our editing window.
          void  indexFrame();                                     // Run an index for our frame.   
          
          lineMarker  firstIndex(void);                           // Return our first editing line.
          lineMarker  lastIndex(void);                            // Return our last editing line.
          lineMarker  getLine(int index);                         // Find the line this index maps to. NULL means out of indexed area.

          int         getNumLines(void);                          // How many lines did we end up with?
          int         getLineWidth(int line);                     // How many charactors in a line?

  virtual void        setText(char* text);                        // Replace our text buff with a copy of this.
  virtual void        appendText(char* text);                     // Add this to the end of our text.
  virtual void        insertText(int index,char* text);           // Stick a substring in at this index.
  virtual void        deleteText(int startIndex,int endIndex);    // Remove some text from middle, beginning? End?
          
  virtual void        drawSelf(void);
  
          char* mTextBuff;      // Our string of text to display.
          int   mTHeight;       // Height in pixals for our font.
          int   mTWidth;        // Wifdth, in pixels of our font. (monospaced)
          int   mLineSpace;     // Pixels between lines.
};

#endif
