#ifndef offscreen_h
#define offscreen_h

#include "bitmap.h"
#include "displayObj.h"
#include "Adafruit_GFX.h"


class mapDisplay :  public Adafruit_GFX {
  
  public :
            mapDisplay(bitmap* inMap,int ofsX,int ofsY);
    virtual ~mapDisplay(void);
          
            bool  dispObjBegin(void);
    virtual void  drawPixel(int16_t x, int16_t y, uint16_t color);

  protected :
  				int		offsetX;
				int		offsetY;
            colorObj	mFColor;
            colorObj	mBColor;
            bool		mTransp;
            bitmap*	mBitmap;
};



class offscreen : public displayObj {

  public :
				offscreen(void);
				~offscreen(void);

				bool  dispObjBegin(void);
				void  beginDraw(bitmap* inMap,int inOffsetX=0,int inOffsetY=0);
				void  endDraw(void);

	virtual   void  setRotation(byte inRotation);
	virtual   void  setTextColor(colorObj* tColor);
	virtual   void  setTextColor(colorObj* tColor,colorObj* bColor);
	virtual   void  setTextSize(byte inSize);
	virtual   void  setTextWrap(boolean wrap);
	virtual   void  setFont(const GFXfont* font);
	virtual   void  setCursor(int x,int y);
	virtual   void  drawText(char* inText);
	virtual   void  drawChar(int x,int y,char inChar,colorObj* fColor,colorObj* bColor,int size);
	virtual   void  fillScreen(colorObj* inColor);
	virtual   void  fillRect(int x,int y,int width,int height,colorObj* inColor);
	virtual   void  fillRect(rect* aRect,colorObj* inColor);
	virtual   void  drawRect(int x,int y,int width,int height,colorObj* inColor);
	virtual   void  drawRect(rect* aRect,colorObj* inColor);
	virtual   void  fillRoundRect(int x,int y,int width,int height,int radius,colorObj* inColor);
	virtual   void  fillRoundRect(rect* aRect,int radius,colorObj* inColor);
	virtual   void  drawRoundRect(int x,int y,int width,int height,int radius,colorObj* inColor);
	virtual   void  drawRoundRect(rect* aRect,int radius,colorObj* inColor);
	virtual   void  drawCircle(int x,int y,int diam, colorObj* inColor);
	virtual   void  fillCircle(int x,int y,int diam, colorObj* inColor);
	virtual   void  drawVLine(int x,int y,int height,colorObj* inColor);
	virtual   void  drawHLine(int x,int y,int width,colorObj* inColor);
	virtual   void  drawLine(int x,int y,int x2,int y2,colorObj* inColor);
	virtual   void  drawPixel(int x,int y,colorObj* inColor);

		
		bool        mDrawing;
		mapDisplay* mDisplay;
		displayObj* oldScreen;
};


#endif
