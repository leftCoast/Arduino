#ifndef handheldOS_h
#define handheldOS_h

#include <lilOS.h>
#include <panel.h>

#define SCREEN_PIN   25
#define BEEP_PIN     23

enum  apps { homeApp = HOME_PANEL_ID, fileOpenApp, fileSaveApp, starTrekApp, breakoutApp, calcApp, rgnTestApp, iconEditApp, nextApp };



// *****************************************************
// ******************   handheldOS    ******************
// *****************************************************


class handheldOS : public lilOS {

  public:
           handheldOS(void);
  virtual ~handheldOS(void);
  
  virtual int     begin(void);              // The global world is online, do hookups.
  virtual panel*  createPanel(int panelID);
  virtual void    launchPanel(void);        // Dispose of current and launch a newly created panel.
          void    beep(void);
          void    hideRedraw(void);
          void    bringUp(void);
  virtual void    idle(void);               // If we need to do something in the background, here we are.

          bool        mDimScreen;
          int         mNowTime;
          int         mEndTime;
          multiMap    screenMap;
          timeObj     mScreenTimer;
};

extern handheldOS  ourOS;


#endif
