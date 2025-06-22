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


class wristDisplay {

	public:
				wristDisplay(void);
	virtual	~wristDisplay(void);
	
				void	setup(void);
				void	setupDisplay(void);
				void	light(bool onOff);
				void	checkUSBParser(void);
				void	checkWlessParser(void);
				void	loop(void);
				
				
				lilParser	USBParser;
				lilParser	wlessParser;
};


/*
// **************************************************
// ******************     disp     ****************** 
// **************************************************


class disp {

	public:
				disp(void);			//(int inChipSelect,int inReset,int inBacklight,int inDataCommand);
	virtual	~disp(void);
	
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

*/

#endif


