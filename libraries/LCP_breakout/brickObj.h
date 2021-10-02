#ifndef brickObj_h
#define brickObj_h

#include "backColorObj.h"
#include "breakout.h"


extern int  brickIndex;

class brickObj :	public drawObj,
						public backColorObj {

public:
			brickObj(int inX,int inY);
virtual	~brickObj(void);

			void  setColor(bitmap* inBitmap);
			void  hit(void);
			void  reset(void);
virtual	void	drawSelf(void);
    		
			bitmap*	ourBMap;
			int		homeX;
			int		homeY;
};

extern brickObj* brickList[];
extern void resetBricks(void);
extern boolean bricks(void);

#endif
