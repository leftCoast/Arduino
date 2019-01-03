#ifndef ltlOS_h
#define ltlOS_h

#include <colorObj.h>
#include <iconButton.h>
#include "panel.h"
#include "textview.h"

// If we'd like to have an overall look. Or at least a default look.
#define SCREEN_PIN    25

#define SYS_TEDIT_BCOLOR        LC_WHITE
#define SYS_TEDIT_TCOLOR        LC_BLACK
#define SYS_TEDIT_HCOLOR        LC_LIGHT_BLUE

#define SYS_PANEL_COLOR         LC_GREY
#define SYS_INPUT_BTN_COLOR     LC_LIGHT_GREY
#define SYS_CONTROL_BTN_COLOR   LC_DARK_GREY
#define SYS_CONTROL_LBL_COLOR   LC_BLACK
#define SYS_SHADOW_COLOR        LC_CHARCOAL

#define SYS_MENU_PIX            16  // How many pixles rserved for the menu?
#define SYS_FILE_PATH           "/SYSTEM/SYSTEM.PRF"

//#define out logView.appendText
//#define outln logView.appendText("\n")

//extern textView    logView;


enum        apps { noPanel, homeApp, phoneApp, textApp, contactApp, calcApp, qGameApp, breakoutApp };


class appIcon : public iconButton {
  
  public:
          appIcon(int xLoc,int yLoc,apps message,char* path);
  virtual ~appIcon(void);
  
  virtual void  doAction(void);

          apps  mMessage;
};



class homePanel : public panel {

  public:
          homePanel(void);
  virtual ~homePanel(void);
  
  virtual void  setup(void);
  virtual void  loop(void);
  virtual void  drawSelf(void);

          appIcon*    calcIcon;
          appIcon*    textIcon;
          appIcon*    contactIcon;
          appIcon*    qGameIcon;
          appIcon*    breakoutIcon;
          appIcon*    phoneIcon;
  
};



class litlOS :  public idler {

  public:
          litlOS(void);
  virtual ~litlOS(void);

          void  begin(void);          // The global world is online, do hookups.
          void  initOSFile(void);     // If there is no OS file, stat one from defaults.
          void  launchPanel(void);    // Dispose of this and launch a newly created panel.
          void  loop(void);           // Tell the current panel its loop time.
          void  doStatus(void);       // Fill in the status datablock.
          void  hideRedraw(void);
          void  bringUp(void);
  virtual void  idle(void);           // If we need to do something in the background, here we are.

          bool        mDimScreen;
          int         mNowTime;
          int         mEndTime;
          multiMap    screenMap;
          timeObj     mScreenTimer;
          blockFile*  mFile;
          panel*      mPanel;
};


#endif
