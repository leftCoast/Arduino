#ifndef listView_h
#define listView_h

#include <drawObj.h>

enum scrollCommand { scrollUp, pageUp, toTop, scrollDn, pageDn, toBottom,
scrollRight, pageRight, toRight, scrollLeft, pageLeft, toLeft };


class listView : public drawObj, public viewMgr {

  public:
  listView(word inX,word inY,word width,word inRowHeight,word inRows);

  virtual void      addObj(drawObj* newObj);
          drawObj*  findObj(word index);
  virtual void      drawSelf(void);
  virtual void      idle(void);
  
  virtual void    scrollList(scrollCommand command);

          word        rowHeight;
          word        numRows;
          word        currentRow;
          word        totalRows;
          word        colWidth;
          word        numCols;
          word        currentCol;
          word        totalCols;
};


class scrollControl : public drawObj {

  public:
  scrollControl(word inX,word inY,word width, word height, listView* inList, scrollCommand inCommand);
  
  virtual void    drawSelf(void);
  virtual void    doAction(void);

          listView*     ourList;
          scrollCommand command;
};


#endif

