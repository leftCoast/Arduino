
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
#include "src/cellListener/cellListener.h"
#include "icons.h"

// If we'd like to have an overall look. Or at least a default look.
#define SCREEN_PIN    25

#define SYS_FILE_PATH       "/SYSTEM/SYSTEM.PRF"
//#define IMAGE_FILE_PATH     "/SYSTEM/IMAGES/TGIVE.BMP"
#define IMAGE_FILE_PATH     "/SYSTEM/IMAGES/TEAMRKT.BMP"
#define CONTACT_FILE_PATH   "/SYSTEM/CONTACTS"

#define ICON_PATH_X22       "/system/icons/redX22.bmp"
#define ICON_PATH_NEW22     "/system/icons/new22.bmp"
#define ICON_PATH_SEARCH22  "/system/icons/search22.bmp"
#define ICON_PATH_TEXT22    "/system/icons/text22.bmp"
#define ICON_PATH_PHONE22   "/system/icons/call22.bmp"
#define ICON_PATH_TRASH22   "/system/icons/trash22.bmp"


#define MENU_BAR_H    24  // Because we have 22x22 icos to stick on it. So there!

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


// *****************************************************
// *****************   std buttons  ********************
// *****************************************************


class closeBtn : public iconButton {

  public:
          closeBtn(int x,int y);
  virtual ~closeBtn(void);
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


class textBtn : public iconButton {

  public:
          textBtn(int x,int y);
  virtual ~textBtn(void);
};


class callBtn : public iconButton {

  public:
          callBtn(int x,int y);
  virtual ~callBtn(void);
};


class trashBtn : public iconButton {

  public:
          trashBtn(int x,int y);
  virtual ~trashBtn(void);
};


// *****************************************************
// ******************   homeScreen  ********************
// *****************************************************



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
          void    initOSFile(void);     // If there is no OS file, start one from defaults.
  virtual panel*  createPanel(int panelID);
  virtual void    launchPanel(void);    // Dispose of current and launch a newly created panel.
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