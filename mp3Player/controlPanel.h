#ifndef controlPanel_h
#define controlPanel_h

#include <drawObj.h>
#include <runningAvg.h>
#include <mapper.h>

#include "pushPotGUI.h"
#include "scrollingList.h"



// ************************************
// ************ contPnlItem ***********
// ************************************


enum CPIType { randPlayOn, randPlayOff }; // Can't think of any others now..

class contPnlItem : public drawObj {

public:
          contPnlItem(drawList* myList,CPIType myType);
  virtual ~contPnlItem(void);

  virtual void  drawSelf(void);
  virtual void  doAction(void);
  
          drawList* mDrawList;
          CPIType   mCPIType;
};



// ************************************
// *********** controlPanel ***********
// ************************************


class controlPanel :  public scrollingList,
                      public pushPotGUI {

  public:
          controlPanel(int x, int y, word width,word height);
  virtual ~controlPanel(void);

  virtual void    drawSelf(void);   
  virtual void    doPotVal(int aVal);
  virtual void    reset(void);
  virtual void    offList(void);

          runningAvg*   potSmoother;  
          mapper*       potToControl;
          contPnlItem*  mOnItem;
          contPnlItem*  mOffItem;
};

#endif
