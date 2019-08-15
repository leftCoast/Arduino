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
          void  handleReadDryLimit(byte* comPtr);
          void  handleReadWTime(byte* comPtr);
          void  handleReadSTime(byte* comPtr);
          void  handleReadName(byte* comPtr);
          void  handleReset(byte* comPtr);
          void  handleSetPump(byte* comPtr);
          void  handleSetDryLimit(byte* comPtr);
          void  handleSetWaterTime(byte* comPtr);
          void  handleSetSoakTime(byte* comPtr);
          void  handleSetPulseOnOff(byte* comPtr);
};

extern handheld ourHandheld;

#endif
