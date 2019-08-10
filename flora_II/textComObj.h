#ifndef textComObj_h
#define textComObj_h

#include <lilParser.h>

class textComObj {

  public:
          textComObj(void);
  virtual ~textComObj(void);

          void begin(void);
          void checkTextCom(void);

  protected:
          void initParams(void);
          void printParams(void);
          void setMoisture(void);
          void setWaterTime(void);
          void setSoakTime(void);
          void setPlantName(void);
          void setPWMPercent(void);
          void setPWMPeriod(void);
          void turnPump(void);
          
          lilParser mParser;
};

extern textComObj textComs;

#endif
