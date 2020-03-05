#ifndef parameters_h
#define parameters_h

 #include <EEPROM.h>

#define DEF_DRY_LIMIT           25            // These guys are in the parameer block.
#define DEF_WATER_TIME          30000         // Defauly water time, 10 seconds.
#define DEF_SOAK_TIME           120000        // Default soak time, 120 seconds.
#define DEF_NAME                "Left coast PlantBot"
#define DEF_DRY                  335          // Wipe it dry and block it up off the wood table.
#define DEF_MUD                  1015         // Holding it in your hand it maxes out.
#define DEF_RUN_NUM              0            // Default to NOT logging.

#define DEF_READ_TIME           500           // These guys are hard coded.
#define DEF_CSMOOTHER           20
#define DEF_TSMOOTHER           20

// ******************************************
// *********   COPY TO CONTROLLER   *********
// ******************************************

#define NAME_BUFF_BYTES         24

struct paramType {                  // paramType is the TYPE of structure we save all of our parameters in. We can plunk it into EEPROM and read it right back out.
  int dryLimit;                     // The moisture percentage reading (limit) we use to trigger when its time to water.
  int waterTime;                    // How long we will water once the limit is reached in miliseconds.
  int soakTime;                     // How long we will wait after watering before starting up moisture readings again. This is also in miliseconds.
  int percent;                      // Currently ignored. What percent of time do we hold the motor power on each time we want to give it a jolt of juice. (Throttle)
  int period;                       // Actually this, I think is used. How often do we give the motor a jolt of power to hold the speed we want. (Smoothness, [Number of cylinders])
  int dry;                          // What capacitive reading are we calling our totally dry limit.
  int mud;                          // What capacitive reading are we calling absolute mud. The totally wet limit.
  int runNum;                       // Data logger sets this to 1 if logging is turned on, 0 if turned off. If it reads a 1, it adds 1 to it.
  char name[NAME_BUFF_BYTES];       // Although at this time you can't change it. Each plant minder can have its own name. (Later)
};


// ******************************************
// ******************************************
// ******************************************

class parameters {                            // parameters is the onject that manages your saved info.

public:
          parameters(void);
virtual   ~parameters(void);

          void floraReset(void);
          void readParams(void);
          void saveParams(void);

          int   getDryLimit(void);
          int   getWaterTime(void);
          int   getSoakTime(void);
          char* getName(void);
          int   getPWMPercent(void);
          int   getPWMPeriod(void);
          int   getDry(void);
          int   getMud(void);
          int   getRunNum(void);
          
          void  setDryLimit(int percent);
          void  setWaterTime(int waterMs);
          void  setSoakTime(int soakMs);
          void  setName(char* nameStr);
          void  setPWMPercent(int percent);
          void  setPWMPeriod(int periodMs);
          void  setDry(int dryCap);
          void  setMud(int mudCap);
          void  setRunNum(int runNum);
          paramType params;
};

extern bool needReset;          
extern parameters ourParamObj;  // Global object for managing your parameters.

#endif
