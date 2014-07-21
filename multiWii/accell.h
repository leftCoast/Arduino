#ifndef accell_h
#define accell_h

#include "idlers.h"

#define ACC_1G 255

class accell : public idler {

public:
   accell(void);
   ~accell(void);

   virtual void    idle(void);
   boolean newReadings(void);
   void    readValues(int* x,int* y,int* z);
 //void    dataDump(void);

protected:
   byte    readRegister(byte regNum);
   boolean writeRegister(byte regNum, byte value);
   void    initAccell(void);
   void    checkAccell(void);
   void    readAccell(void);

   byte    state;
   boolean dataReady;
   int     x_accell;
   int     y_accell;
   int     z_accell;
};


#endif //accell_h
