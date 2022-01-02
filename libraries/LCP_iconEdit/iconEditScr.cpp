#include <iconEditScr.h>

#include <debug.h>

char* iconFolder = NULL;

iconEditScr::iconEditScr(int inX,int inY,int inWidth,int inHeight,char* filePath) 
	: bmpObj(inX,inY,inWidth,inHeight,filePath) {
	
	
	setEventSet(touchLift);
	editColor.setColor(&red);
	scale = SCALE;
	xMap.setValues(0,inWidth-1,0,31);
	yMap.setValues(0,inHeight-1,0,31);
	openDocFile(FILE_WRITE);
}


iconEditScr::~iconEditScr(void) {  }


void iconEditScr::doAction(event* inEvent,point* locaPt) {
ST
	
	
	int xPix;
	int yPix;
	
	ourOSPtr->beep();
	if (inEvent->mType==touchEvent) {
		xPix = xMap.map(locaPt->x);
		yPix = yMap.map(locaPt->y);
		//xPix = inEvent->mTouchPos.x/scale;
		//yPix = inEvent->mTouchPos.y/scale;
		setPixel(xPix,yPix,&red);
	}
}	


void iconEditScr::drawSelf(void) {
ST
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