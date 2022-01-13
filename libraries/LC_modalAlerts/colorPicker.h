#ifndef colorPicker_h
#define colorPicker_h

#include <alertObj.h>
#include <slider.h>
#include <colorRect.h>

class colorPicker :	public alertObj {

	public:
				colorPicker(listener* inListener);
	virtual	~colorPicker(void);
	
	virtual	void    	setColor(colorObj* inColor);
	virtual	void    	idle(void);
	virtual	void    	handleCom(stdComs comID);
	virtual	colorObj	result(void);
	
	    
				slider*		redSlider;
				slider*		greenSlider;
				slider*		blueSlider;
				colorRect*	swatch;
				mapper		percentToByte;
				mapper		byteToPercent;
				timeObj		changeTimer;
				byte			savedRed;
				byte			savedGreen;
				byte			savedBlue;
};
	
#endif