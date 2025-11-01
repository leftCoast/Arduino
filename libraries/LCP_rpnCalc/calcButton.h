#ifndef calcButton_h
#define calcButton_h

#include <rpnCalc.h>

#define NUMBER_BTN  0
#define EDIT_BTN    1
#define FX_BTN      2
#define DEG_RAD_BTN 3


class rpnCalc;

class calcButton : public drawObj, public idler {
  
	public :
				calcButton(const char* inFStr,word inLocX, word inLocY,byte width,byte inType,rpnCalc* inCalc);
				calcButton(const char* inFStr,const char* inAFStr, word inLocX, word inLocY,byte width,byte inType,rpnCalc* inCalc);
	virtual	~calcButton(void);
                  
	virtual	void	idle(void);
	virtual	void	drawSelf(void);
	virtual	void	doAction(void); 
		 
	protected :
				char*		fStr;
				char*		aFStr;
				byte		bType;
				bool		saved2ndFx;
				rpnCalc* ourCalc;
};


class secondfxButton : public calcButton {

	public :
				secondfxButton(const char* inFStr,word inLocX, word inLocY,byte width,byte inType,rpnCalc* inCalc);
	virtual	~secondfxButton(void);
  
	virtual	void	doAction(void);

};
  

class degRadButton : public calcButton {

	public :
				degRadButton(word inLocX, word inLocY,byte width, byte height,rpnCalc* inCalc);
	virtual	~degRadButton(void);
  
				void	setColors(colorObj* inTColor,colorObj* inHTColor,colorObj* inBColor);
	virtual	void	drawSelf(void);
	virtual	void	doAction(void);

				colorObj	tColor;
				colorObj hColor;
				colorObj bColor;
};


#endif
