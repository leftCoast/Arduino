#ifndef floraOS_h
#define floraOS_h

#include "litlOS.h"
#include "panel.h"
#include "quickCom.h"
#include "baseButton.h"
#define BEEP_PIN    23

enum  apps { homeApp = HOME_PANEL_ID, controlApp, nextApp };

extern qCMaster ourComPort;


// ******************************************
// *********   From unit.   *********
// ******************************************

enum floraComSet    { floraReset, readMoisture, readWaterTime, readSoakTime, readParams, pumpOn, pumpOff, setMoisture, setWaterTime, setSoakTime, setPulseOn, setPulsePeriod };
enum floraReplySet  { noErr, unknownCom, badParam };

#define NAME_BUFF_BYTES         24

struct paramType {
  int moisture;
  int waterTime;
  int soakTime;
  int percent;
  int period;
  char name[NAME_BUFF_BYTES];
};



// *****************************************************
// ******************   homeScreen   *******************
// *****************************************************


class homeScreen : public homePanel {

  public:
          homeScreen(void);
  virtual ~homeScreen(void);
  
  virtual void  setup(void);
  virtual void  loop(void);
  virtual void  drawSelf(void);
};



// *****************************************************
// ********************   floraOS   ********************
// *****************************************************


class floraOS : public litlOS {

  public:
           floraOS(void);
  virtual ~floraOS(void);
  
  virtual int     begin(void);          // The global world is online, do hookups.
  virtual panel*  createPanel(int panelID);
          void    beep(void);
  virtual void    idle(void);           // If we need to do something in the background, here we are.
};

extern floraOS  ourOS;

#endif
