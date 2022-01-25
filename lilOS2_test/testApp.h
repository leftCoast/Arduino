#ifndef testApp_h
#define testApp_h

#include <lilOS.h>

class bubble : public drawObj {
    public:
            bubble(void);
   virtual  ~bubble(void);

   virtual  void eraseSelf(void);
   virtual  void drawSelf(void);
};


class testApp :  public panel {

   public:
            testApp(lilOS* ourOS,int ourAppID);
   virtual  ~testApp(void);

   virtual  void setup(void);
   virtual  void loop(void);

   virtual  void drawSelf(void);

            point    oldLoc;
            bubble*  theBubble;
            timeObj  bubbleTimer;
};


#endif
