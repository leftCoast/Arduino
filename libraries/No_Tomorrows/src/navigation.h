#ifndef navigation_h
#define navigation_h

#include <NMEA2kBase.h>

class navigation : public	NMEA2kBase {

	public:
				navigation(void);
	virtual	~navigation(void);
	
	virtual	void	setup(void);
	virtual	void	loop(void);
	virtual	void	addNMEAHandlers(void);		// Without handlers, who are we anyway?
};
			
#endif
