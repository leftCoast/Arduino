
#ifndef DFRobot_0669_Obj_h
#define DFRobot_0669_Obj_h

#include <displayObj.h>
#include <ILI9488_t3.h>

#include <LC_SPI.h>


#define TFT_WIDTH		340	//< The width of the display
#define TFT_HEIGHT	480	//< The height of the display


extern float	percBlack;


class DFRobot_0669_Obj : public displayObj {

	public :
  				DFRobot_0669_Obj(byte inCS,byte inRST);
	virtual	~DFRobot_0669_Obj(void);
  
  	virtual 	bool	begin(void);
	virtual	int	width(void);
	virtual	int	height(void);
	virtual 	void	setRotation(byte inRotation);
	virtual 	void	setTextColor(colorObj* tColor);
	virtual 	void	setTextColor(colorObj* tColor,colorObj* bColor);
	virtual 	void	setTextSize(byte inSize);
	virtual 	void	setTextWrap(boolean wrap);
	virtual	rect	getTextRect(const char* inText);
	virtual 	void	setFont(const GFXfont* font);
	virtual 	void	setCursor(int x,int y);
	virtual	int	getCursorX(void);
	virtual	int	getCursorY(void);
	virtual 	void	drawText(const char* inText);
	virtual 	void	fillScreen(colorObj* inColor);
	virtual 	void	fillRect(int x,int y,int width,int height,colorObj* inColor);
	virtual 	void	drawRect(int x,int y,int width,int height,colorObj* inColor);
	virtual 	void	fillRect(rect* inRect,colorObj* inColor);
	virtual 	void	drawRect(rect* inRect,colorObj* inColor);
	virtual 	void	fillRoundRect(int x,int y,int width,int height,int radius,colorObj* inColor);
	virtual 	void	drawRoundRect(int x,int y,int width,int height,int radius,colorObj* inColor);
	virtual 	void	drawCircle(int x,int y,int diam, colorObj* inColor);
	virtual 	void	fillCircle(int x,int y,int diam, colorObj* inColor);
	virtual	void	drawTriangle(point* pt0,point* pt1,point* pt2,colorObj* inColor);
	virtual	void	fillTriangle(point* pt0,point* pt1,point* pt2,colorObj* inColor);
	virtual 	void	drawVLine(int x,int y,int height,colorObj* inColor);
	virtual 	void	drawHLine(int x,int y,int width,colorObj* inColor);
	virtual 	void	drawLine(int x,int y,int x2,int y2,colorObj* inColor);
	virtual 	void	drawPixel(int x,int y,colorObj* inColor);
	
	
  private:
          ILI9488_t3*		theTFT;
          byte             cs;
          byte             rst;
};


#endif