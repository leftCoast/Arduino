#ifndef menuBar_h
#define menuBar_h

#include <drawObj.h>
#include <panel.h>
#include <iconButton.h>

#define MENU_BAR_H    24  // Because we have 22x22 icos to stick on it. So there!

#define ICON_NAME_X22			"/system/icons/standard/x22.bmp"
#define ICON_NAME_NEW22			"/system/icons/standard/cross22.bmp"
#define ICON_NAME_SEARCH22		"/system/icons/standard/search22.bmp"
#define ICON_NAME_TRASH22		"/system/icons/standard/trashC22.bmp"

#define PATH_BUFF_CHARS			128
	
#define CLOSE_X         0
#define CLOSE_Y         1
#define CLOSE_W         22
#define CLOSE_H         22

extern colorObj	menuBarColor;

	
// *****************************************************
//								menueBar
// *****************************************************

class panel;

class menuBar  : public drawGroup {

  public:
            menuBar(panel* inPanel,bool closeBox=true);
    virtual ~menuBar(void);

    virtual void    drawSelf(void);

            panel*  mPanel;
};



// *****************************************************
// 						std buttons
// *****************************************************

// these are the little 22x22 pix buttons that do functions like make a call,
// text someone, new document, delete current document. An entire grab bag of functons.


class closeBtn : public iconButton {

  public:
          closeBtn(panel* inPanel);
  virtual ~closeBtn(void);
  
  virtual void  doAction(void);
  
          panel* mPanel;
};


class newBtn : public iconButton {

  public:
          newBtn(int x,int y);
  virtual ~newBtn(void);
};


class searchBtn : public iconButton {

  public:
          searchBtn(int x,int y);
  virtual ~searchBtn(void);
};


class trashBtn : public iconButton {

  public:
          trashBtn(int x,int y);
  virtual ~trashBtn(void);
};


#endif