#ifndef handheld_h
#define handheld_h

#include "quickCom.h"

class handheld : public qCSlave {

  public :
                handheld(void);
  virtual       ~handheld(void);

  virtual void  begin(void);
          void  checkComs(void);
          void  handleReset(byte* comPtr);
          void  handleReadState(byte* comPtr);
          void  handleReadMoisture(byte* comPtr);
          void  handleReadTemp(byte* comPtr);
          void  handleReadDryLimit(byte* comPtr);
          void  handleReadWTime(byte* comPtr);
          void  handleReadSTime(byte* comPtr);
          void  handleReadName(byte* comPtr);
          void  handleReadPump(byte* comPtr);

          void  handleSetPump(byte* comPtr);
          void  handleSetName(byte* comPtr);
          void  handleSetDryLimit(byte* comPtr);
          void  handleSetWaterTime(byte* comPtr);
          void  handleSetSoakTime(byte* comPtr);
          void  handleSetPulseOnOff(byte* comPtr);

          void  handleGetLogState(byte* comPtr);
          void  handleSetLogState(byte* comPtr);
          void  handleGetLogSize(byte* comPtr);
          void  handleGetLogLines(byte* comPtr);
          void  handleGetLogWLines(byte* comPtr);
          void  handleGetLogBuff(byte* comPtr);
          void  handleDeleteLogFile(byte* comPtr);
};

extern handheld ourHandheld;

#endif
