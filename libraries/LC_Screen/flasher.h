#ifndef flasher_h
#define flasher_h


// flasher.h contains base classes for flashing things on the screen. Basically a two
// frame sprit that repeats. flasher, flashes a base class drawOnj and bmpFlasher holds
// and swaps two .bmp files.


#include <drawObj.h>
#include <squareWave.h>
#include <bmpPipe.h>


// *****************************************************
//                        flasher
// *****************************************************

// flasher is a drawObj that has two states that automatically flip back and forth.
// The default is a two color rectangle changing colors every 1/2 second.

class flasher :	public drawObj,
						public squareWave {

	public:
				flasher(rect* inRect,colorObj* backColor=&black);
				flasher(int inLocX,int inLocY,int inWidth,int inHeight,colorObj* backColor=&black);
					
	virtual	~flasher(void);
	virtual	void	drawSelf(void);
	virtual	void	pulseOn(void);                                                   
	virtual	void	pulseOff(void);
	
	protected:
				colorObj		mForeColor;
				colorObj		mBackColor;
				bool			mFlash;
};



// *****************************************************
//                     bmpFlasher
// *****************************************************

// bmpFlasher swaps between two bitmaps instead of a colored rectangle.

class bmpFlasher :  public flasher {
    
	public:
				bmpFlasher(int inX,int inY, int width,int height,char* onBmp, char* offBmp);
				bmpFlasher(rect* inRect,char* onBmp, char* offBmp);
	virtual	~bmpFlasher(void);

				void	setup(char* onBmp, char* offBmp);
	virtual	void	drawSelf(void);

				bool		mReady;
            bmpPipe*	mOnBmp;
            bmpPipe*	mOffBmp;     
};

#endif
