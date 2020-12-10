#ifndef GPS_NMEA_h
#define GPS_NMEA_h

enum quad { north, south, east, west };

struct $GPRMC_Data {

   int   hour;   
   int   min;
   int   sec;     // 1    = UTC of position fix
   bool  valid;   // 2    = Data status (A = OK, V = warning)
   int   degLat;
   float minLat;  // 3    = Latitude of fix
   quad  latQuad; // 4    = N or S
   int   degLon;
   float minLon;  // 5    = Longitude of fix
   quad  lonQuad; // 6    = E or W
   float knots;   // 7    = Speed over ground in knots
   float course;  // 8    = Track made good in degrees True
   int   day;
   int   month;
   int   year;    // 9    = UT date
   float magVar;  // 10   = Magnetic variation degrees (Easterly var. subtracts from true course)
   quad  varDir;  // 11   = E or W
};


class GPS_NMEA {

   public:
   
            GPS_NMEA(void);
   virtual  ~GPS_NMEA(void);

   bool  parseStr(char* inStr);
   bool  decodeGPRMC(char* inStr);
   void  showGPRMC(void);
   
   $GPRMC_Data $GPRMC;
};



#endif
