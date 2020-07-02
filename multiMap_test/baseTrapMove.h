#ifndef baseTrapMove_h
#define baseTrapMove_h

#include "multiMap.h"

class baseTrapMove : public multiMap {

   public:
            baseTrapMove(void);
   virtual  ~baseTrapMove(void);

            void     setupMove(double distD,double velocityV,double accelAngA);
            double   position(double timeT,bool forward);

            double   D;
            double   V;
            double   a;
            double   TANa;
            double   T;
};

#endif
