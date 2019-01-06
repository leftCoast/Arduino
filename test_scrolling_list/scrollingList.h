#ifndef scrollingList_h
#define scrollingList_h

#include <drawObj.h>

enum scrollType { touchScroll, dialScroll, buttonScroll };

class scrollingList : public drawList {

  public:
          scrollingList(int x, int y, word width,word height,scrollType sType,bool clicks=false);
  virtual ~scrollingList(void);

          void  setScrollValue(float percent);
  virtual void  drawSelf(void);

          scrollType  mType;
};


#endif
