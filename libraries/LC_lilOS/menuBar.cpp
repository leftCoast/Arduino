#include <menuBar.h>
#include <lilOS.h>
#include <iconButton.h>
#include <resizeBuff.h>

colorObj	menuBarColor(LC_BLACK);

// *****************************************************
// 							 menueBar
// *****************************************************


menuBar::menuBar(panel* inPanel,bool closeBox)
  : drawGroup(0,0,panelWith,MENU_BAR_H) {
    
  stdComBtn*   ourCloseBtn;
  
  mPanel = inPanel;
  if (closeBox) {
    ourCloseBtn = newStdBtn(CLOSE_X,CLOSE_Y,CLOSE_SIZE,closeCmd,mPanel);
    addObj(ourCloseBtn);
  }
}


menuBar::~menuBar(void) {  }



void menuBar::drawSelf(void) { screen->fillRect(this,&menuBarColor); }

