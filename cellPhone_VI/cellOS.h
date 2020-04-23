
#ifndef cellOS_h
#define cellOS_h

#include <colorObj.h>
#include <iconButton.h>
#include <blockFile.h>
#include <timeObj.h>
#include <textView.h>
#include <bmpPipe.h>
#include <colorRect.h>
#include <keystroke.h>
#include <scrKeyboard.h>
#include <editLabel.h>
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

#define ICON_PATH_CONTACT22  "/system/icons/cPhone/cont22.bmp"
#define ICON_PATH_TEXT22    "/system/icons/cPhone/msg22.bmp"
#define ICON_PATH_PHONE22   "/system/icons/cPhone/phone22.bmp"

#define STATUS_X        200
#define STATUS_Y        2
#define STATUS_W        40
#define STATUS_H        22   

//#define SIG_X           15

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

enum  apps { homeApp = HOME_PANEL_ID, phoneApp, textApp, contactApp, calcApp, qGameApp, toolsApp, breakoutApp };



// *****************************************************
//                      cellOSPanel
// *****************************************************

// We make our own panel that adds a battery & rssi thing to the end of each menu.
class cellOSPanel : public panel {

  public:
            cellOSPanel(int panelID,menuBarChoices menuBarChoice=closeBoxMenuBar,eventSet inEventSet=noEvents);
 virtual    ~cellOSPanel(void);
};


// *****************************************************
//                        buttons
// *****************************************************


class contactsBtn : public iconButton {

  public:
          contactsBtn(int x,int y);
  virtual ~contactsBtn(void);

  virtual void  doAction(void);
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



// *****************************************************
//                      statusIcon
// *****************************************************



class statusIcon  : public drawGroup,
                    public timeObj {

  public:
            statusIcon(void);
    virtual ~statusIcon(void);
    
    virtual void  setStatus(void);
    virtual void  idle(void);
    virtual void  drawSelf(void);
    
            battPercent*  mBatPct;
            RSSIicon*     mRSSI;
};


// *****************************************************
// ******************  cellEditField  ******************
// *****************************************************

// NOTE!! This, as currently written, can not be created in your constructor. It
// won't be able to get its initial drawing commands there. Set it to NULL in your
// constructor then create it as the first thing in your setup().

class cellEditField :  public drawGroup {

  public:
          cellEditField (rect* inRect,char* defText,keyboard* inKeyboard);
  virtual ~cellEditField(void);

  virtual void      drawSelf(void);
  virtual void      setFocus(bool setLoose);
  virtual void      doAction(void);
          int       getNumChars(void);      // Not including the \0. You may need to add one.
          void      formatAsPN(void);
          void      getText(char* inBuff);  // You better have added the (1) for the \0.
          void      setText(char* inText);
          

          keyboard*   mKeyboard;            // They'll pass this in. DON'T delete it!
          editLabel*  mEditField;           // Our editing field.
          colorRect*  mEditBase;            // Our editing field's base.     
};



// *****************************************************
// ******************   homeScreen  ********************
// *****************************************************


// Hardcode a clipping rouded rect to a 32x32 icon. Total hack!
// Stop-gap for not having clipping regions or alpha channels.
class roundedIconBtn : public appIcon {
 
 public:
          roundedIconBtn(int xLoc,int yLoc,int message,char* path);
  virtual ~roundedIconBtn(void);
  
          void  drawPixel(int x,int y,colorObj* pixel);
  virtual void  drawLine(File bmpFile,int x,int y);
  virtual void  drawImage(int x,int y);
  
          int xLoc;
          int yLoc;
};



class homeScreen : public homePanel {

  public:
          homeScreen(void);
  virtual ~homeScreen(void);
  
  virtual void  setup(void);
  virtual void  loop(void);
  virtual void  drawSelf(void);

          bmpPipe*        mBackImage;
          appIcon*        calcIcon;
          appIcon*        textIcon;
          appIcon*        contactIcon;
          appIcon*        toolsIcon;
          appIcon*        phoneIcon;
          roundedIconBtn* breakoutIcon;
          roundedIconBtn* qGameIcon;
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
