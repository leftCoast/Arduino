#ifndef wristDisplay_h
#define wristDisplay_h

#include <lilParser.h>
#include <DFRobot_0995_Obj.h>
#include <drawObj.h>
#include <idlers.h>
#include <label.h>
#include <fontLabel.h>
#include <bargraph.h>
#include <offscreen.h>
#include <flasher.h>


enum cmds {

	noCmd,
	setOrderCmd,		// Choose four data boxes and set their order.
	setSpeedCmd,		// Boat speed though water.
	setDepthCmd,		// Water depth in fathoms.
	setBearingCmd,		// Magnetic bearing to next mark.
	setCOGCmd,			// Magnetic course over ground.
	setEngineCmd,		// Fuel%, RPM, Oil PSI alarm, Water temp alarm.
	helpCmd
};

class OSDataBox;
class valueBox;
class engineBox;

class wristDisplay {

	public:
				wristDisplay(void);
	virtual	~wristDisplay(void);
	
				void			setup(void);
				void			addCmd(cmds cmd,const char* text);
				void			setupDisplay(void);
				void			light(bool onOff);
				void			loop(void);
				void			checkUSBParser(void);
				void			checkWlessParser(void);
				void  		checkCom(int cmd,bool debug);
				OSDataBox*	selectdBox(const char* inStr);
				void  		handleSetOrder(bool debug);
				void  		handleSetSpeed(bool debug);		// Sanity check values here. Much harder at the display level.
				void  		handleSetDepth(bool debug);
				void  		handleSetBearing(bool debug);
				void  		handleSetCOG(bool debug);
				void  		handleHelp(bool debug);
				
				lilParser*	USBParser;
				lilParser*	wlessParser;
				
				valueBox*	speed;
				valueBox*	depth;
				valueBox*	bearing;
				valueBox*	cog;
				engineBox*	engine;
};



// **************************************************
// *****************   OSDataBox    ***************** 
// **************************************************
//
// Base rectangle box that defaults to a color sweep
// background, and is drawn offline.

class OSDataBox :	public drawGroup {

	public:
				OSDataBox(void);
	virtual	~OSDataBox(void);
	
	virtual	void	setPos(int newPos);
	virtual	void	setDrawRect(rect* inRect);
	
	protected:
	
	virtual	void	setup(void);
	virtual	void	drawSelf(void);
	virtual	void  draw(void);
	virtual	void  timedOut(void);
	virtual	void	idle(void);
	virtual	void	setTimeout(float inMs);
	
				rect		drawRect;
				bool		useRect;
				int		position;
				timeObj	timeoutTimer;
};



// **************************************************
// *****************    valueBox     **************** 
// **************************************************
//
// A box based on OSDataBox with label, units and a
// numeric value.
//
class valueBox :	public OSDataBox {

	public:
				valueBox(void);
	virtual	~valueBox(void);
	
	virtual	void	setTypeText(const char* inStr);
	virtual	void	setUnitText(const char* inStr);
	virtual	void	setValue(float inValue);
	virtual	void	setPrecision(int inPrecision);
	virtual	void	setNoValueStr(const char* inStr);
	virtual	void  timedOut(void);
			
	protected:
	
	virtual	void	setup(void);
				
				fontLabel*	typeLabel;
				fontLabel*	valueLabel;
				fontLabel* 	unitsLabel;
				float			value;
				int			precision;
				int			decMult;
				char*			noValueStr;
				bool			sawNAN;			
};



// **************************************************
// *****************   engineBox   ****************** 
// **************************************************


class idiotLight :	public flasher {

	public:
				idiotLight(rect* inRect,OSDataBox* inBox);
	virtual	~idiotLight(void);
	
	virtual	void setActive(bool onOff);
	virtual	void	pulseOn(void);
	virtual	void	pulseOff(void);
	
				OSDataBox*	ourBox;
		
};



#define THERMO_X	110
#define THERMO_Y	30
#define THERMO_W	18
#define THERMO_H	38
#define THERMO_W2	8

class tempLight :	public idiotLight {

	public:
				tempLight(rect* inRect,OSDataBox* inBox);
	virtual	~tempLight(void);
	
	virtual	void drawSelf(void);			
};
	

#define OIL_X	THERMO_X + 40
#define OIL_Y	THERMO_Y
#define OIL_W	THERMO_W
#define OIL_H	THERMO_H
	
class oilLight :	public idiotLight {

	public:
				oilLight(rect* inRect,OSDataBox* inBox);
	virtual	~oilLight(void);
	
	virtual	void drawSelf(void);			
};


class tachObj :	public drawGroup {

	public:
				tachObj(rect* inRect);
	virtual	~tachObj(void);
	
				void	setRPM(void);
	virtual	void	drawSelf(void);

};


class engineBox :	public OSDataBox {
	
	public:
				engineBox(void);
	virtual	~engineBox(void);
	
	//virtual	void	drawSelf(void);
	
		
	protected:
	virtual	void	setup(void);
	
				tempLight*	ourTempLight;
				oilLight*	ourOilLight;
				tachObj*		ourTach;
};


#endif


