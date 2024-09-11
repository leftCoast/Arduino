#include "GPS_NMEA.h"
#include <Wire.h>
#include <SPI.h>
#include <timeObj.h>
#include <numStream.h>
#include <Arduino.h>   // required before wiring_private.h
#include <wiring_private.h> // For pinPeripheral() function

// ***************************************************
//                     dataOut class
// ***************************************************


#define SEALEVELPRESSURE_HPA  (1013.25)


enum fields {  hourField,  minField, secField, validField, degLatField,
               minLatField, latQuadField, degLonField, minLonField, lonQuadField,
               knotsField, courseField, dayField, monthField, yearField,
               magVarField, varDirField
            };


class dataOut :   public numStreamOut {

   public :
               dataOut(GPS_NMEA* inParser/*,Adafruit_BME680* inBme*/);
   virtual     ~dataOut(void);

   virtual  void  writeVar(int index);

            GPS_NMEA*         mParser;
};



// ******** Code *********


dataOut::dataOut(GPS_NMEA* inParser)
   : numStreamOut(22) {
   
   mParser  = inParser;
}

   
dataOut::~dataOut(void) {  }


void dataOut::writeVar(int index) {

   switch((fields)index) {
      case hourField    : outPort.print(mParser->$GPRMC.hour);         break;
      case minField     : outPort.print(mParser->$GPRMC.min);          break;
      case secField     : outPort.print(mParser->$GPRMC.sec);          break;
      case validField   : outPort.print((int)mParser->$GPRMC.valid);   break;
      case degLatField  : outPort.print(mParser->$GPRMC.degLat);       break;
      case minLatField  : outPort.print(mParser->$GPRMC.minLat,4);     break;
      case latQuadField : outPort.print((int)mParser->$GPRMC.latQuad); break;
      case degLonField  : outPort.print(mParser->$GPRMC.degLon);       break;
      case minLonField  : outPort.print(mParser->$GPRMC.minLon,4);     break;
      case lonQuadField : outPort.print((int)mParser->$GPRMC.lonQuad); break;
      case knotsField   : outPort.print(mParser->$GPRMC.knots);        break;
      case courseField  : outPort.print(mParser->$GPRMC.course);       break;
      case dayField     : outPort.print(mParser->$GPRMC.day);          break;
      case monthField   : outPort.print(mParser->$GPRMC.month);        break;
      case yearField    : outPort.print(mParser->$GPRMC.year);         break;
      case magVarField  : outPort.print(mParser->$GPRMC.magVar);       break;
      case varDirField  : outPort.print(mParser->$GPRMC.varDir);       break;
   }
}




// ***************************************************
//                     main program
// ***************************************************

#define DEF_BUFF_SIZE         200
//#define GPSPort Serial1





Uart GPSPort (&sercom1, 11, 10, SERCOM_RX_PAD_0, UART_TX_PAD_2);
void SERCOM1_Handler()
{
  GPSPort.IrqHandler();
}


bool              synk;
char              inBuff[DEF_BUFF_SIZE];
int               buffIndex;
GPS_NMEA          ourParser;
dataOut*          ourDataWriter;
timeObj           outTimer(5000);


void blink13(void) {
  
  digitalWrite(13,HIGH);
  delay(20);
  digitalWrite(13,LOW);
  delay(100);
  digitalWrite(13,HIGH);
  delay(20);
  digitalWrite(13,LOW);
}


void setup() {

  Serial.begin(115200);
  while (!Serial) delay(10);
  
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  
  GPSPort.begin(9600);
  // Assign pins 10 & 11 SERCOM functionality
  pinPeripheral(10, PIO_SERCOM);
  pinPeripheral(11, PIO_SERCOM);
  
  synk  = false;
  buffIndex = 0;
  ourDataWriter = new dataOut(&ourParser);
}


// Lets see if there's something waiting for us from the GPS.
void checkGPS(void) {

   char  aChar;
   
   if (GPSPort.available()) {
      aChar = GPSPort.read();
      Serial.print(aChar);
      if (synk) {
         if (aChar=='\n') {
            inBuff[buffIndex] = '\0';
            synk = false;
            if (ourParser.parseStr(inBuff)) {
               ourParser.showGPRMC();
            }
            buffIndex = 0;
         } else {
            inBuff[buffIndex++] = aChar;
         }
      } else {
         if (aChar=='$') {
            inBuff[buffIndex++] = aChar;
            synk = true;
         }
      }
   }
}


void loop() {

   //if (Serial.available()) GPSPort.write(Serial.read());
   //if (GPSPort.available()) Serial.write(GPSPort.read());
   checkGPS();
   if (outTimer.ding()) {
      blink13();
      ourDataWriter->writeStream();
      outTimer.start();
   }
}
