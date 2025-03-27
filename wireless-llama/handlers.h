#include "setup.h"
//                                                                                        |
// Each type of device that comunicates with the network needs to be able to perform
// certain functions. We try to automate most of these using the J1939 library. BUT, the
// bits where info. comes from the "real" world through sensors etc. And where info. is to
// be displayed through whatever medium the end user would like. YOU need to write those.
// This is where handlers come into the picure.
//                                                                                       |
// If you are some sort of sensor reading data from the outside world. You will need your
// code wrapped into a handler class. In this class you'll aquire the neccissary
// information. And, at the right time. Send it out on the network for other to see.
//
// If you are a display or control of some kind. Again, your code needs to be wrapped into
// a handler class to access the requred information so you can perform your task. Be it
// display or control. Or even putting it back on the netowork for others.
// 
//                                                                                       |
// Following is a copy of the base msgHandler class definition. This is what you will
// inherit and extend to create your own handlers.
//
// Now, all that being said. The J1939/NMEA2000 spec. tries to lay out every conceivable
// data type message that will ever be. At first passs this seems like a futile and
// absurd idea. But, if it's a finite set of data types. And all of them are pre-defined.
// This makes it paossible for diffrent systems to share data and workk together. (Yeah..)
//
// Like I said. A futile and absurd idea. Even more so because, as far as I can find out
// reading about it, it costs tens of thousands of dollars to see and/or use the "offical"
// data descriptions.
//
// Or, if your makeing bits for your boat, like I am.
// You can go here : https://canboat.github.io/canboat/canboat.html
//
/*
 // ***************************************************************************************
//                              ----- msgHandler -----
// ***************************************************************************************


// Base class for handling and creation of SAE J1939 network messages. Inherit this create
// your handler object and add them using the netObj call addMsgHandler().
class msgHandler :   public linkListObj { 

   public:
            msgHandler(netObj* inNetObj);
            ~msgHandler(void);
            
            int   getNumBytes(void);            // These only set the value. No buffer here.
            void  setNumBytes(int inNumBytes);  // The message class reads  and uses this.
   virtual  bool  handleMsg(message* inMsg);    // Fill in to handle messages.
   virtual  void  newMsg(void);                 // Fill in to create messages.
   virtual  void  sendMsg(message* inMsg);      // This one just sends messages on their way.
   
            void  setSendInterval(float inMs);  // Used for broadcasting.
            float getSendInterval(void);        //
   virtual  void  idleTime(void);               // Same as idle, but called by the netObj.
            
            netObj*  ourNetObj;                 // Pointer to our boss!
            timeObj  intervaTimer;              // If broadcasting, how often do we broadcast? (Ms)
};
 */
//                                                                                       |
// So, what I have here is some example handlers I wrote for devices I could fing "cheap"
// on Amazon. And a nifty chat handler to type messages between devices. Like do ing text
// but down the NMEA2000 network.


// ************* msgHandler *************
//   Below are example message handlers.
// *************************************



// ************* waterSpeedObj *************

// This listens for boat speed messages and outputs a boatspeed in knots. The transducer
// used for this test was an AIRMAR DST810

class waterSpeedObj  : public msgHandler {

   public:
            waterSpeedObj(netObj* inNetObj);
   virtual  ~waterSpeedObj(void);
   
            float getSpeed(void);
   virtual  bool  handleMsg(message* inMsg);
   
            mapper  speedMap;
            float   knots;
  };



// ************* waterDepthObj *************

// This listens for water depth messages and outputs a depth in feet. The transducer
// used for this test was an AIRMAR DST810

class waterDepthObj  : public msgHandler {

   public:
            waterDepthObj(netObj* inNetObj);
   virtual  ~waterDepthObj(void);
   
            float getDepth(void);
   virtual  bool  handleMsg(message* inMsg);
   
            float feet;
        
};



// ************* waterTempObj *************

// This listens for water depth messages and outputs a depth in feet. The transducer
// used for this test was an AIRMAR DST810

class waterTempObj  : public msgHandler {

   public:
            waterTempObj(netObj* inNetObj);
            ~waterTempObj(void);
          
            float getTemp(void);
   virtual  bool  handleMsg(message* inMsg);
   
            float   degF;
};



// ************* fluidLevelObj *************
//                                                                                       |
// This takes in a float value from the user. This value is interpreted as the percentage
// full of fuel tank #0. This value is then broadcast onto the network every 2.5 seconds
// for whatever display that wants to show the fuel level value. All the different tank
// types are specified by the NMEA2000/J-1939 documentation. Also is tank capacity. I
// think of this as gallons, being American. The display shows this to me in gallons. But
// the network needs to broadcast it in deciliters. So we'll do a calculation when we
// send it.
//
// Displays tested were a KUS K-N00001 NMFR-BS and a B&G H5000 Graphic.

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
   virtual  ~fluidLevelObj(void);
          
            tankType getTankType(void);
            void     setTankType(tankType inType);
            float    getLevel(void);
            void     setLevel(float inLevel);
            float    getCapacity(void);
            void     setCapacity(float inCapacity);
   virtual  bool     handleMsg(message* inMsg);
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
