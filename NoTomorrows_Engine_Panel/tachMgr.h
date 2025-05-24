#include <idlers.h>
#include <timeObj.h>
#include <blinker.h>
#include <multiMap.h>

class tachMgr :   public idler,
                  public timeObj {
   public:
            tachMgr(int teeth,float waitTime);
   virtual  ~tachMgr(void);

   virtual  void  idle(void);
            bool  begin(int inPin,int outPin);
            void  setupMappers(void);
            void  setTach(float inRPM);
            
            float    RPM;
            float    constVal;
            blinker* tach;
            multiMap RPMUpMapper;
            multiMap RPMDnMapper;
};
