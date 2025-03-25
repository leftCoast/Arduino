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
				
				int			chipSelect;
				int			reset;
				int			backlight;
				int			dataCommand;
				dispErr		currentErr;
				valueBox*	speedBox;
				valueBox*	depthBox;
				valueBox*	COGBox;
				//valueBox*	fuelBox;
				valueBarBox*	fuelBox;
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
				char*			noValueStr;
				
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
	//virtual	void  drawSelf(void);
	
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

#endif
