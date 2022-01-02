#ifndef iconEditScr_h
#define iconEditScr_h

#include <lilOS.h>
#include <bmpObj.h>
#include <mapper.h>

#define SCALE 		6
#define EDITSCR_X	23
#define EDITSCR_Y	35
#define EDITSCR_W	32 * SCALE
#define EDITSCR_H	32 * SCALE

class iconEditScr : public bmpObj {

	public:
				iconEditScr(int inX,int inY,int inWidth,int inHeight,char* filePath);
	virtual	~iconEditScr(void);
	
	virtual	void		drawSelf(void);
	virtual	void  	doAction(event* inEvent,point* locaPt);

				mapper	xMap;
				mapper	yMap;
				colorObj	editColor;
				int		scale;
};

#endif