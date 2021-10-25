#include <iconEditScr.h>

char* iconFolder = NULL;

iconEditScr::iconEditScr(lilOS* OSPtr,int inX,int inY,int inWidth,int inHeight,char* filePath) 
	: bmpObj(inX,inY,inWidth,inHeight,filePath) {
	
	ourOS = OSPtr;
	setEventSet(touchLift);
	editColor.setColor(&red);
	scale = SCALE;
	openDocFile(FILE_WRITE);
}


iconEditScr::~iconEditScr(void) {  }


void iconEditScr::doAction(event* inEvent,point* locaPt) {

	int xPix;
	int yPix;
	
	ourOS->beep();
	if (inEvent->mType==touchEvent) {
		xPix = inEvent->mTouchPos.x/scale;
		yPix = inEvent->mTouchPos.y/scale;
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


bool iconEditScr::askOk(char* qStr) {return true; }