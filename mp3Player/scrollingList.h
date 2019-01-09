#ifndef scrollingList_h
#define scrollingList_h

#include <drawObj.h>

enum scrollType { touchScroll, dialScroll, dSOpenTop, dSOpenBtm, dSOpenBoth, buttonScroll };

enum locType { onList, aboveList, belowList };

class scrollingList : public drawList {

  public:
          scrollingList(int x, int y, word width,word height,scrollType sType,bool clicks=false,bool vertical=true);
  virtual ~scrollingList(void);

          void  setScrollValue(float percent);  // Who do we want to see next?
  virtual void  drawSelf(void);                 // Possibly want to draw? Or not..
  virtual void  offList(void);                  // We off list? If so, here is where you find out.         

          scrollType  mType;
          locType     mLoc;
};


#endif
