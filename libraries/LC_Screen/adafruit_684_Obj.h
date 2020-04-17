#include "displayObj.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <LC_SPI.h>


// Rotation
#define PORTRAIT      0  // Wires up
#define LANDSCAPE     1  // Wires left
#define INV_PORTRAIT  2  // Wires down
#define INV_LANDSCAPE 3  // Wires right

class adafruit_684_Obj : public displayObj {

  public :
  				adafruit_684_Obj(byte inCS,byte inRST=-1);
  virtual	~adafruit_684_Obj(void);
  
	virtual	int	width(void);
	virtual	int	height(void);
	virtual 	bool	dispObjBegin(void);
	virtual 	void	setRotation(byte inRotation);
	virtual 	void	setTextColor(colorObj* tColor);
	virtual 	void	setTextColor(colorObj* tColor,colorObj* bColor);
	virtual 	void	setTextSize(byte inSize);
	virtual 	void	setTextWrap(boolean wrap);
	virtual 	void	setFont(const GFXfont* font);
	virtual 	void	setCursor(int x,int y);
	virtual 	void	drawText(char* inText);
	virtual 	void	fillScreen(colorObj* inColor);
	virtual 	void	fillRect(int x,int y,int width,int height,colorObj* inColor);
	virtual 	void	drawRect(int x,int y,int width,int height,colorObj* inColor);
	virtual 	void	fillRect(rect* inRect,colorObj* inColor);
	virtual 	void	drawRect(rect* inRect,colorObj* inColor);
	virtual 	void	fillRoundRect(int x,int y,int width,int height,int radius,colorObj* inColor);
	virtual 	void	drawRoundRect(int x,int y,int width,int height,int radius,colorObj* inColor);
	virtual 	void	drawCircle(int x,int y,int diam, colorObj* inColor);
	virtual 	void	fillCircle(int x,int y,int diam, colorObj* inColor);
	virtual 	void	drawVLine(int x,int y,int height,colorObj* inColor);
	virtual 	void	drawHLine(int x,int y,int width,colorObj* inColor);
	virtual 	void	drawLine(int x,int y,int x2,int y2,colorObj* inColor);
	virtual 	void	drawPixel(int x,int y,colorObj* pColor);
	 //virtual	void	blit(int locX,int locY,bitmap* inBitmap);

  private:
          Adafruit_SSD1331* theOLED;
          byte              cs;
          byte              rst;
};
