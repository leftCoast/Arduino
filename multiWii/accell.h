#ifndef accell_h
#define accell_h

#include "mapper.h"
#include "idlers.h"
#include "timeObj.h"

// in case no one told you, this code is for running the Bosch BMA 180 accell chip.
extern mapper accellMapper;

class accell : public idler {

public:
   accell(void);
   ~accell(void);

   virtual void idle(void);
   boolean      newReadings(void);
   void         readRawValues(int* x,int* y,int* z);           // Without calibration..
   void         readValues(int* x,int* y,int* z);              // With calibration. (use this one)
 
   void calibrate(void);                                       // Calculate the offsets. (Needs to be running first)
   void readOffsets(int* xOffset,int* yOffset,int* zOffset);   // Lets say we wanted to save these in Flash memory?
   void setOffsets(int xOffset,int yOffset,int zOffset);
 //void dataDump(void);

protected:
   byte    readRegister(byte regNum);
   boolean writeRegister(byte regNum, byte value);
   void    initAccell(void);
   void    checkAccell(void);
   void    readAccell(void);
   
   timeObj* chipTimer;
   byte     state;
   boolean  dataReady;
   int      x_offset;
   int      y_offset;
   int      z_offset;
   int      x_accell;
   int      y_accell;
   int      z_accell;
};


#endif //accell_h
