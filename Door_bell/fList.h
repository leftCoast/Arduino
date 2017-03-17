#ifndef fList_h
#define fList_h

#include <Adafruit_GFX.h>
//#include <Fonts/FreeSerif9pt7b.h>

#include <bmpLabel.h>
#include <mapper.h>
#include <drawObj.h>
#include "bmpDrawObj.h"

#define FL_X    10
#define FL_Y    10
#define FL_H    110
#define FL_W    100

#define FLI_X   0
#define FLI_Y   0
#define FLI_H   12
#define FLI_W   100

#define NUM_FLI FL_H/FLI_H


void startEditor(void);
void buttonClick(void);
void potVal(int aVal);


class fListItem;

// First we have the list itself..

class fList : public drawList {

  public:
              fList(void);

      void    setHighlite(int index);
      void    potVal(int aVal);
      void    buttonClick(void);
      void    selectFile(void);
      void    setupMapper(void);
      void    scroll(int ydist);
      void    makeVisable(fListItem* theItem);
      bool    visible(fListItem* theItem);
virtual void  drawSelf(void);

      mapper  listMapper;
      int     savedHighlight;
};


// And here the list items..

class marker;
class DBSelector;

class fListItem : public drawGroup {

  public:
                    fListItem(char* fileName);
                    ~fListItem(void);

          void      highlite(bool onOff);       // Show we are the choosen one!
  virtual void      doAction(void);             // For starters, this will be to play the file.
          void      setGoupRefresh(void);
  virtual boolean   wantRefresh(void);

          bmpLabel*   mText;
          marker*     mMarker;
          DBSelector* mSelector;
          bool        choosen;
};


class marker : public drawObj {

  public:

                 marker(int x,int y, word width, word height);

          void  setColor(colorObj* aColor);
  virtual void  drawSelf(void);

          colorObj  mColor;
};


class DBSelector : public bmpDrawObj {
  
  public:
  
  DBSelector(label* inLabel);

          void  select(void);
  virtual void  drawSelf(void);

          label*  mText;
};


#endif
