#ifndef cellCommon_h
#define cellCommon_h


enum cellCommands { getStatus, setCurrentPN, sendSMS, getSMS, makeCall, hangUp, pickUp, setVolume, chooseRing, touchTone, sweepUNS, somethingElse  };

enum networkStatus { NS_notRegistered, NS_registeredHome, NS_registeredSearching, NS_denied, NS_unknown, NS_registeredRoaming };

enum callStatus  { CS_ready, CS_noStatus, CS_unknown, CS_ringingIn, CS_ringingOut };

struct cellStatus {
  uint16_t    FONAOnline;
  uint16_t    batteryVolts;
  uint16_t    batteryPercent;
  uint16_t    RSSI;
  uint16_t    networkStat;
  uint16_t    volume;
  uint16_t    callStat;
  uint16_t    numSMSs;
  char        networkTime[24];
  uint16_t    errByte;
};
 
 
struct tone {
  uint16_t  freq;
  uint16_t  durationMS;
};


#endif
