#ifndef handheld_h
#define handheld_h

#include "quickCom.h"

class handheld : public qCSlave {

  public :
                handheld(void);
  virtual       ~handheld(void);

  virtual void  begin(void);
          void  checkComs(void);
          void  handleReadMoisture(byte* comPtr);
          void  handleReset(byte* comPtr);
          void  handleReadParams(byte* comPtr);              
          void  handleSetPump(byte* comPtr);
          void  handleSetMoisture(byte* comPtr);
          void  handleSetWaterTime(byte* comPtr);
          void  handleSetSoakTime(byte* comPtr);
          void  handleSetPulseOn(byte* comPtr);
          void  handleSetPulsePeriod(byte* comPtr);
};

extern handheld ourHandheld;

#endif
