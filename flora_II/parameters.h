#ifndef parameters_h
#define parameters_h

#include <EEPROM.h>

#define DEF_MOISTURE_LIMIT      45
#define DEF_READ_TIME           10000
#define DEF_WATER_TIME          10000
#define DEF_SOAK_TIME           120000
#define DEF_CSMOOTHER           20
#define DEF_TSMOOTHER           20
#define DEF_NAME                "Plant name"


// ******************************************
// *********   COPY TO CONTROLLER   *********
// ******************************************

#define NAME_BUFF_BYTES         24

struct paramType {
  int moisture;
  int waterTime;
  int soakTime;
  int percent;
  int period;
  char name[NAME_BUFF_BYTES];
};


// ******************************************
// ******************************************
// ******************************************

class parameters {

public:
          parameters(void);
virtual   ~parameters(void);

          void floraReset(void);
          void readParams(void);
          void saveParams(void);

          int   getMoisture(void);
          int   getWaterTime(void);
          int   getSoakTime(void);
          char* getName(void);
          int   getPWMPercent(void);
          int   getPWMPeriod(void);
          
          void  setMoisture(int percent);
          void  setWaterTime(int waterMs);
          void  setSoakTime(int soakMs);
          void  setName(char* nameStr);
          void  setPWMPercent(int percent);
          void  setPWMPeriod(int periodMs);

          paramType params;
};

extern bool needReset;
extern parameters ourParamObj;

#endif
