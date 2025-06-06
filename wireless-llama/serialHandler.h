
#include <strTools.h>
#include <SAE_J1939.h>

class serialHandler : public msgHandler {

   public :
            enum states {
               doingHoldTime,    // We only send every X seconds. Wait for that.
               watchForChange,   // We're only sending changes. Watch for a change.
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
            timeObj     blindSendTimer;
            float       holdTime;
            float       lastValue;
};
