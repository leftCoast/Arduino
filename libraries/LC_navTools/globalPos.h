#ifndef globalPos_h
#define globalPos_h

#include <Arduino.h>


enum quad {
	north,
	south,
	east,
	west
};


extern bool	checkLatDeg(int degrees);
extern bool	checkLonDeg(int degrees);
extern bool	checkMin(double minutes);
extern double rad2deg(double angleRad);
extern double deg2rad(double angleDeg);
//extern double hav(double theta);

// **********************************************
// ****************  globalPos  *****************
// **********************************************

#define	G_POS_BUFF_BYTES	40

class globalPos {

	public:
				globalPos(void);
	virtual	~globalPos(void);
	
				bool		valid(void);
				
				int		writeToEEPROM(int addr);
				int		copyFromEEPROM(int addr);
				
				void		copyPos(globalPos* aLatLon);
				void		copyLat(globalPos* aLatLon);
				void		copyLon(globalPos* aLatLon);
				
				void		setLatValue(const char* inLatStr);
				void		setLatQuad(const char* inQuad);
				void		setLonValue(const char* inLonStr);
				void		setLonQuad(const char* inQuad);
				
				void		setLat(double inLat);
				void		setLon(double inLon);
				
				void		setPosValues(const char* latStr,const char* lonStr);
				void		setQuads(const char*  inLatQuad,const char*  inLonQuad);
				
				void		setPos(double inLat, double inLon);
				void		setLatValue(int inLatDeg, double inLatMin);
				void		setLatQuad(quad inLatQuad);
				void		setLonValue(int inLonDeg, double inLonMin);
				void		setLonQuad(quad inLonQuad);
				void		setQuads(quad inLatQuad,quad inLonQuad);
				
				void		setPosition(int inLatDeg, double inLatMin, quad inLatQuad, int inLonDeg, double inLonMin, quad inLonQuad);
				
				double	trueBearingTo(globalPos* inDest);
				double	distanceTo(globalPos* inDest);
				
				char*		getLatStr(void);
				char*		getLatQuadStr(void);
				char*		getLonStr(void);
				char*		getLonQuadStr(void);
				
				char*		showLatStr(void);			// Formatted for humans.
				char*		showLonStr(void);			// This one too.
				
				int		getLatDeg(void);
				double	getLatMin(void);
				quad		getLatQuad(void);
				int		getLonDeg(void);
				double	getLonMin(void);
				quad		getLonQuad(void);
				double	getLatAsDbl(void);		// These last six kinda' need a 32 bit processer.
				double	getLonAsDbl(void);		// Otherwise you may run into rounding errors.
				int32_t	getLatAsInt32(void);		// For NMEA2k messages.
				int32_t	getLonAsInt32(void);		// For NMEA2k messages.
				int64_t	getLatAsInt64(void);		// For NMEA2k messages.
				int64_t	getLonAsInt64(void);		// For NMEA2k messages.
				
	protected:
				int		latDeg;
				double	latMin;
				quad		latQuad;
				int		lonDeg;
				double	lonMin;
				quad		lonQuad;
				char		outStr[G_POS_BUFF_BYTES];
};


#endif