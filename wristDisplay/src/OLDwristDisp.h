#ifndef wristDisp_h
#define wristDisp_h

#include <DFRobot_0995_Obj.h>
#include <drawObj.h>
#include <idlers.h>
#include <label.h>
#include <fontLabel.h>
#include <colorBargraph.h>
#include <offscreen.h>
#include <serialStr.h>


class valueBox;
class valueBarBox;
class fuelBargraph;
class engineBox;

extern void startSerial(void);

// **************************************************
// *****************   wristDisp    ***************** 
// **************************************************


enum dispErr {
	noErr,
	initErr,
	memErr
};


class wristDisp {

	public:
				wristDisp(int inChipSelect,int inReset,int inBacklight,int inDataCommand);
	virtual	~wristDisp(void);
	
	virtual	bool		begin(void);
	virtual	dispErr	errMsg(void);
	virtual	void		setupDisp(void);
				void		light(bool onOff);		
	virtual	void		checkDisp(void);
	
	protected:
				
				int				chipSelect;
				int				reset;
				int				backlight;
				int				dataCommand;
				dispErr			currentErr;
				valueBox*		speedBox;
				valueBox*		depthBox;
				valueBox*		COGBox;
				valueBox*		barometerBox;
				valueBarBox*	fuelBox;
				engineBox*		enginePanel;
};



// **************************************************
// *****************    dataBox     ***************** 
// **************************************************


#define DBOX_X	0
#define DBOX_Y	0
#define DBOX_W	TFT_WIDTH
#define DBOX_H	TFT_HEIGHT/4


class dataBox :	public drawGroup {

	public:
				dataBox(rect* inRect);
	virtual	~dataBox(void);
	
	protected:
	
	virtual	void	setup(void);
	virtual	void	drawSelf(void);
	virtual	void  draw(void);
	
				colorObj 	startColor;
				colorObj 	endColor;
				offscreen	vPort;
};



// **************************************************
// *****************    valueBox     **************** 
// **************************************************


class valueBox :	public dataBox {

	public:
				valueBox(rect* inRect);
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


// **************************************************
// *****************   valueBarBox   **************** 
// **************************************************	
	
	
class valueBarBox :	public dataBox {

	public:
				valueBarBox(rect* inRect);
	virtual	~valueBarBox(void);
	
	virtual	void	setTypeText(const char* inStr);
	virtual	void	setUnitText(const char* inStr);
	virtual	void	setValue(float inValue);
	
	protected:
	
	virtual	void	setup(void);
				
				fontLabel*		typeLabel;
				fuelBargraph*	valueBar;
				fontLabel*		theE;
				fontLabel*		theF;
				float				value;
};



// **************************************************
// *****************  fuelBargraph   **************** 
// **************************************************	


class fuelBargraph :	public colorBargraph {

	public:
				fuelBargraph(rect* inRect,orientation inOrientation=bottomUp);
	virtual	~fuelBargraph(void);
	
	virtual	void drawSelf(void);

};



// **************************************************
// *******************  engineBox  ****************** 
// **************************************************	


class idiotLight : public drawObj {

	public:
				idiotLight(rect* inRect);
	virtual	~idiotLight(void);
	
	virtual	void setActive(bool onOff);
	
				bool	active;		
};



#define THERMO_X	110
#define THERMO_Y	30
#define THERMO_W	18
#define THERMO_H	38
#define THERMO_W2	8

class tempLight :	public idiotLight {

	public:
				tempLight(rect* inRect);
	virtual	~tempLight(void);
	
	virtual	void drawSelf(void);			
};
	

#define OIL_X	THERMO_X + 40
#define OIL_Y	THERMO_Y
#define OIL_W	THERMO_W
#define OIL_H	THERMO_H
	
class oilLight :	public idiotLight {

	public:
				oilLight(rect* inRect);
	virtual	~oilLight(void);
	
	virtual	void drawSelf(void);			
};


// **************************************************
// ********************  tachObj  ******************* 
// **************************************************	


class tachObj :	public dataBox {

	public:
				tachObj(rect* inRect);
	virtual	~tachObj(void);
	
				void	setRPM(void);
	virtual	void	drawSelf(void);

};


class engineBox :	public dataBox {
	
	public:
				engineBox(rect* inRect);
	virtual	~engineBox(void);
	
	//virtual	void	drawSelf(void);
	
		
	protected:
	virtual	void	setup(void);
	
				tempLight*	ourTempLight;
				oilLight*	ourOilLight;
				tachObj*		ourTach;
};
	
#endif
