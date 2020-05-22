#ifndef adafruit_1947_Obj_h
#define adafruit_1947_Obj_h

#include <Wire.h>      			// this is needed for FT6206
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
//#include <ILI9341_t3.h>
#include <Adafruit_FT6206.h>
#include <LC_SPI.h>
#include <baseGraphics.h>
#include <displayObj.h>

// Shield version
#define ADA_1947_SHIELD_CS		10
#define ADA_1947_SHIELD_RST	  -1
#define ADA_1947_SHIELD_SDCS	4

// Rotation
#define PORTRAIT      0  // USB up
#define LANDSCAPE     1  // USB left
#define INV_PORTRAIT  2  // USB down
#define INV_LANDSCAPE 3  // USB right

#ifndef swap
#define swap(a, b) { int16_t t = a; a = b; b = t; }
#endif

class adafruit_1947_Obj : public displayObj {

	public :
				adafruit_1947_Obj(void);						// Shield version.
				adafruit_1947_Obj(byte inCS,byte inRst);  // Breakout version.
	virtual	~adafruit_1947_Obj(void);
  
	virtual	bool	dispObjBegin(void);
	virtual	int	width(void);
	virtual	int	height(void);
	virtual	void	startWrite(void);
	virtual	void	endWrite(void);
	virtual	void	setRotation(byte inRotation);
	virtual	void	setTextColor(colorObj* tColor);
	virtual	void	setTextColor(colorObj* tColor,colorObj* bColor);
	virtual	void	setTextSize(byte inSize);
	virtual	void	setTextWrap(boolean wrap);
	virtual	void	setFont(const GFXfont* font);
	virtual	void	setCursor(int inX,int inY);
	virtual	void	drawText(char* inText);
	virtual	void	fillScreen(colorObj* inColor);
	virtual	void	fillRect(int x,int y,int width,int height,colorObj* inColor);
	virtual	void	drawRect(int x,int y,int width,int height,colorObj* inColor);
	virtual	void	fillRect(rect* inRect,colorObj* inColor);
	virtual	void	drawRect(rect* inRect,colorObj* inColor);
	virtual	void	fillRoundRect(int x,int y,int width,int height,int radius,colorObj* inColor);
	virtual	void	drawRoundRect(int x,int y,int width,int height,int radius,colorObj* inColor);
	virtual	void	drawCircle(int x,int y,int diam, colorObj* inColor);
	virtual	void	drawCircleHelper(int x,int y,int rad,byte corner,colorObj* inColor);
	virtual	void	fillCircle(int x,int y,int diam, colorObj* inColor);
	virtual	void	drawVLine(int x,int y,int height,colorObj* inColor);
	virtual	void	drawHLine(int x,int y,int width,colorObj* inColor);
	virtual	void	drawLine(int x,int y,int x2,int y2,colorObj* inColor);
	virtual	void	drawPixel(int x,int y,colorObj* pColor);
  
	virtual	point     getPoint(void);
	virtual	bool		  touched(void);
  
	private:
				Adafruit_FT6206*	cTS;
				Adafruit_ILI9341*	theTFT;
				//ILI9341_t3*			theTFT;
				byte					cs;
				byte					rst;
				bool					lastTouch;
				int					touchNum;
};

#endif