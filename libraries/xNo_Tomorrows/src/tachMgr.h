#include <idlers.h>
#include <timeObj.h>
#include <blinker.h>
#include <multiMap.h>

class tachMgr :   public idler,
                  public timeObj {
   public:
            tachMgr(int teeth,float waitTime);
   virtual  ~tachMgr(void);

            bool  begin(int inPin,int outPin);
            void  setupMappers(void);
            void	setAuto(bool onOff);
	virtual  void  idle(void);
				void	setMS(float inMS);
            void  setTach(float inRPM);
            float getTach(void);
            
            bool		runAuto;
            float    RPM;
            float    constVal;
            blinker* tach;
            multiMap RPM_Mapper;
};
