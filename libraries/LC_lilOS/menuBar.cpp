#include <menuBar.h>
#include <lilOS.h>
#include <iconButton.h>
#include <resizeBuff.h>

colorObj	menuBarColor(LC_BLACK);
// *****************************************************
// 							 menueBar
// *****************************************************


menuBar::menuBar(panel* inPanel,bool closeBox)
  : drawGroup(0,0,PANEL_WIDTH,MENU_BAR_H) {
    
  closeBtn*   ourCloseBtn;
  
  mPanel = inPanel;
  if (closeBox) {
    ourCloseBtn = new closeBtn(mPanel);
    addObj(ourCloseBtn);
  }
}


menuBar::~menuBar(void) {  }



void menuBar::drawSelf(void) { screen->fillRect(this,&menuBarColor); }



// *****************************************************
// 							std buttons
// *****************************************************


closeBtn::closeBtn(panel* inPanel)
  : iconButton(CLOSE_X,CLOSE_Y,ICON_NAME_X22,22) {
  
  mPanel = inPanel;
  begin();
 }


closeBtn::~closeBtn(void) {  }


void closeBtn::doAction(void) { mPanel->close(); }



// ******


newBtn::newBtn(int x,int y)
  : iconButton(x,y,ICON_NAME_NEW22,22)  { begin(); }

   
newBtn::~newBtn(void) {  }



// *****


searchBtn::searchBtn(int x,int y)
  : iconButton(x,y,ICON_NAME_SEARCH22,22)  { begin(); }
 
    
searchBtn::~searchBtn(void) {  }



// *****


trashBtn::trashBtn(int x,int y)
  : iconButton(x,y,ICON_NAME_TRASH22,22)  { begin(); }


trashBtn::~trashBtn(void) {  }


