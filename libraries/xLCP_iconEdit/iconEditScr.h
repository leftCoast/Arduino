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
				iconEditScr(int inX,int inY,int inWidth,int inHeight,const char* filePath);
	virtual	~iconEditScr(void);
	
				int		getBrushSize(void);
	virtual	void		setBrushSize(int inSize);
	virtual	void		doPixels(int x,int y,int dia,colorObj* color);
	virtual	void  	doAction(event* inEvent,point* locaPt);
	virtual	void		drawSelf(void);

				mapper	xMap;
				mapper	yMap;
				colorObj	editColor;
				int		scale;
				int		brushSize;
				float 	savedTime;
				bool		dragging;
};

#endif