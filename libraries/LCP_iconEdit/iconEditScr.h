#ifndef iconEditScr_h
#define iconEditScr_h

#include <lilOS.h>
#include <bmpObj.h>

#define SCALE 6

class iconEditScr : public bmpObj {

	public:
				iconEditScr(lilOS* OSPtr,int inX,int inY,int inWidth,int inHeight,char* filePath);
	virtual	~iconEditScr(void);
	
	virtual	void		drawSelf(void);
	virtual	void  	doAction(event* inEvent,point* locaPt);
				
				lilOS* 	ourOS;
				colorObj	editColor;
				int		scale;
};

#endif