#ifndef NMEA2kBase_h
#define NMEA2kBase_h

#include <llama2000.h>
#include <lilParser.h>
#include <handlers.h>


#define NMEA_ADDR_E_LOC	0


enum baseComs {
	noCommand,
   devList,
   showName,
   findName,
   copyName,
   pasteName,
   changeAddr,
   reset,
   help,
   lastCom
};


class NMEA2kBase {

	public:
				NMEA2kBase(uint32_t inDeviceID,byte inDeviceClass,byte inDeviceFunct);
	virtual	~NMEA2kBase(void);
	
	virtual	void	setup(void);
	virtual	void	loop(void);
	virtual	byte	getInitialAddr(void);			// Pull our initial address out of storage and return it.
	virtual	void	changeAddress(byte inAddr);	// Change our address to this. Then store it as next time's initial address.
	virtual	void	setupName(void);					// Set the earlier passed in values into our name.
	virtual	bool	addNMEAHandlers(void)=0;		// Without handlers, who are we anyway?
	virtual	void	addCommands(void);				// Inherit to add more commands.
				void	showDeviceList(void);			// Start device list gathering.
				void	checkDeviceList(void);			// Check to see if device list is ready.
				void	showDeviceName(void);			// Show our name.
				void	findNameFromAddr(void);			// Put in addr get back name, if found.
				void	copyNameFromAddr(void);			// Put in name save addr, if found.
				void	pasteNameToSelf(void);			// Copy name from saved addr to ourselves. (Because we are weird.)
				void	changeAnAddr(void);				// (One value) change our addr to this. (Two values) Change theirs to that.
				void	resetNameNAddr(void);			// Put back our name and address to power up state.
				void	error(int errNum);				// Show's parse error. (Not like you could actually fix it.)
	virtual	void	printHelp(void);					// Print list of commands.
	virtual	void	checkAddedComs(int comVal);	// For adding your own typed commands.

	
				llama2000*	llamaBrd;      // The class that inherits netObj, adding our attachmet to hardware.
				uint32_t		deviceID;		// You get 21 bits. Think serial number. You decide.
				byte			deviceSystem;	// See SAE_J1939.h for a list of device system #defines. Choose one.
				byte			deviceFunct;	// See SAE_J1939.h for list of functions of that system. Choose one.
				
				// Debug/setup command parser.
				lilParser	cmdParser;
				bool			gettingDevList;		// We busy waiting for the list to come in?
				timeObj*    devListTimer;  		// A timer to track time before calling the list "stale".
   			bool			devListNeedsRefresh;	// List is too old to bother with.
   			netName		ourName;					// We save a copy of our net name when starting up.
				byte			ourAddr;					// And our address.
				int			EEPROMUsed;				// Amount of bytes we use at the start of the EEPROM.
		     	netName		aName;					// A place to save a net name. (For copy/paste)

};



#endif