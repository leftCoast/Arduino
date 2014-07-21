#include <Wire.h>
#include "accell.h"


//Address, ID and Registers
#define ACCELL_ADDR 0x40  /* (7bit) (SDO connection to VCC) */
#define ID          0x00
#define Version     0x01
#define ACCXLSB     0x02
#define ACCXMSB     0x03
#define ACCYLSB     0x04
#define ACCYMSB     0x05
#define ACCZLSB     0x06
#define ACCZMSB     0x07
#define TEMPERATURE 0x08
#define STATREG1    0x09
#define STATREG2    0x0A
#define STATREG3    0x0B
#define STATREG4    0x0C
#define CTRLREG0    0x0D
#define CTRLREG1    0x0E
#define CTRLREG2    0x0F

#define BW_TCS      0x20
#define CTRLREG3    0x21

#define HILOWNFO    0x25
#define LOWDUR      0x26

#define LOWTH       0x29

#define TCO_Y       0x2F
#define TCO_Z       0x30

#define OLSB1       0x35


enum accellStates { unInitilized, calculating, valueReady };

accell::accell(void) {
   
   state = unInitilized;
   dataReady = false;
   x_accell = 0;
   y_accell = 0;
   z_accell = 0;
}

   
accell::~accell(void) {  }
   
   
boolean accell::newReadings(void) { 
   
   hookup();           // First time we ask about readings, hook up.
   return dataReady;
}


void accell::readValues(int* x,int* y,int* z) {
   
   *x = x_accell;
   *y = y_accell;
   *z = z_accell;
   dataReady = false; // You done read it. I'll let you know when there's a fresh batch.
}


void accell::idle(void) {
   
   switch (state) {
      case unInitilized : initAccell(); break;
      case calculating : checkAccell(); break;
      case valueReady : readAccell(); break;
   }
}
  
  
byte accell::readRegister(byte regNum) {
   
   Wire.beginTransmission(ACCELL_ADDR);       // Start all this nonsense.
   Wire.write(regNum);                      // sends register number
   Wire.endTransmission();                  // transmit!
   if (Wire.requestFrom(ACCELL_ADDR, 1)==1)   // request 1 byte from slave device #accell_ADDR
       return Wire.read();                  // We got our byte, return it.
   else {
      //Serial.println("readReg fail!");
      return 0;                            // if it fails? Return zero I guess..
   }
}
 
 
boolean accell::writeRegister(byte regNum, byte value) {

   //Serial.print("Writing Reg : ");Serial.print(regNum);Serial.print("  Value : ");Serial.println(value,BIN);
   Wire.beginTransmission(ACCELL_ADDR);      // Hello, we're talkin to you!
   Wire.write(regNum);                      // sends register number
   Wire.write(value);                       // sends value to write in said register.
   return(Wire.endTransmission()==0);       // transmit! 0 means success.
}


//default range 2G: 1G = 4096 unit.
void accell::initAccell(void) {
    
   byte temp;
 
   if (readRegister(ID)==3) {          // We talkin'? Or am I wastin' my breath? 3 is good.. - PG 54 BMA 180 Databook
      
      temp = readRegister(CTRLREG0);   // Grab CTRLREG0.. - PG 47 BMA 180 Databook
      temp = temp | B00010000;         // Set bit ee_w to 1 to enable writing.
      writeRegister(CTRLREG0,temp);    // Write it back.
      
      temp = readRegister(BW_TCS);     // Copy the BW_TCS register. - PG 29 BMA 180 Databook
      temp = temp & B00001111;         // Stomp bw reg.. setting low pass filter to 10Hz (bits value = 0000xxxx)
      writeRegister(BW_TCS, temp);     // Set it back in.
      
      temp = readRegister(TCO_Z);      // Copy the TCO_Z register. - PG 29 BMA 180 Databook
      temp = temp & B11111110;         // Set mode_config to 0. Why? Low noise, high current. Full bandwidth!
      writeRegister(TCO_Z, temp);      // Send it back in.
      
      temp = readRegister(OLSB1);      // Copy the OLSB1 register. - PG 28 BMA 180 Databook
      temp = temp & B00001111;         // Clear range bits.
      temp = temp | B01010000;         // Set range bits to 8G (MW again..)
      writeRegister(OLSB1, temp);      // Send it back in.
      
      state = calculating;             // At this point, we're good! 
   }
}


// Assume its initialized and ready to go. Check for new readings.
void accell::checkAccell(void) {
   
   byte accellStatus;
   
   accellStatus = readRegister(ACCXLSB);               // Check ready bit  PG 19 & 52 BMA 180 Databook
   if(accellStatus & B00000001) state = valueReady; 
}


// Assume its initialized and there is new data waiting.
void accell::readAccell(void) {
   
   int high;
   int low;
   
   low = readRegister(ACCXLSB);   // Must read LSB first - 52 BMA 180 Databook
   high = readRegister(ACCXMSB);
   high = high << 8;
   x_accell = high + low;
   x_accell >> 2;                 // Dump the two non-value bits in LSB - PG 22 & 52 BMA 180 Databook
   
   low = readRegister(ACCYLSB);
   high = readRegister(ACCYMSB);
   high = high << 8;
   y_accell = high + low;
   y_accell >> 2;
   
   low = readRegister(ACCZLSB);
   high = readRegister(ACCZMSB);
   high = high << 8;
   z_accell = high + low;
   z_accell >> 2;
   
   state = calculating;
   dataReady = true;
}
 

/* 
void accell::dataDump(void) {
   
   Serial.print("State = "); 
   switch(state) {
      case unInitilized : Serial.println("unInitilized"); break;
      case calculating : Serial.println("calculating"); break;
      case valueReady : Serial.println("valueReady"); break;
      default : Serial.println("INVALID"); break;
   };
   Serial.print("dataReady = "); Serial.println(dataReady);
   Serial.print("x_accell = "); Serial.println(x_accell);
   Serial.print("y_accell = "); Serial.println(y_accell);
   Serial.print("z_accell = "); Serial.println(z_accell);
   Serial.println();
}
*/
