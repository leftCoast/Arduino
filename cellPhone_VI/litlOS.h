#ifndef ltlOS_h
#define ltlOS_h

#include <colorObj.h>
#include "panel.h"

// If we'd like to have an overall look. Or at least a default look.

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


class homePanel : public panel {

  public:
          homePanel(blockFile* inFile,unsigned long rootID);
  virtual ~homePanel(void);
  
  virtual void  drawSelf(void);
  
};


class litlOS :  public idler {

  public:
          litlOS(void);
  virtual ~litlOS(void);

          void  begin(void);                    // The global world is online, do hookups.
          void  initOSFile(void);               // If there is no OS file, stat one from defaults.
          void  launchPanel(panel* newPanel);   // Launch a newly created panel.
          void  doLoop(void);                   // Tell the current panel its loop time.
  virtual void  idle(void);                     // If we need to do something in the background, here we are.

          blockFile*  mFile;
          homePanel*  mSysPanel;
};



#endif
