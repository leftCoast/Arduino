#ifndef navigation_h
#define navigation_h

#include <NMEA2kBase.h>
#include <GPSReader.h>
#include <GPS_NMEA2K.h>
#include <handlers.h>
#include <RTClib.h>
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
	
	virtual	void		setup(void);
	virtual	void		loop(void);
				bool		haveMark(void);
				float		bearing(void);
				float		distance(void);
				float		inHg(void);
	virtual	void		checkAddedComs(int comVal);
	virtual	bool		addNMEAHandlers(void);			// Without handlers, who are we anyway?
	virtual	void		addCommands(void);
	virtual	void		printHelp(void);					// Print list of commands.		
				void		doGetPos(void);
				void		doGetCOG(void);
				void		doGetData(void);
				void		doSetLat(void);
				void		doSetLon(void);
				void		doGetBearing(void);
				void		doGetDist(void);
				
				bool				haveMarkLat;
				bool				haveMarkLon;
				globalPos		destMark;
				barometerObj*	barometer;
				waterSpeedObj*	knotMeter;
				waterDepthObj*	depthSounder;
				fluidLevelObj*	fuelGauge;
				engParam*		engHdler;
				timeObj     	timer;
				int				hoursOffUTC;
};


extern navigation  ourNavApp;
			
#endif
