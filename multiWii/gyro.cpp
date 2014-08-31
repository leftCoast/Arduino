#include <Wire.h>
#include "gyro.h"

#define GYRO_ADDR  B01101000   // Figured out that much.

#define DIV_REG    21
#define DLPF_REG   22
#define FS_SEL     B00011000  // As per databook. Set FS_SEL to 3, no other choice.
#define SMPLRT_DIV B00000000  // 8000Hz these next two are taken from the MW code.
#define DLPF_CFG   B00000000
    
#define INT_CONFIG       23
#define LATCH_INT_EN     B00100000 // Hold high 'till read.
#define INT_ANYRD_2CLEAR B00000000 // Only clear when status register is read.
#define RAW_RDY_EN       B00000001 // Turn on data ready inturupt. (flag)

#define PWR_MGM   62
#define CLK_SEL   B00000001 // Data sheet says its better to select a gyro as clock. This is X gyro.

#define INT_STATUS     26
#define READY_MASK     B00000001

#define X_HIGH         29
#define X_LOW          30
#define Y_HIGH         31
#define Y_LOW          32
#define Z_HIGH         33
#define Z_LOW          34

#define CALIB_ITR      500

enum gyroStates { unInitilized, calculating, valueReady };

gyro::gyro(void) {
   
   state = unInitilized;
   dataReady = false;
   x_offset = 0;
   y_offset = 0;
   z_offset = 0;
   x_rotation = 0;
   y_rotation = 0;
   z_rotation = 0;
   lastRead = 0;
   x_angle = 0;
   y_angle = 0;
   z_angle = 0;
}

   
gyro::~gyro(void) {  }
   
   
boolean gyro::newReadings(void) { 
   
   hookup();           // First time we ask for readings, hook up.
   return dataReady;
}


void gyro::readValues(int* x,int* y,int* z) {
   
   *x = x_rotation;
   *y = y_rotation;
   *z = z_rotation;
   dataReady = false; // You done read it. I'll let you know when there's a fresh batch.
}


void gyro::idle(void) {
   
   switch (state) {
      case unInitilized : initGyro(); break;
      case calculating : checkGyro(); break;
      case valueReady : readGyro(); break;
   }
}
  
  
byte gyro::readRegister(byte regNum) {
   
   Wire.beginTransmission(GYRO_ADDR);       // Start all this nonsense.
   Wire.write(regNum);                      // sends register number
   Wire.endTransmission();                  // transmit!
   if (Wire.requestFrom(GYRO_ADDR, 1)==1)   // request 1 byte from slave device #GYRO_ADDR
       return Wire.read();                  // We got our byte, return it.
   else {
      //Serial.println("readReg fail!");
      return 0;                            // if it fails? Return zero I guess..
   }
}
 
 
boolean gyro::writeRegister(byte regNum, byte value) {

   //Serial.print("Writing Reg : ");Serial.print(regNum);Serial.print("  Value : ");Serial.println(value,BIN);
   Wire.beginTransmission(GYRO_ADDR);       // Hello, we're talkin to you!
   Wire.write(regNum);                      // sends register number
   Wire.write(value);                       // sends value to write in said register.
   return(Wire.endTransmission()==0);       // transmit! 0 means success.
}


void gyro::initGyro(void) {
    
   if (writeRegister(DIV_REG,SMPLRT_DIV))                                          // Setup sample rates, filtering etc.
      if(writeRegister(DLPF_REG,FS_SEL|DLPF_CFG))
         if (writeRegister(INT_CONFIG,LATCH_INT_EN|INT_ANYRD_2CLEAR|RAW_RDY_EN))   // Turn on the flag to show new readings.
            if (writeRegister(PWR_MGM,CLK_SEL))                                    // Do the "select x gyro as clk" thing
            state = calculating;                                                   // At this point, we're good! 
}


// Assume its initialized and ready to go. Check for new readings.
void gyro::checkGyro(void) {
   
   byte          gyroStatus;
   unsigned long deltaT;
   unsigned long now;
   float         deltaSec;
   
   gyroStatus = readRegister(INT_STATUS);
   if(gyroStatus & READY_MASK) {
      now = micros();                         // We see data, note the time.
      if(lastRead != 0 && lastRead < now) {   // If we have 2 time values and we're not crossing the 0 line..
         deltaT = now - lastRead;
         deltaSec = deltaT/1000000.0;
         //Serial.println(deltaT);
         //Serial.println(deltaSec,4);
         x_angle = x_angle + (deltaSec * x_rotation);  // And these units would be degrees?
         y_angle = y_angle + (deltaSec * y_rotation);
         z_angle = z_angle + (deltaSec * z_rotation);
      }
      lastRead = now;
      state = valueReady;
   }
}


// Assume its initialized and there is new data waiting.
void gyro::readGyro(void) {
   
   int high;
   int low;
  
   high = readRegister(X_HIGH);
   low = readRegister(X_LOW);
   high = high << 8;
   x_rotation = high + low;
   x_rotation = x_rotation - x_offset;
   
   high = readRegister(Y_HIGH);
   low = readRegister(Y_LOW);
   high = high << 8;
   y_rotation = high + low;
   y_rotation = y_rotation - y_offset;
   
   high = readRegister(Z_HIGH);
   low = readRegister(Z_LOW);
   high = high << 8;
   z_rotation = high + low;
   z_rotation = z_rotation - z_offset;
   
   state = calculating;
   dataReady = true;
}
 
 
 // Calibrate routine must be called when the controller board is being held level and not moving.
void gyro::calibrate(void) {
   
   int  x;
   int  y;
   int  z;
   long rotX = 0;
   long rotY = 0;
   long rotZ = 0;
   int  i = 0;
   
   if (state!=unInitilized) {   // Only bother if we're running.
      setOffsets(0,0,0);        // Clear offsets to start..
      while(i<CALIB_ITR) {
         while(!newReadings()) theIdlers.idle();   // let -everyone- have some time..
         readValues(&x,&y,&z);
         rotX = rotX + x;
         rotY = rotY + y;
         rotZ = rotZ + z;
         i++;
      }
      setOffsets(rotX/CALIB_ITR,rotY/CALIB_ITR,rotZ/CALIB_ITR); // Send in the avarages.
   }
}


void gyro::readOffsets(int* xOffset,int* yOffset,int* zOffset) {
   
   *xOffset = x_offset;
   *yOffset = y_offset;
   *zOffset = z_offset;
}


void gyro::setOffsets(int xOffset,int yOffset,int zOffset) {
   
   x_offset = xOffset;
   y_offset = yOffset;
   z_offset = zOffset;
}


void gyro::readAngles(int* xAngle,int* yAngle,int* zAngle) {
     
   *xAngle = x_angle;
   *yAngle = y_angle;
   *zAngle = z_angle;
}


void gyro::setAngles(int xAngle,int yAngle,int zAngle) {
 
   x_angle = xAngle;
   y_angle = yAngle;
   z_angle = zAngle;
}
   
   
/*  
void gyro::dataDump(void) {
   
   Serial.print("State = "); 
   switch(state) {
      case unInitilized : Serial.println("unInitilized"); break;
      case calculating : Serial.println("calculating"); break;
      case valueReady : Serial.println("valueReady"); break;
      default : Serial.println("INVALID"); break;
   };
   Serial.print("dataReady = "); Serial.println(dataReady);
   Serial.print("x_rotation = "); Serial.println(x_rotation);
   Serial.print("y_rotation = "); Serial.println(y_rotation);
   Serial.print("z_rotation = "); Serial.println(z_rotation);
   Serial.println();
}
*/
