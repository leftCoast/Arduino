#ifndef floraOS_h
#define floraOS_h

#include <litlOS.h>
#include <panel.h>
#include <quickCom.h>
#include "globals.h"
#include "baseButton.h"

#define BEEP_PIN    23

enum  apps { homeApp = HOME_PANEL_ID, controlApp, nextApp };

extern qCMaster ourComPort;


// ******************************************
// *********   From unit.   *********
// ******************************************

enum floraComSet    {
                      floraReset,
                      readName,
                      readState,
                      readMoisture,
                      readTemp,
                      readDryLimit,
                      readWaterTime,
                      readSoakTime,
                      pumpOn, pumpOff,
                      setDryLimit,
                      setWaterTime,
                      setSoakTime,
                      setPulseOn,
                      setPulseOff, 

                      getLogState,
                      setLogState,
                      getLogSize,
                      getLogBuff,
                      deleteLogFile,
                      };
                      
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
// ********************   floraOS   ********************
// *****************************************************


class floraOS : public litlOS {

  public:
           floraOS(void);
  virtual ~floraOS(void);
  
  virtual int     begin(void);          // The global world is online, do hookups.
  virtual panel*  createPanel(int panelID);
          void    beep(bool beepUp);
  virtual void    idle(void);           // If we need to do something in the background, here we are.
};

extern floraOS  ourOS;

#endif
