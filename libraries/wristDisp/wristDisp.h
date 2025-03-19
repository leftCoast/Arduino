#ifndef wristDisp_h
#define wristDisp_h


#include <DFRobot_0995_Obj.h>
#include <drawObj.h>
#include <idlers.h>
#include <label.h>


#define TFT_DC  9
#define TFT_CS  10
#define TFT_RST 3
#define TFT_BL  5

class valueBox;



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
				valueBox*	fuelBox;
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
	
};



// **************************************************
// *****************    valueBox     **************** 
// **************************************************


class valueBox :	public dataBox {

	public:
				valueBox(rect* inRect);
	virtual	~valueBox(void);
	
				void	setTypeText(const char* inStr);
				void	setUnitText(const char* inStr);
				void	setValue(float inValue);
				void	setPrecision(int inPrecision);
				void	setNoValueStr(const char* inStr);				
			
	protected:
	
	virtual	void	setup(void);
				
				label*	typeLabel;
				label*	valueLabel;
				label* 	unitsLabel;
				float		value;
				int		precision;
				char*		noValueStr;
};


	
	
	


#endif
