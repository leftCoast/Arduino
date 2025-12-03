#ifndef wireless_h
#define wireless_h

#include <NMEA2kBase.h>


class wireless : public	NMEA2kBase {

	public:
				wireless(void);
	virtual	~wireless(void);
	
	virtual	void	setup(void);
	virtual	void	loop(void);
	virtual	bool	addNMEAHandlers(void);		// Without handlers, who are we anyway?
				
};

			
#endif
