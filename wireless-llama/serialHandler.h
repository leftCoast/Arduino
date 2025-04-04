#include "setup.h"
#include <strTools.h>


class serialHandler : public msgHandler {

   public :
            enum states {
               spendHoldTime,    // We only send every X seconds. Wait for that.
               waitForChange,    // We're only sending changes. Wait for a change.
               claimingPort,     // Only one handler gets the port at a time. See if it's clear.
               waitPortReady,    // We need to wait a time after the last transmisson is complete.
               waitPortDone      // We need to wait 'till our transmission is complete to start timer.
            };
            
            serialHandler(netObj* inNetObj,Stream* inPort);
   virtual  ~serialHandler(void);

   virtual  void  idleTime();
   virtual  bool  valueChanged(void);
   virtual  bool  xMissionDone(void);
   virtual  void  sendMessage(void);

            states      ourState;
            Stream*     ourPort;
            timeObj     ourHoldTimer;
            float       holdTime;
            float       lastValue;
};
