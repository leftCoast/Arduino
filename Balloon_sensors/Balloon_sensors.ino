#include "GPS_NMEA.h"
#include <Wire.h>
#include <SPI.h>
#include <timeObj.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <numStream.h>

// ***************************************************
//                     dataOut class
// ***************************************************


#define SEALEVELPRESSURE_HPA  (1013.25)


enum fields {  hourField,  minField, secField, validField, degLatField,
               minLatField, latQuadField, degLonField, minLonField, lonQuadField,
               knotsField, courseField, dayField, monthField, yearField,
               magVarField, varDirField,
       
               tempField, pressureField, humidityField, gasField, AltitudeField
            };


class dataOut :   public numStreamOut {

   public :
               dataOut(GPS_NMEA* inParser,Adafruit_BME680* inBme);
   virtual     ~dataOut(void);

   virtual  void  writeVar(int index);

            GPS_NMEA*         mParser;
            Adafruit_BME680*  mBme;
};



// ******** Code *********


dataOut::dataOut(GPS_NMEA* inParser,Adafruit_BME680* inBme)
   : numStreamOut(22) {
   
   mParser  = inParser;
   mBme     = inBme;
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
      
      case tempField       : outPort.print(mBme->temperature);                         break;
      case pressureField   : outPort.print(mBme->pressure/100.0);                      break;
      case humidityField   : outPort.print(mBme->humidity);                            break;
      case gasField        : outPort.print(mBme->gas_resistance/1000.0);               break;
      case AltitudeField   : outPort.print(mBme->readAltitude(SEALEVELPRESSURE_HPA));  break;
      
   }
}




// ***************************************************
//                     main program
// ***************************************************

#define DEF_BUFF_SIZE         200
#define GPSPort               Serial3
#define masterPort            Serial1


enum states { readingAir, readingGPS };

Adafruit_BME680   bme; // I2C
states            ourState;
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
}


void setup() {
   
   pinMode(13,OUTPUT);
   digitalWrite(13,LOW);
   masterPort.begin(9600);    // This is our output serial port.
   GPSPort.begin(9600);       // And this is where we read in GPS data.
   synk  = false;
   buffIndex = 0;
   ourState = readingGPS;
   ourDataWriter = new dataOut(&ourParser,&bme);
   
   bme.begin();
   // Set up oversampling and filter initialization
   bme.setTemperatureOversampling(BME680_OS_8X);
   bme.setHumidityOversampling(BME680_OS_2X);
   bme.setPressureOversampling(BME680_OS_4X);
   bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
   bme.setGasHeater(320, 150); // 320*C for 150 ms
   
}


// Lets see if there's something waiting for us from the GPS.
void checkGPS(void) {

   char  aChar;
   
   if (GPSPort.available()) {
      aChar = GPSPort.read();
      //Serial.print(aChar);
      if (synk) {
         if (aChar=='\n') {
            inBuff[buffIndex] = '\0';
            synk = false;
            if (ourParser.parseStr(inBuff)) {
               //ourParser.showGPRMC();
               changeState(readingAir);
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


void showAir(void) {

   Serial.println();
   Serial.println("----------------");
      
   Serial.print("Temperature = ");
   Serial.print(bme.temperature);
   Serial.println(" *C");
   
   Serial.print("Pressure = ");
   Serial.print(bme.pressure / 100.0);
   Serial.println(" hPa");
   
   Serial.print("Humidity = ");
   Serial.print(bme.humidity);
   Serial.println(" %");
   
   Serial.print("Gas = ");
   Serial.print(bme.gas_resistance / 1000.0);
   Serial.println(" KOhms");
   
   Serial.print("Approx. Altitude = ");
   Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
   Serial.println(" m");

    Serial.println();
}


// Lets see if we can read the Air sensor.
void checkAir(void) {

   if (bme.performReading()) {
      //showAir();
   }
   changeState(readingGPS);
}


void changeState(states newState) {

   switch (newState) {
      case readingGPS : 
         while(GPSPort.available()) GPSPort.read();
         synk = false;
         ourState = readingGPS;
      break;
      case readingAir : ourState = readingAir; break;
   }
}


void loop() {
      
   switch (ourState) {
      case readingGPS      : checkGPS();     break;
      case readingAir      : checkAir();     break;
   }
   if (outTimer.ding()) {
      blink13();
      ourDataWriter->writeStream();
      outTimer.start();
   }
}
