#ifndef fList_h
#define fList_h

#include <Adafruit_GFX.h>


#include <timeObj.h>
#include <bmpLabel.h>
#include <mapper.h>
#include <drawObj.h>
#include "bmpDrawObj.h"
#include "pushPotGUI.h"


#define FL_X    10
#define FL_Y    10
#define FL_H    110
#define FL_W    100

#define FLI_X   0
#define FLI_Y   0
#define FLI_H   12
#define FLI_W   100

#define NUM_FLI FL_H/FLI_H

#define POT_MS  100

void startEditor(void);
void drawBackground(void);
extern colorObj  pencil;

class fListItem;

// First we have the list itself..

class fList : public drawList, public pushPotGUI {

  public:
          fList(void);
  virtual ~fList(void);
        
  virtual void  reset(void);
  virtual void  drawSelf(void);
          void  chooseItem(int index);
  virtual void  doPotVal(int aVal);
  virtual void  gainControl(void);
          void  scroll(int ydist);
          void  makeVisable(fListItem* theItem);
          bool  visible(fListItem* theItem);
          void  currentSongRefresh();
};


// And here the list items..

class marker;
class DBSelector;
class playFile;

class fListItem : public drawGroup, public pushPotGUI {

  public:
          fListItem(char* fileName);
  virtual ~fListItem(void);

  virtual void      setFocus(bool onOff);
  virtual void      gainControl(void);
  virtual void      doAction(void); 
  virtual void      doPotVal(int aVal);         
  virtual bool      wantRefresh(void);
  virtual void      drawSelf(void);

          bmpLabel*   mText;
          marker*     mMarker;
          DBSelector* mSelector;
          playFile*   mPlayFile;
};


class marker : public bmpDrawObj {

  public:

          marker(int x,int y, word width, word height);
  virtual ~marker(void);
                
          void  setColor(colorObj* aColor);
  virtual void  doAction(void); 
  virtual void  drawSelf(void);

          colorObj  mColor;
          bool      pointIn;
};


class playFile : public bmpDrawObj {
  
  public:
  
          playFile(label* inLabel);
  virtual ~playFile(void);
  
  virtual void  doAction(void);
  virtual void  drawSelf(void);

          label*  mText;
};


class DBSelector : public bmpDrawObj {
  
  public:
  
          DBSelector(label* inLabel);
  virtual ~DBSelector(void);
  
  virtual void  doAction(void);
  virtual void  drawSelf(void);

          label*  mText;
};


#endif
