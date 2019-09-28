#include "controlPanel.h"
#include <bmpObj.h> 
#include <bmpKeyboard.h> 

// *****************************************************
// ****************   controlPanel   *******************
// *****************************************************


#define LINE_SPACE  20
#define TOP_Y       30
#define LABEL_W     138
#define LABEL_H     18
#define LINE_SPACE  20

#define LIVE_X        LABEL_W+5
#define LIVE_W        90


controlPanel::controlPanel(void) : panel(controlApp) {  }


controlPanel::~controlPanel(void) { }


void controlPanel::setup(void) {

  int traceY;
  int stepY;
  
  traceY = TOP_Y;
  stepY  = LINE_SPACE;

  bmpKeyboard*  theKeybaord = new bmpKeyboard(NULL,true);
  theKeybaord->loadKeys();
  addObj(theKeybaord);
  
  nameText* mNameLabel = new nameText(10,traceY,230,18);
  addObj(mNameLabel);


  traceY = traceY + 2 * stepY;
  bmpObj* limit = new bmpObj(0,traceY,LABEL_W,LABEL_H,LIMIT_BMP);
  limit->begin();
  addObj(limit);

  moistureLimitText*  lText = new moistureLimitText(LIVE_X,traceY,LIVE_W,LABEL_H);
  addObj(lText);
  
  traceY = traceY + stepY;
  bmpObj* waterT = new bmpObj(0,traceY,LABEL_W,LABEL_H,WATER_TIME_BMP);
  waterT->begin();
  addObj(waterT);

  waterTimeText*  wtText = new waterTimeText(LIVE_X,traceY,LIVE_W,LABEL_H,theKeybaord,this);
  wtText->setEventSet(touchLift);
  addObj(wtText);

  traceY = traceY + stepY;
  bmpObj* soakT = new bmpObj(0,traceY,LABEL_W,LABEL_H,SOAK_TIME_BMP);
  soakT->begin();
  addObj(soakT);

  soakTimeText*  sText = new soakTimeText(LIVE_X,traceY,LIVE_W,LABEL_H);
  addObj(sText);

}



void controlPanel::drawSelf(void) {
  screen->fillScreen(&black);
}
