#ifndef cellCommon_h
#define cellCommon_h


enum cellCommands { getStatus, setCurrentPN, sendSNS, makeCall, hangUp, pickUp, setVolume, somethingElse  };

enum networkStatus { NS_notRegistered, NS_registeredHome, NS_registeredSearching, NS_denied, NS_unknown, NS_registeredRoaming };

enum callStatus	{ CS_ready, CS_noStatus, CS_ringingIn, CS_ringingOut, CS_unknown };

struct cellStatus {
	uint16_t		FONAOnline;
	uint16_t		batteryVolts;
	uint16_t		batteryPercent;
	uint16_t		RSSI;
	uint16_t		networkStat;
	uint16_t		volume;
	uint16_t		callStat;
	uint16_t		numSMSs;
	char			networkTime[24];
};
 

#endif