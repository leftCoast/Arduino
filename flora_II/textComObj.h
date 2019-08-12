#ifndef textComObj_h
#define textComObj_h

#include <timeObj.h>
#include <idlers.h>
#include <lilParser.h>

class textComObj : public idler {

  public:
          textComObj(void);
  virtual ~textComObj(void);

          void begin(void);
          void checkTextCom(void);

  protected:
          void  initParams(void);
          void  printParams(void);
          void  printReadings(void);
          void  doPrintReadings(void);
          void  setMoisture(void);
          void  setWaterTime(void);
          void  setSoakTime(void);
          void  setPlantName(void);
          void  setPWMPercent(void);
          void  setPWMPeriod(void);
          void  setDry(void);
          void  setMud(void);
          void  turnPump(void);
  virtual void  idle(void);
          
          lilParser mParser;
          bool      mAutoRead;
          timeObj   readTimer;
};

extern textComObj textComs;

#endif
