#include "readData.h"

sensorData ourData;

dataIn::dataIn(void)
   : numStreamIn() {  }

   
dataIn::~dataIn(void) {  }


void dataIn::readVar(int index) {
   Serial.print("index : ");Serial.println(index);
   switch((fields)index) {
      case hourField    : ourData.hour    = atoi(mTokenBuff);           break;
      case minField     : ourData.min     = atoi(mTokenBuff);           break;
      case secField     : ourData.sec     = atoi(mTokenBuff);           break;
      case validField   : ourData.valid   = atoi(mTokenBuff);           break;
      case degLatField  : ourData.degLat  = atoi(mTokenBuff);           break;
      case minLatField  : ourData.minLat  = atof(mTokenBuff);           break;
      case latQuadField : ourData.latQuad = (quad)(atoi(mTokenBuff));   break;
      case degLonField  : ourData.degLon  = atoi(mTokenBuff);           break;
      case minLonField  : ourData.minLon  = atof(mTokenBuff);           break;
      case lonQuadField : ourData.lonQuad = atoi(mTokenBuff);           break;
      case knotsField   : ourData.knots   = atof(mTokenBuff);           break;
      case courseField  : ourData.course  = atof(mTokenBuff);           break;
      case dayField     : ourData.day     = atoi(mTokenBuff);           break;
      case monthField   : ourData.month   = atoi(mTokenBuff);           break;
      case yearField    : ourData.year    = atoi(mTokenBuff);           break;
      case magVarField  : ourData.magVar  = atof(mTokenBuff);           break;
      case varDirField  : ourData.varDir  = atof(mTokenBuff);           break;

      case tempField       : ourData.temp       = atof(mTokenBuff);         break;
      case pressureField   : ourData.pressure   = atof(mTokenBuff);         break;
      case humidityField   : ourData.humidity   = atof(mTokenBuff);         break;
      case gasField        : ourData.gas        = atof(mTokenBuff);         break;
      case AltitudeField   : 
         ourData.altitude = atof(mTokenBuff);
         mComplete = true;
         Serial.println("complete?");
      break;
   }
}
