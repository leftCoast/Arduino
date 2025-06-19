#ifndef navigation_h
#define navigation_h

#include <NMEA2kBase.h>
#include <GPSReader.h>
#include <GPS_NMEA2K.h>

class navigation : public	NMEA2kBase {

	public:
				navigation(void);
	virtual	~navigation(void);
	
	virtual	void	setup(void);
	virtual	void	loop(void);
	virtual	bool	addNMEAHandlers(void);		// Without handlers, who are we anyway?
				
				GPSReader	ourGPS;
};

			
#endif
