#ifndef navDisp_h
#define navDisp_h

#include <adafruit_2050.h>
#include <displayObj.h>
#include <idlers.h>
#include <globalPos.h>
#include <fontLabel.h>
#include <label.h>
#include <colorRect.h>


#define SD_CS        4		// wht
#define SD_Detect    5		// grn
#define LC_DC			9		// Data/command	- YELLOW WIRE 30AWG
#define SCREEN_RST   14		// red
#define SCREEN_LED   15		// blu
#define BEEP_PIN		23		// none yet
#define SCREEN_CS		25		// blk


// Y+ to Arduino A2
// X+ to Arduino D8
// Y- to Arduino D7
// X- to Arduino A3

// GPS Serial1
// RX 0
// TX 1



// ************* valueBox *************

class erasibleText :	public fontLabel {

	public:
				erasibleText(void);
				erasibleText(rect* inRect);
				erasibleText(int inX, int inY, int inW,int inH);
	virtual	~erasibleText(void);
	
	virtual	void	drawSelf(void);	// Fixing up fontLabel..
};


// You get a number and units.
class valueBox	: public drawGroup {

	public:
				valueBox(int inX,int inY,int inWidth,int inHeight,const char* inLabel,int inPrec);
	virtual	~valueBox(void);

	virtual	void	setup(float* inSource=NULL);
	virtual	void	drawSelf(void);
	virtual	void	setValue(float inValue);
	virtual	void	idle(void);
			
				char*				label;
				int				prec;
				erasibleText*	valueLabel;
				fontLabel*		unitsLabel;
				float*			dataSource;
				int				savedIntVal;
				timeObj*			updateTimer;
				bool				isNanNow;
};



class LED :	public colorRect {

	public:
				LED(rect* inRect,colorObj* inOnColor,colorObj* inOffColor);
	virtual	~LED(void);
	
	virtual	void	setColors(colorObj* inOnColor,colorObj* inOffColor);
	virtual	void	setState(bool onOff);
	virtual	void	drawSelf(void);
	
				colorObj	offColor;
				colorObj	onColor;
				bool		ourState;
};


class navDisp {

	public:
				navDisp(void);
	virtual	~navDisp(void);

				void	setup(void);
				void	showPos(globalPos* fix);
				
				LED*		fixLED;
				
				label*	latLabel;
				label*	lonLabel;
				label*	timeLabel;
				
				valueBox*	knotGauge;
				valueBox*	depthGauge;
				valueBox*	bearingGauge;
				valueBox*	distanceGauge;
				valueBox*	barometerGauge;
				timeObj*		updateTimer;
};





				
				
				
				
#endif