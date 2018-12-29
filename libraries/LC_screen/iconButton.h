#ifndef iconButton_h
#define iconButton_h

#include <drawObj.h>
#include <bmpPipe.h>

// Make a button with a 32x32 pixel icon stored in a .bmp file.
// Seeing its expecting a 32x32 pixel file we can simplify a bunch.


class iconButton :	public drawObj,
							public bmpPipe {

	public:
				iconButton(int xLoc,int yLoc,char* path);
	virtual	~iconButton(void);
	
				void	begin(void);
	virtual	void	drawSelf(void);
	virtual	void	doAction(void);
	
				char*	mPath;
};

#endif