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

/*
enum scrModes {
  scrOn,
  scrOff,
  scrRising,
  scrSetting
};
*/

class handheldOS : public lilOS {

  public:
            handheldOS(void);
   virtual  ~handheldOS(void);
  
	virtual  bool		   begin(void);              // The global world is online, do hookups.
   virtual  panel*	 createPanel(int panelID);
   virtual  void     launchPanel(void);        // Dispose of current and launch a newly created panel.
            void     beep(void);
            void     setScr(float brightness);  // 0 .. 100 percent 0 being black.
            //void     hideRedraw(void);
            //void     bringUp(void);
   virtual  void     idle(void);               // If we need to do something in the background, here we are.

  // Calls to be overwritten by used version.
   virtual  int         getTonePin(void);
   virtual  const char* getSystemFolder(void);
   virtual  const char* getPanelName(int panelID);
   
            /*
            bool     hidingRedraw;
            scrModes scrMode;
            int      mNowTime;
            int      mEndTime;
            mapper  rampUpMap;
            mapper  rampDnMap;
            timeObj scrTimer;
            */
            mapper  scrBvalMapper;
};

extern handheldOS ourOS;

#endif
