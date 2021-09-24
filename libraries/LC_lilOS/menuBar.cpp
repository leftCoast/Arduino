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
  : iconButton(CLOSE_X,CLOSE_Y,inPanel->mOSPtr->stdIconPath(x22),22) {
  
  mPanel = inPanel;
  begin();
 }


closeBtn::~closeBtn(void) {  }


void closeBtn::doAction(void) { mPanel->close(); }



// ******


newBtn::newBtn(panel* inPanel,int x,int y)
  : iconButton(x,y,inPanel->mOSPtr->stdIconPath(cross22),22)  {
  
  mPanel = inPanel;
  begin();
}

   
newBtn::~newBtn(void) {  }



// *****


searchBtn::searchBtn(panel* inPanel,int x,int y)
  : iconButton(x,y,inPanel->mOSPtr->stdIconPath(search22),22)  {
  
  mPanel = inPanel;
  begin();
}
 
    
searchBtn::~searchBtn(void) {  }



// *****


trashBtn::trashBtn(panel* inPanel,int x,int y)
  : iconButton(x,y,inPanel->mOSPtr->stdIconPath(trashC22),22)  {
  
  mPanel = inPanel;
  begin();
}


trashBtn::~trashBtn(void) {  }


