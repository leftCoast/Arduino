#include "LC_llama2000.h"



// ************* msgHandler *************
//   Below are example message handlers.
// *************************************


class waterSpeedObj  : public msgHandler {

   public:
        waterSpeedObj(netObj* inNetObj);
        ~waterSpeedObj(void);

            float getSpeed(void);
   virtual  bool  handleMsg(message* inMsg);
             
          mapper  speedMap;
          float   knots;
};



// ************* waterDepthObj *************


class waterDepthObj  : public msgHandler {

   public:
        waterDepthObj(netObj* inNetObj);
        ~waterDepthObj(void);
          
            float getDepth(void);
  virtual bool  handleMsg(message* inMsg);
  
            float feet;
          
};


// ************* waterTempObj *************


class waterTempObj  : public msgHandler {

   public:
            waterTempObj(netObj* inNetObj);
            ~waterTempObj(void);
          
            float getTemp(void);
   virtual  bool  handleMsg(message* inMsg);
   
            float   degF;
};



// ************* fluidLevelObj *************

enum tankType {
   fuel,
   water,
   grayWater,
   liveWell,
   oil,
   blackWater
};


class fluidLevelObj  : public msgHandler {

   public:
            fluidLevelObj(netObj* inNetObj);
            ~fluidLevelObj(void);
          
            tankType getTankType(void);
            void     setTankType(tankType inType);
            float    getLevel(void);
            void     setLevel(float inLevel);
            float    getCapacity(void);
            void     setCapacity(float inCapacity);
   virtual  bool    handleMsg(message* inMsg);
   virtual  void     newMsg(void);
   
            tankType fluidType;
            float    level;
            float    capacity;
            int      tankID;
};



// ************* airTempBarometer *************

//runningAvg inHgSmooth(6);

class airTempBarometer  : public msgHandler {

   public:
            airTempBarometer(netObj* inNetObj);
            ~airTempBarometer(void);
          
   virtual  bool  handleMsg(message* inMsg);
        float getAirTemp(void);
        float getInHg(void);
   
        float       degF;
        runningAvg* inHgSmooth;
            float       inHg;
};


// ************* LC_ChatObj *************

class LC_ChatObj  : public msgHandler {

   public:
            LC_ChatObj(netObj* inNetObj);
            ~LC_ChatObj(void);

            void  setOutStr(char* inStr);
            void  clearOutStr(void);
   virtual  bool  handleMsg(message* inMsg);
   virtual  void  newMsg(void);

            char* outStr;
};
