#ifndef engine_h
#define engine_h

#include <NMEA2kBase.h>
#include <mechButton.h>
#include <tachMgr.h>
#include <blinker.h>


#define OIL_PIN      		20
#define TEMP_PIN     		21
#define TACH_IN_PIN  		22
#define TACH_OUT_PIN 		23
#define BEEP_OUT_PIN 		16    // Purple on tycho's test board. High for beep.
#define FUTURE_PIN   		17
#define TEETH_ROTATE 		97
#define WAIT_TIME_MS 		1000  // One second
#define ENG_ALARM_PERIOD	500
#define ENG_ALARM_PULSE		150
#define MIN_RPM				500

enum engComs {

	setRPM = lastCom,
	setMs,
	setFuel,
	readFuel,
	setAuto
};


class engine : public	NMEA2kBase {

	public:
				engine(void);
	virtual	~engine(void);
	
	virtual	void	setup(void);
	virtual	void	loop(void);
	virtual	bool	addNMEAHandlers(void);			// Without handlers, who are we anyway?
	virtual	void	addCommands(void);				// Inherit to add more commands.
	virtual	void	checkAddedComs(int comVal);	// For adding your own typed commands.
	virtual	void	printHelp(void);					// Print list of commands.
				void	doSetRPM(void);					// Commmand handlers
				void	doSetMS(void);
				void	doSetFuel(void);
				void	doReadFuel(void);
				void	doSetAuto(void);
				
				tachMgr*			ourTachMgr;
				mechButton*		oilPSI_Sender;
				mechButton*		tempSender;
				blinker*			ourBeeper;
				timeObj*			updateTimer;
				
				engParam*		engHdler;
				engParamII*		engHdlerII;
				fluidLevelObj*	fuelHdlr;
};
	
#endif