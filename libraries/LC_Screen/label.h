#ifndef label_h
#define label_h

#include "drawObj.h"
#include "colorObj.h"

#define DEF_TEXT_SIZE	1	// If not set, what size do we want?
#define CHAR_WIDTH		6	// Actual size is 6 * text size.
#define CHAR_HEIGHT		8	// Actual size is 8 * text size.
#define DEF_PRECISION	2	// Number of digits past decimal point.

#define MAX_PREC			12					// #chars - 3 '-','0','.'. The calculator can disp 16.
#define TEMP_BUFF_SIZE	41 + MAX_PREC	// Temp buff for doing text formatting and things.
						  							// Think about what you'll need here. Its important!
						  							// float can get to 3.4xx E+38 add '-', '.' & \0 
						  							// gives 41 chars.

enum {
  TEXT_RIGHT,
  TEXT_LEFT,
  TEXT_CENTER
};


class label : public drawObj {

	public :
  				label();
  				label(char* inText);
  				label(char* inText, int inSize);
  				label(int inLocX, int inLocY, int inWidth,int inHeight);
  				label(int inLocX, int inLocY, int inWidth,int inHeight, char* inText);
  				label(int inLocX, int inLocY, int inWidth,int inHeight, char* inText,int textSize);
  				label(rect* inRect,char* inText,int textSize=1);
  				label(label* aLabel);
	virtual	~label(void);
  
  				void 	setTextSize(int size);  // 1,2,3.. - Ends up as multiples of 8 pixals.
  				void 	setJustify(int inJustify);
  				void 	setColors(colorObj* tColor);
				void 	setColors(colorObj* tColor, colorObj* bColor);
  				void 	setPrecision(int inPrec);
	virtual	void 	setValue(int val);
  				void 	setValue(unsigned long val);
  				void 	setValue(float val);
  				void 	setValue(double val);
  				void 	setValue(char* str);
				int	getNumChars(void);		// We want to know how long the string is..
				int	getViewChars(void);		// We want to know how many chars can we display?
				void	getText(char* inBuff);	// We asked above how much you have. Hand it over.
  				int 	getTextWidth(void);		// How wide in pixels is our text?
  				int 	getTextHeight(void);		// How tall in pixels are the charactors?
  
	virtual	void drawSelf(void);

//private :
	void initLabel(void);
  
	int		textSize;
	int		justify;
	colorObj	backColor;
	colorObj textColor;
	bool		transp;
	char*    buff;
	int      prec;      // For formatting floats.
};

#endif

