#ifndef dataLog_h
#define dataLog_h

#include <SD.h>
#include "parameters.h"

#define LOG_FILE  "/logFile.txt"  // Our logfile pathname.


class dataLog {

  public:
                dataLog(void);
  virtual       ~dataLog(void);

          void          begin(bool haveSD);
          bool          isLogging(void);
          bool          setLogging(bool onOff);
          void          deleteLog(void);
          void          saveDataRecord(void);
          void          clearFlags(void);
          void          showLogfile(void);
          unsigned long getFileSize(void);
          unsigned long getFileBuff(unsigned long index,unsigned long buffBytes,byte* buff);
          
          void  addMode(int mode);
          void  addLimit(int limit);
          void  addWaterSec(int wTime);
          void  addSoakSec(int wSoak);
              
          void  addRawCap(uint16_t rCap);
          void  addRawTemp(float rTemp);
          void  addAveCap(uint16_t aCap);
          void  addAveTemp(float aTemp);
          void  addMoisture(float moisture);

          File      logFile;    // Our data logging file.
          bool      mHaveSD;
          
          int       mMode;      bool modeSet;
          int       mLimit;     bool limitSet;
          int       mWTime;     bool wTimeSet;
          int       mSTime;     bool sTimeSet;
          
          uint16_t  mRCap;      bool rCapSet;
          float     mRTemp;     bool rTempSet;
          uint16_t  mACap;      bool aCapSet;
          float     mATtemp;    bool aTempSet;
          float     mMoisture;  bool moistureSet;              
};


#endif
