#ifndef navDisp_h
#define navDisp_h

#include <adafruit_2050.h>
#include <displayObj.h>
#include <idlers.h>
#include <globalPos.h>
#include <label.h>

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



class navDisp : public idler {

	public:
				navDisp(void);
	virtual	~navDisp(void);

				void	setup(void);
				void	showPos(globalPos* fix);
	virtual	void	idle(void);
				
				label*	latLabel;
				label*	lonLabel;
};


#endif