#include "GPS_NMEA.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include "GPSnAirCommon.h"

#define DEF_BUFF_SIZE  80
#define SEALEVELPRESSURE_HPA (1013.25)


enum states { readingAir, readingGPS };

Adafruit_BME680   bme; // I2C
states            ourState;
bool              synk;
char              inBuff[DEF_BUFF_SIZE];
int               buffIndex;
GPS_NMEA          ourParser;
sensorData        ourDatablock;


void setup() {
   
   Serial1.begin(9600);
   Serial3.begin(9600);
   synk  = false;
   buffIndex = 0;
   ourState = readingGPS;

   bme.begin();
   // Set up oversampling and filter initialization
   bme.setTemperatureOversampling(BME680_OS_8X);
   bme.setHumidityOversampling(BME680_OS_2X);
   bme.setPressureOversampling(BME680_OS_4X);
   bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
   bme.setGasHeater(320, 150); // 320*C for 150 ms
}


// Lets see if there's something waiing for us from the GPS.
void checkGPS(void) {

   char  aChar;
   
   if (Serial3.available()) {
      aChar = Serial3.read();
      //Serial.print(aChar);
      if (synk) {
         if (aChar=='\n') {
            inBuff[buffIndex] = '\0';
            synk = false;
            if (ourParser.parseStr(inBuff)) {
               //ourParser.showGPRMC();
               fillDataGPS();
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

void fillDataGPS(void) {

   ourDatablock.hour = ourParser.$GPRMC.hour;
   ourDatablock.min = ourParser.$GPRMC.min;
   ourDatablock.sec  = ourParser.$GPRMC.sec;
   ourDatablock.valid = (uint16_t)ourParser.$GPRMC.valid;
   ourDatablock.degLat = ourParser.$GPRMC.degLat;
   ourDatablock.minLat = ourParser.$GPRMC.minLat;
   ourDatablock.minDecLatX1000 = (ourParser.$GPRMC.minLat - ourDatablock.minLat) * 1000;
   ourDatablock.northSouthLat = (uint16_t)ourParser.$GPRMC.latQuad;
   ourDatablock.degLon = ourParser.$GPRMC.degLon;
   ourDatablock.minLon = ourParser.$GPRMC.minLon;
   ourDatablock.minDecLonX1000 = (ourParser.$GPRMC.minLon - ourDatablock.minLon) * 1000;
   ourDatablock.eastWestLon = (uint16_t)ourParser.$GPRMC.lonQuad;
   ourDatablock.knotsX100 = ourParser.$GPRMC.knots * 100;
   ourDatablock.headingX100 = ourParser.$GPRMC.course * 100;
 }


 void fillDataAIR(void) {

   ourDatablock.tempX100 = bme.temperature * 100;
   ourDatablock.pressureX100 = bme.pressure;
   ourDatablock.humidityX100 = bme.humidity * 100;
   ourDatablock.gasKOhmsX1000 = bme.gas_resistance;
   ourDatablock.AltMX100 = bme.readAltitude(SEALEVELPRESSURE_HPA) * 100;
 }
 

void showAir(void) {

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
}


// Lets see if we can read the Air sensor.
void checkAir(void) {

   if (bme.performReading()) {
      fillDataAIR();
      //showAir();
   }
   changeState(readingGPS);
}


void changeState(states newState) {

   switch (newState) {
      case readingGPS : 
         while(Serial3.available()) Serial3.read();
         synk = false;
         ourState = readingGPS;
      break;
      case readingAir : ourState = readingAir; break;
   }
}


void loop() {
   
   char  aChar;
   
   switch (ourState) {
      case readingGPS      : checkGPS();     break;
      case readingAir      : checkAir();     break;
   }
   if (Serial1.available()) {
      aChar = Serial1.read();
      if (aChar=='R') {
         Serial1.write((char*)&ourDatablock,sizeof(sensorData));
      }
   }
}
