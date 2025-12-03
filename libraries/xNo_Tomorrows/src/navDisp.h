#ifndef navDisp_h
#define navDisp_h

#include <adafruit_2050.h>
#include <displayObj.h>
#include <fontLabel.h>
#include <label.h>
#include <colorRect.h>
#include <navigation.h>


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

// *********** erasableText ***********


class erasableText :	public fontLabel {

	public:
				erasableText(void);
				erasableText(rect* inRect);
				erasableText(int inX, int inY, int inW,int inH);
	virtual	~erasableText(void);
	
	virtual	void	drawSelf(void);	// Fixing up fontLabel..
};



// *************** LED  ***************


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



// ************* valueBox *************


// You get a number and units.
class valueBox	: public drawGroup {

	public:
				valueBox(int inX,int inY,int inWidth,int inHeight,const char* inLabel,int inPrec);
	virtual	~valueBox(void);

	virtual	void	setup(int inDataChoice=0);
	virtual	void	drawSelf(void);
	virtual	void	setValue(float inValue);
	virtual	float	checkData(void)=0;
	virtual	void	idle(void);
			
				char*				label;
				int				prec;
				float				factor;
				erasableText*	valueLabel;
				fontLabel*		unitsLabel;
				int				savedIntVal;
				timeObj*			updateTimer;
				bool				isNanNow;
				int				dataChoice;
};



// *************  NMEABox  *************


class NMEABox	: public valueBox {

	public:
				NMEABox(int inX,int inY,int inWidth,int inHeight,const char* inLabel,int inPrec);
	virtual	~NMEABox(void);

	virtual	float	checkData(void);
};



// *************  GPSBox  *************


class GPSBox	: public valueBox {

	public:
				GPSBox(int inX,int inY,int inWidth,int inHeight,const char* inLabel,int inPrec);
	virtual	~GPSBox(void);

	virtual	float	checkData(void);
};



// ************* navDisp  *************


class navDisp {

	public:
				navDisp(void);
	virtual	~navDisp(void);

				void	setup(void);
				void	showPos(globalPos* fix);
				
				LED*				fixLED;
				erasableText*	timeLabel;
				erasableText*	latLabel;
				erasableText*	lonLabel;
				NMEABox*			knotGauge;
				NMEABox*			depthGauge;
				GPSBox*			bearingGauge;
				GPSBox*			distanceGauge;
				NMEABox*			barometerGauge;
				char*				savedStamp;
};

				
#endif