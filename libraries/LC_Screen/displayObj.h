#ifndef displayObj_h
#define displayObj_h

#include <Arduino.h>
#include <gfxfont.h>
#include <colorObj.h>
#include <baseGraphics.h>
#include <bitmap.h>

// This is going to by our base display object. Similar to adafruit's GFX object.
// The plan is to inherit this for whatever glue code each piece of 
// hardware needs to run.


class displayObj {    

	public :
				displayObj(bool inGraphic,bool inColor,bool inTouch,bool inSD,bool inReadable);
	virtual	~displayObj(void);

				void	pushOffset(int x, int y);
				void	popOffset(int x, int y);
				int	gX(int x);
				int	gY(int y);
				point	lP(point gP);
					
	virtual	int	width(void);
	virtual	int	height(void);
	virtual	bool	dispObjBegin(void);
  	virtual	void	startWrite(void);
  	virtual	void	endWrite(void);
	virtual	void	setRotation(byte inRotation);
	virtual	void	setTextColor(colorObj* tColor);
	virtual	void	setTextColor(colorObj* tColor,colorObj* bColor);
	virtual	void	setTextSize(byte inSize);
	virtual	void	setTextWrap(bool inWrap);
	virtual	void	setFont(const GFXfont* font);
	virtual	void	setCursor(int inX,int inY);
	virtual	void	drawText(char* inText);
	virtual	void  drawChar(int x,int y,char inChar,colorObj* fColor,colorObj* bColor,int size);
	virtual	void	fillScreen(colorObj* inColor);
	virtual	void	fillRect(int locX,int locY,int width,int height, colorObj* inColor);
	virtual	void	fillRect(rect* inRect,colorObj* inColor);
	virtual	void	drawRect(rect* inRect,colorObj* inColor);
	virtual	void	drawRect(int locX,int locY,int width,int height, colorObj* inColor);
	virtual	void	fillRectGradient(int inX,int inY,int inXSize,int inYSize,colorObj* startColor,colorObj* endColor,bool vertical=true);
	virtual	void	fillScreenGradient(colorObj* startColor,colorObj* endColor,bool vertical=true);
	virtual	void	fillRoundRect(int locX,int locY,int width,int height,int radius,colorObj* inColor);
	virtual	void	drawRoundRect(int locX,int locY,int width,int height,int radius,colorObj* inColor);
	virtual	void	fillRoundRect(rect* inRect,int radius,colorObj* inColor);
	virtual	void	drawRoundRect(rect* inRect,int radius,colorObj* inColor);
	virtual	void	drawCircle(int locX,int locY,int inDiam, colorObj* inColor);
	virtual	void	drawCircleHelper(int locX,int locY,int inRad,byte corner,colorObj* inColor);
	virtual	void	fillCircle(int locX,int locY,int inDiam, colorObj* inColor);
	virtual	void	drawVLine(int locX,int locY,int height,colorObj* inColor);
	virtual	void	drawHLine(int locX,int locY,int width,colorObj* inColor);
	virtual	void	drawLine(int locX,int locY,int locX2,int locY2,colorObj* inColor);
	virtual	void	drawLine(point* startPt,point* endPt,colorObj* inColor);
	virtual	void	drawPixel(int locX,int locY,colorObj* pColor);
   virtual	void	blit(int locX,int locY,bitmap* inBitmap);
   
	virtual	void	drawPixelInvert(int x,int y);
	virtual	void	frameRectInvert(int x,int y,int width,int height);

	virtual	point	getPoint(void);
	virtual	bool	touched(void);
  
  				bool isGraphic(void);
  				bool isColor(void);
  				bool hasTouchScreen(void);
  				bool hadSDDrive(void);
  				bool canRead(void);
  
  protected:
  				int		offsetX;
  				int		offsetY;
				bool		graphic;
				bool		color;
				bool		hasTouch;
				bool		hasSD;
				bool		readable;
};

#endif

