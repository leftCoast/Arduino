
#ifndef cellOS_h
#define cellOS_h

#include <colorObj.h>
#include <iconButton.h>
#include <blockFile.h>
#include <timeObj.h>
#include <textView.h>
#include <litlOS.h>
#include "panel.h"
#include "cellListener.h"

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

#define SYS_FILE_PATH           "/SYSTEM/SYSTEM.PRF"

enum  apps { homeApp = HOME_PANEL_ID, phoneApp, textApp, contactApp, calcApp, qGameApp, breakoutApp };

class homeScreen : public homePanel {

  public:
          homeScreen(void);
  virtual ~homeScreen(void);
  
  virtual void  setup(void);
  virtual void  loop(void);
  virtual void  drawSelf(void);

          appIcon*    calcIcon;
          appIcon*    textIcon;
          appIcon*    contactIcon;
          appIcon*    qGameIcon;
          appIcon*    breakoutIcon;
          appIcon*    phoneIcon;
          timeObj     statusTimer;
          textView*   mText;
  
};



class cellOS :  public litlOS {

  public:
          cellOS(void);
  virtual ~cellOS(void);

  virtual int     begin(void);          // The global world is online, do hookups.
          void    initOSFile(void);     // If there is no OS file, stat one from defaults.
  virtual panel*  createPanel(int panelID);
  virtual void    launchPanel(void);    // Dispose of this and launch a newly created panel.
          void    doStatus(void);       // Fill in the status datablock.
          void    hideRedraw(void);
          void    bringUp(void);
  virtual void    idle(void);           // If we need to do something in the background, here we are.

          blockFile*  mFile;
          bool        mDimScreen;
          int         mNowTime;
          int         mEndTime;
          multiMap    screenMap;
          timeObj     mScreenTimer;
};

extern  cellOS  ourOS;


#endif
