#ifndef handheldOS_h
#define handheldOS_h

#include <lilOS.h>
#include <debug.h>

#define SCREEN_PIN  25
#define BEEP_PIN    23
#define VIBE_PIN    22



enum  apps {
    
   homeApp = HOME_PANEL_ID,
   //fileOpenApp,
   //fileSaveApp,
   //starTrekApp,
   breakoutApp,
   calcApp,
   //rgnTestApp,
   //iconEditApp,
   //grenadeApp,
   settApp,
   //navTestApp,
   //sTermApp,
   sliderApp,
   shopListApp,
   nextApp
}; 



// *****************************************************
// ******************   handheldOS    ******************
// *****************************************************


class handheldOS : public lilOS {

  public:
            handheldOS(void);
   virtual  ~handheldOS(void);
  
   virtual  bool		   begin(void);              // The global world is online, do hookups.
   virtual  panel*	 createPanel(int panelID);
   virtual  void     launchPanel(void);        // Dispose of current and launch a newly created panel.
            void     beep(void);
            void     setScr(bool onOff);  // 0 .. 100 percent 0 being black.
   virtual  void     idle(void);               // If we need to do something in the background, here we are.

  // Calls to be overwritten by used version.
   virtual  int         getTonePin(void);
   virtual  const char* getSystemFolder(void);
   virtual  const char* getPanelName(int panelID);
};

extern handheldOS ourOS;

#endif
