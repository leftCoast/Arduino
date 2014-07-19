#ifndef gyro_h
#define gyro_h

#include <idlers.h>

// ************************************************************************************************************
// I2C Gyroscope ITG3200 
// ************************************************************************************************************
//  
// InvenSense Inc.
// 1197 Borregas Ave, 
//  Sunnyvale, CA 94089 U.S.A. 
//  Tel: +1 (408) 988-7339
//  Website: www.invensense.com
//  
// PN : ITG3200
 
 /*
 Next chips to code..
 BMA180
 BMP085
 HMC5883
 */


class gyro : 
public idler {

public:
   gyro(void);
   ~gyro(void);

   virtual void    idle(void);
   boolean newReadings(void);
   void    readValues(int* x,int* y,int* z);
   //void    dataDump(void);

protected:
   byte    readRegister(byte regNum);
   boolean writeRegister(byte regNum, byte value);
   void    initGyro(void);
   void    checkGyro(void);
   void    readGyro(void);

   byte    state;
   boolean dataReady;
   int     x_rotation;
   int     y_rotation;
   int     z_rotation;
};


#endif // gyro_h

