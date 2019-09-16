#include <menuBar.h>
#include <panel.h>
#include <iconButton.h>

colorObj	menuBarColor;
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
  : iconButton(CLOSE_X,CLOSE_Y,ICON_PATH_X22,22) {
  
  mPanel = inPanel;
  begin();
 }


closeBtn::~closeBtn(void) {  }


void closeBtn::doAction(void) { mPanel->close(); }



// ******


newBtn::newBtn(int x,int y)
  : iconButton(x,y,ICON_PATH_NEW22,22)  { begin(); }

   
newBtn::~newBtn(void) {  }



// *****


searchBtn::searchBtn(int x,int y)
  : iconButton(x,y,ICON_PATH_SEARCH22,22)  { begin(); }
 
    
searchBtn::~searchBtn(void) {  }



// *****


trashBtn::trashBtn(int x,int y)
  : iconButton(x,y,ICON_PATH_TRASH22,22)  { begin(); }


trashBtn::~trashBtn(void) {  }


