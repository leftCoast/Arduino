#ifndef wristDisplay_h
#define wristDisplay_h

#include <lilParser.h>
#include <DFRobot_0995_Obj.h>
#include <drawObj.h>
#include <idlers.h>
#include <label.h>
#include <fontLabel.h>
#include <colorBargraph.h>
#include <offscreen.h>


enum cmds {

	noCmd,
	setSpeedCmd,		// Boat speed though water.
	setDepthCmd,		// Water depth in fathoms.
	setBearingCmd,		// Magnetic bearing to next mark.
	setCOGCmd,			// Magnetic course over ground.
	setEngineCmd,		// Fuel%, RPM, Oil PSI alarm, Water temp alarm.
	helpCmd
};


class valueBox;


class wristDisplay {

	public:
				wristDisplay(void);
	virtual	~wristDisplay(void);
	
				void	setup(void);
				void	addCmd(cmds cmd,const char* text);
				void	setupDisplay(void);
				void	light(bool onOff);
				void	loop(void);
				void	checkUSBParser(void);
				void	checkWlessParser(void);
				void  checkCom(int cmd,bool debug);
				void  handleSetSpeed(bool debug);		// Sanity check values here. Much harder at the display level.
				void  handleSetDepth(bool debug);
				void  handleSetBearing(bool debug);
				void  handleSetCOG(bool debug);
				void  handleHelp(bool debug);
				
				lilParser*	USBParser;
				lilParser*	wlessParser;
				
				valueBox*	speed;
				valueBox*	depth;
				valueBox*	bearing;
				valueBox*	cog;
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
	
	protected:
	
	virtual	void	setup(void);
	virtual	void	drawSelf(void);
	virtual	void  draw(void);
	
				rect	oldVariableArea;
				rect	newVariableArea;
				int	position;
};



// **************************************************
// *****************    valueBox     **************** 
// **************************************************
//
// A box based on ISDataBox with label, units and a
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


#endif


