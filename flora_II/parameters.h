#ifndef parameters_h
#define parameters_h

#include <EEPROM.h>

#define DEF_MOISTURE_LIMIT      40            // These guys are in the parameer block.
#define DEF_WATER_TIME          10000         // Defauly water time, 10 seconds.
#define DEF_SOAK_TIME           120000        // Default soak time, 120 seconds.
#define DEF_NAME                "Plant name"
#define DEF_DRY                  335          // Wipe it dry and block it up aff the wood table.
#define DEF_MUD                  1015         // Holding it in your hand it maxes out.

#define DEF_READ_TIME           500           // These guys are hard coded.
#define DEF_CSMOOTHER           20
#define DEF_TSMOOTHER           20

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
  int dry;
  int mud;
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
          int   getDry(void);
          int   getMud(void);
          
          void  setMoisture(int percent);
          void  setWaterTime(int waterMs);
          void  setSoakTime(int soakMs);
          void  setName(char* nameStr);
          void  setPWMPercent(int percent);
          void  setPWMPeriod(int periodMs);
          void  setDry(int dryCap);
          void  setMud(int mudCap);
          
          paramType params;
};

extern bool needReset;
extern parameters ourParamObj;

#endif
