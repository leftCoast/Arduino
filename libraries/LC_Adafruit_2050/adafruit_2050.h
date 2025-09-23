#ifndef LC_Adafruit_2050_h
#define LC_Adafruit_2050_h


#include <Adafruit_GFX.h>
#include <Adafruit_HX8357.h>
#include <LC_SPI.h>
#include <baseGraphics.h>
#include <displayObj.h>
#include <mask.h>


class maskableHX8357 :	public Adafruit_HX8357 {
								
	public :
				maskableHX8357(int inCS, int inDC, int inRst);
	virtual	~maskableHX8357(void);
	
	virtual	void drawPixel(int16_t x, int16_t y, uint16_t color);
	virtual	void writePixel(int16_t x, int16_t y, uint16_t color);
	virtual	void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
};



class adafruit_2050 : public displayObj {

	public :
				adafruit_2050(int inCS,int inDC,int inRst);
	virtual	~adafruit_2050(void);
  
	virtual	bool	begin(void);
	virtual	int	width(void);
	virtual	int	height(void);
	virtual	void	startWrite(void);
	virtual	void	endWrite(void);
	virtual	void	setRotation(byte inRotation);
	virtual	void	setTextColor(colorObj* tColor);
	virtual	void	setTextColor(colorObj* tColor,colorObj* bColor);
	virtual	void	setTextSize(byte inSize);
	virtual	void	setTextWrap(boolean wrap);
	virtual	rect	getTextRect(const char* inText);
	virtual	void	setFont(const GFXfont* font);
	virtual	void	setCursor(int inX,int inY);
	virtual	void	drawText(const char* inText);
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
	virtual	void	drawTriangle(point* pt0,point* pt1,point* pt2,colorObj* inColor);
	virtual	void	fillTriangle(point* pt0,point* pt1,point* pt2,colorObj* inColor);
	virtual	void	drawVLine(int x,int y,int height,colorObj* inColor);
	virtual	void	drawHLine(int x,int y,int width,colorObj* inColor);
	virtual	void	drawLine(int x,int y,int x2,int y2,colorObj* inColor);
	virtual	void	drawPixel(int x,int y,colorObj* pColor);

	virtual	point	getPoint(void);
	virtual	bool	touched(void);
  
	private:
				maskableHX8357*	theTFT;
				byte					cs;
				byte					dc;
				byte					rst;
				bool					lastTouch;
				int					touchNum;
				point					lastTouchPt;
};


#endif