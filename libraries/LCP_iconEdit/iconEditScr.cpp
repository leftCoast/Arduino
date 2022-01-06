#include <iconEditScr.h>

//#include <debug.h>


iconEditScr::iconEditScr(int inX,int inY,int inWidth,int inHeight,char* filePath) 
	: bmpObj(inX,inY,inWidth,inHeight,filePath) {
	
	
	setEventSet(touchNDrag);
	editColor.setColor(&red);
	scale = SCALE;
	xMap.setValues(inX+1,inWidth+inX-1,0,32);
	yMap.setValues(inY+1,inHeight+inY-1,0,32);
	openDocFile(FILE_WRITE);
	brushSize = 1;
}


iconEditScr::~iconEditScr(void) {  }


int iconEditScr::getBrushSize(void) { return brushSize; }


void iconEditScr::setBrushSize(int inSize) { brushSize = inSize; }


void iconEditScr::doPixels(int x,int y,int dia,colorObj* color) {

	int rad;
	int trace;
	int trace2;
	
	rad = dia/2;
	if (rad) {
		for(int i=-rad;i<rad;i++) {
			trace = x+i;
			trace2 = y+i;
			setPixel(trace,y,color);
			setPixel(x,trace2,color);
			setPixel(trace,trace2,color);
		}
	} else {
		setPixel(x,y,color);
	}
}

	
void iconEditScr::doAction(event* inEvent,point* locaPt) {

	int xPix;
	int yPix;
	
 	xPix = xMap.map(locaPt->x);
 	yPix = yMap.map(locaPt->y);
	switch(inEvent->mType) {								// We have an incoming event. Check the type.
		case touchEvent  : ourOSPtr->beep();			// THE FINGER!  
		case dragBegin  :										// A starting off a drag action.  
		case dragOn     :										// Drag on event.
			doPixels(xPix,yPix,brushSize,&editColor);	// Do the painting.
		break;													// Done here.
		default : break;										// Shut up compiler!
	}
	setNeedRefresh();
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