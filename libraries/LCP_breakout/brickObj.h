#ifndef brickObj_h
#define brickObj_h

#include "backColorObj.h"
#include "breakout.h"
#include <bmpPipe.h>	// THIS BIT MUST GO!!


extern int  brickIndex;

class brickObj :	public drawObj,
						public backColorObj,
						public bmpPipe {

public:
			brickObj(int inX,int inY);
virtual	~brickObj(void);

			void  setColor(char* path);
			void  hit(void);
			void  reset(void);
virtual	void  drawSelf(void);
    
			int       homeX;
			int       homeY;
};

extern brickObj* brickList[];
extern void resetBricks(void);
extern boolean bricks(void);

#endif
