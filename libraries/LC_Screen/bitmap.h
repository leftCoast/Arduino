#ifndef bitmap_h
#define bitmap_h

#include "colorObj.h"

class bitmap {

	public :
				bitmap(void);
				bitmap(int width,int height);
				bitmap(bitmap* aBitmap);
	virtual	~bitmap(void);

				bool			setSize(int width,int height);
				void			clearMap(void);
				bool			getHasMap(void);
				int			getWidth(void);
				int			getHeight(void);
				void			setColor(int x,int y,colorObj* aColor);
				void			setColor(int x,int y,RGBpack* aColor);
				colorObj		getColor(int x,int y);
				RGBpack		getColorPack(int x,int y);
				RGBpack*		getBitmapAddr(void);
				
  protected:
      int			mWidth;
      int			mHeight;
      bool			mHaveMap;
      RGBpack*		mBitmap;
};


#endif
