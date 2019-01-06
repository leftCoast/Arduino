/*
#ifndef fListItem_h
#define fListItem_h

#include <drawObj.h>

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


#endif
*/
