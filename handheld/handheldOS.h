#ifndef handheldOS_h
#define handheldOS_h

#include <litlOS.h>
#include <panel.h>

#define SCREEN_PIN   25
#define BEEP_PIN     23

enum  apps { homeApp = HOME_PANEL_ID, starTrekApp, breakoutApp, calcApp, nextApp };



// *****************************************************
// ******************   handheldOS    ******************
// *****************************************************


class handheldOS : public litlOS {

  public:
           handheldOS(void);
  virtual ~handheldOS(void);
  
  virtual int     begin(void);              // The global world is online, do hookups.
  virtual panel*  createPanel(int panelID);
          void    beep(void);
  virtual void    idle(void);               // If we need to do something in the background, here we are.
  //virtual void    loop(void);               // If we need to do something in the foreground, here we are.
};

extern handheldOS  ourOS;

#endif
