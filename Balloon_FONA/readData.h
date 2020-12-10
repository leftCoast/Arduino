#ifndef readData_h
#define readData_h

#include "numStream.h"

enum quad { north, south, east, west };

struct sensorData {
   int   hour;
   int   min;
   int   sec;
   int   valid;
   int   degLat;
   float minLat;
   quad  latQuad;
   int   degLon;
   float minLon;
   quad  lonQuad;
   float knots;
   float course;
   int   day;
   int   month;
   int   year;
   float magVar;
   int   varDir;
   float temp;
   float pressure;
   float humidity;
   float gas;
   float altitude;
};

extern sensorData ourData;


enum fields {  hourField,  minField, secField, validField, degLatField,
               minLatField, latQuadField, degLonField, minLonField, lonQuadField,
               knotsField, courseField, dayField, monthField, yearField,
               magVarField, varDirField,
       
               tempField, pressureField, humidityField, gasField, AltitudeField
            };


class dataIn : public numStreamIn {

   public :
               dataIn(void);
   virtual     ~dataIn(void);

   virtual  void  readVar(int index);
};


#endif
