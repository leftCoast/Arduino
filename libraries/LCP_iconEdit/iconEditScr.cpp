#include <iconEditScr.h>

//#include <debug.h>

char* iconFolder = NULL;

iconEditScr::iconEditScr(int inX,int inY,int inWidth,int inHeight,char* filePath) 
	: bmpObj(inX,inY,inWidth,inHeight,filePath) {
	
	
	setEventSet(touchLift);
	editColor.setColor(&red);
	scale = SCALE;
	xMap.setValues(inX+1,inWidth+inX-1,0,32);
	yMap.setValues(inY+1,inHeight+inY-1,0,32);
	openDocFile(FILE_WRITE);
}


iconEditScr::~iconEditScr(void) {  }


void iconEditScr::doAction(event* inEvent,point* locaPt) {

	int xPix;
	int yPix;
	
	ourOSPtr->beep();
	if (inEvent->mType==touchEvent) {
		xPix = xMap.map(locaPt->x);
		yPix = yMap.map(locaPt->y);
		setPixel(xPix,yPix,&red);
	}
}	


void iconEditScr::drawSelf(void) {

	colorObj	aColor;
	
	if (mode!=fClosed) {
		for(int ty=0;ty<drawObj::height/scale;ty++) {
			for(int tx=0;tx<drawObj::width/scale;tx++) {
				aColor = getPixel(tx+offsetX,ty+offsetY);
				screen->fillRect(scale*tx+x,scale*ty+y,scale,scale,&aColor);
				screen->drawRect(scale*tx+x,scale*ty+y,scale,scale,&black);	
			}
		}
	} else {
		drawObj::drawSelf();
	}
}