#ifndef floraOS_h
#define floraOS_h

#include <lilOS.h>
#include <panel.h>
#include <quickCom.h>
//#include "src/rpnCalc/rpnCalc.h"


#define BEEP_PIN    23

//calcApp
enum  apps { homeApp = HOME_PANEL_ID, controlApp, loggingApp, sTermApp, nextApp };



// *****************************************************
// ********************   floraOS   ********************
// *****************************************************


class floraOS : public lilOS {

  public:
           floraOS(void);
  virtual ~floraOS(void);
  
  virtual int     begin(void);              // The global world is online, do hookups.
  virtual panel*  createPanel(int panelID);
          void    beep(void);
  virtual void    idle(void);               // If we need to do something in the background, here we are.
  virtual void    loop(void);               // If we need to do something in the foreground, here we are.
};

extern floraOS  ourOS;

#endif
