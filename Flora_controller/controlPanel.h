#ifndef controlPanel_h
#define controlPanel_h

#include "floraOS.h"
#include "label.h"
#include "panel.h"
#include "tools.h"
#include <colorRect.h>
#include <blinker.h>
#include <bmpPipe.h>
#include <fontLabel.h>

class controlPanel;


// *****************************************************
//                    controlPanel 
// *****************************************************

class controlPanel : public panel {

  public:
          controlPanel(void);
  virtual ~controlPanel(void);
          
  virtual void  setup(void);
          void  doComSetDryLimit(float limit);
          void  doComSetWTime(float wTime);
          void  doComSetSTime(float sTime);
  virtual void  drawSelf(void);

          //selectBtn*    mDryLimitBtn;
          //selectBtn*    mWaterTimeBtn;
          //selectBtn*    mSoakTimeBtn;
};

#endif
