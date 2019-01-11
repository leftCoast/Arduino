
#ifndef cellOS_h
#define cellOS_h

#include <colorObj.h>
#include <iconButton.h>
#include <blockFile.h>
#include <timeObj.h>
#include <textView.h>
#include <bmpPipe.h>
#include <litlOS.h>
#include "panel.h"
#include "cellListener.h"
#include "icons.h"

// If we'd like to have an overall look. Or at least a default look.
#define SCREEN_PIN    25

#define SYS_FILE_PATH "/SYSTEM/SYSTEM.PRF"
#define IMAGE_FILE_PATH "/SYSTEM/IMAGES/TGIVE.BMP"

#define MENU_BAR_H    20

// Starting points, they are tweaked in setup().
extern colorObj  backColor;
extern colorObj  textColor;
extern colorObj  textSelectColor;
extern colorObj  textActiveColor;
extern colorObj  editFieldBColor;

extern colorObj  lightbButtonColor;
extern colorObj  lightbButtonHighlight;
extern colorObj  lightbButtonColorHit;
extern colorObj  darkButtonColor;
extern colorObj  darkButtonColorHit;

extern colorObj  redButtonColor;
extern colorObj  redButtonHighlight;
extern colorObj  greenbuttonColor;
extern colorObj  greenButtonHighlight;

extern colorObj battLineColor;
extern colorObj menuBarColor;

enum  apps { homeApp = HOME_PANEL_ID, phoneApp, textApp, contactApp, calcApp, qGameApp, breakoutApp };

class homeScreen : public homePanel {

  public:
          homeScreen(void);
  virtual ~homeScreen(void);
  
  virtual void  setup(void);
  virtual void  loop(void);
          void  showStatus(void);
  virtual void  drawSelf(void);

          bmpPipe*    mBackImage;
          appIcon*    calcIcon;
          appIcon*    textIcon;
          appIcon*    contactIcon;
          appIcon*    qGameIcon;
          appIcon*    breakoutIcon;
          appIcon*    phoneIcon;
          timeObj     statusTimer;
          //textView*   mText;
          battPercent*  mBatPct;
          RSSIicon*     mRSSI;
  
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
