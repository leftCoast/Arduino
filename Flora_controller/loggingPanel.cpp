#include "loggingPanel.h"
#include <IOandKeys.h>
#include <bmpObj.h>
#include "floraOS.h"
#include "bmpKeyboard.h"
#include "flasher.h"

#define TB_X    10
#define TB_Y    28
#define TB_W    220
#define TB_H    118

#define TF_INSET  2
#define TF_X      TB_X+TF_INSET
#define TF_Y      TB_Y+TF_INSET
#define TF_W      TB_W-(2*TF_INSET)
#define TF_H      TB_H-(2*TF_INSET)

#define EB_X    TB_X
#define EB_Y    TB_Y+TB_H+14
#define EB_W    TB_W
#define EB_H    22

#define ET_INSET  2
#define ET_X      EB_X+ET_INSET
#define ET_Y      EB_Y+4
#define ET_W      EB_W-(2*ET_INSET)
#define ET_H      10

#define out  theTextField->appendText

loggingPanel::loggingPanel(void)
  : panel(loggingApp) { }


loggingPanel::~loggingPanel(void) { }
          
void loggingPanel::setup(void) {

  bmpFlasher* loggingButton = new bmpFlasher(30,30,22,22,"/icons/bluon22.bmp","/icons/bluoff22.bmp");
  loggingButton->setTimes(500,1000);  
  loggingButton->setBlink(true);
  //addObj(loggingButton);

  drawObj* editBox = new drawObj(EB_X,EB_Y,EB_W,EB_H);
  addObj(editBox);

  /*
  label*  theEditLabel = new label(ET_X,ET_Y,ET_W,ET_H,"",2);
  theEditLabel->setColors(&blue,&green);
  theEditLabel->setJustify(TEXT_CENTER);
  theEditLabel->setValue("12345678901234");
  addObj(theEditLabel);
  */
  
  editLabel*  theEditLabel = new editLabel(ET_X,ET_Y,ET_W,ET_H,"",2);
  theEditLabel->setColors(&white,&black);
  theEditLabel->setJustify(TEXT_CENTER);
  theEditLabel->setValue("1234567890123");
  addObj(theEditLabel);

  
  //textView*   theTextField = new textView(TF_X,TF_Y,TF_W,TF_H);
  //theTextField->setTextColors(&white,&black);
  //out("Can you see this?");
  //addObj(theTextField);
  
  bmpKeyboard*  theKeybaord = new bmpKeyboard(theEditLabel,true);
  theKeybaord->loadKeys();
  addObj(theKeybaord);
  
  theEditLabel->beginEditing();
}


void loggingPanel::drawSelf(void) { screen->fillScreen(&black); }
