#ifndef handheldOS_h
#define handheldOS_h

#include <lilOS.h>

#define SCREEN_PIN              25
#define BEEP_PIN                23


enum  apps { homeApp = HOME_PANEL_ID, fileOpenApp, fileSaveApp, starTrekApp, breakoutApp, calcApp, rgnTestApp, iconEditApp, grenadeApp, settApp, nextApp }; // settApp,



// *****************************************************
// ******************   handheldOS    ******************
// *****************************************************


class handheldOS : public lilOS {

  public:
            handheldOS(void);
   virtual  ~handheldOS(void);
  
   virtual  bool      begin(void);              // The global world is online, do hookups.
   virtual  panel*   createPanel(int panelID);
   virtual  void     launchPanel(void);        // Dispose of current and launch a newly created panel.
            void     beep(void);
            void     hideRedraw(void);
            void     bringUp(void);
   virtual  void     idle(void);               // If we need to do something in the background, here we are.

  // Calls to be overwritten by used version.
   virtual  int      getPanelWidth(void);
   virtual  int      getPanelHeight(void);
   virtual  int      getTonePin(void);
   virtual  void     setBrightness(byte brightness);
   virtual  char*    getSystemFolder();
   virtual  char*    getPanelFolder(int panelID);
   
            bool     mDimScreen;
            int      mNowTime;
            int      mEndTime;
            multiMap screenMap;
            timeObj  mScreenTimer;
};

extern handheldOS ourOS;

#endif
