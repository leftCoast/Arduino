#ifndef bmpLabel_h
#define bmpLabel_h

#include <bmpPipe.h>
#include <label.h>

// This was written for the old vacuum gauge. It had a graphic "skin" and
// The text was colored to look like it was glowing under the glass. This
// made it so I could update the text without seeing a nasty flicker that
// would just destroy the effect.
//
// Oddly, it worked.

class bmpLabel : public label {

	public:
  				bmpLabel(int inX,int inY,int width,int height,char* inText,bmpPipe* inBackdrop);
	virtual	~bmpLabel(void);
  
  				void		setFastDraw(bool fast);
  				void		smoothDraw(void);
  				void		fasterDraw(void);
	virtual	void		drawSelf(void);

				bmpPipe*	backdrop;
				bool		fastDraw;
};

#endif

