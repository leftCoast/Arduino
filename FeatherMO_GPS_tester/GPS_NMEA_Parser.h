#ifndef GPS_NMEA_Parser_h
#define GPS_NMEA_Parser_h

#include <timeObj.h>
#include <idlers.h>

enum dataTypes {
  noType,
  GPRMC,
  GPGGA
};


enum quad { north, south, east, west };


void addMessage(char* inStr);
dataTypes getType(char* inStr);

class latLonAngle {
  
  public:

          latLonAngle(void);
  virtual ~latLonAngle(void);

  void    parseLatLon(char* token);
          
   int    deg;
   float  min;
   float  decDeg;
};


          
class baseNMEAParser : public idler {

  public:
          baseNMEAParser(void);
  virtual ~baseNMEAParser(void);

          
          bool      parseStr(char* inStr);
  virtual bool      doParse(char* inStr)=0; // Child classes need to fill this one out.
  virtual void      idle(void);
          float     getDataAge(void);
  virtual void      showData(void);
  
          timeObj     secTimer;
          float       secs;
          dataTypes   parseType;
          latLonAngle latLonParser;
};



// *******************************************
//               GPRMCParser
// *******************************************


class GPRMCParser : public baseNMEAParser {

  public:
          GPRMCParser(void);
  virtual ~GPRMCParser(void);
  
   virtual bool  doParse(char* inStr);
   virtual void  showData(void);
   
           int   hour;   
           int   min;
           int   sec;         // 1    = UTC of position fix
           bool  valid;       // 2    = Data status (A = OK, V = warning)
           int   degLat;
           float minLat;      // 3    = Latitude of fix
           float decDegLat;
           quad  latQuad;     // 4    = N or S
           int   degLon;
           float minLon;      // 5    = Longitude of fix
           float decDegLon;
           quad  lonQuad;     // 6    = E or W
           float knots;       // 7    = Speed over ground in knots
           float course;      // 8    = Track made good in degrees True
           int   day;
           int   month;
           int   year;        // 9    = UT date
           float magVar;      // 10   = Magnetic variation degrees (Easterly var. subtracts from true course)
           quad  varDir;      // 11   = E or W
};

extern GPRMCParser theGPRMCParser;



// *******************************************
//               GPGGAParser
// *******************************************


class GPGGAParser : public baseNMEAParser {

  public:
          GPGGAParser(void);
  virtual ~GPGGAParser(void);
  
   virtual bool  doParse(char* inStr);
   virtual void  showData(void);

};

extern GPGGAParser theGPGGAParser;

   
#endif
