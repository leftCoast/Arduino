#ifndef navigation_h
#define navigation_h

#include <NMEA2kBase.h>
#include <GPSReader.h>
#include <GPS_NMEA2K.h>


//
// Forms for entering lat/lon
// three values : int float str : 38 26.2 east
// two values   : float str : 38.65934 west
//


enum engComs {

	getPos = lastCom,
	getCOG,
	getGPSData,
	setMarkLat,
	setMarklon,
	getCourse,
	getDist
};


class navigation : public	NMEA2kBase {

	public:
				navigation(void);
	virtual	~navigation(void);
	
	virtual	void	setup(void);
	virtual	void	loop(void);
	virtual	void	checkAddedComs(int comVal);
	virtual	bool	addNMEAHandlers(void);			// Without handlers, who are we anyway?
	virtual	void	addCommands(void);
	virtual	void	printHelp(void);					// Print list of commands.			
				void	doGetPos(void);
				void	doGetCOG(void);
				void	doGetData(void);
				void	doSetLat(void);
				void	doSetLon(void);
				void	doGetBearing(void);
				void	doGetDist(void);
				
				//GPSReader	ourGPS;
				globalPos	destMark;
};

			
#endif
