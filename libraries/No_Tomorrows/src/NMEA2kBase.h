#ifndef NMEA2kBase_h
#define NMEA2kBase_h

#include <llama2000.h>
#include <lilParser.h>

class NMEA2kBase {

	public:
				NMEA2kBase(uint32_t inDeviceID,byte inDeviceClass,byte inDeviceFunct);
	virtual	~NMEA2kBase(void);
	
	virtual	void	setup(void);
	virtual	void	loop(void);
	virtual	byte	getInitialAddr(void);			// Method to return the initial address.
	virtual	void	addressChanged(void);			// Typically, save it as next time's initial address.
	virtual	void	setupName(void);
	virtual	void	addNMEAHandlers(void)=0;		// Without handlers, who are we anyway?
	virtual	void	addCommands(void);
	virtual	void	checkAddedComs(int comVal);	// For adding your own typed commands.

	
				llama2000*	llamaBrd;      // The class that inherits netObj, adding our attachmet to hardware.
				uint32_t		deviceID;		// You get 21 bits. Think serial number. You decide.
				byte			deviceClass;	// See SAE_J1939.h for a list of device class #defines. Choose one.
				byte			deviceFunct;	// See SAE_J1939.h for list of functions of that class. Choose one.
				lilParser	cmdParser;
};



#endif