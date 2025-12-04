#include <iconEditScr.h>

//#include <debug.h>


iconEditScr::iconEditScr(int inX,int inY,int inWidth,int inHeight,const char* filePath) 
	: bmpObj(inX,inY,inWidth,inHeight,filePath) {
	
	setEventSet(touchNDrag);
	editColor.setColor(&red);
	scale = SCALE;
	xMap.setValues(inX+1,inWidth+inX-1,0,32);
	yMap.setValues(inY+1,inHeight+inY-1,0,32);
	openDocFile(FILE_WRITE);
	brushSize = 1;
	dragging = false;
}


iconEditScr::~iconEditScr(void) {  }


int iconEditScr::getBrushSize(void) { return brushSize; }


void iconEditScr::setBrushSize(int inSize) { brushSize = inSize; }


void iconEditScr::doPixels(int x,int y,int dia,colorObj* color) {

	
	mapper		distMapper;
	point			center;
	point			current;
	int			iconX;
	int			iconY;
	float			dist;
	float			rad;
	float			percent;
	RGBpack		packedBackColor;
	colorObj		backColor;
		
	if (x>=0 && y>=0 && dia>2 && color) {			// Just one big pack of sanity checking..
		rad = dia/2.0;
		distMapper.setValues(0,rad,100,0);
		center.x = rad;
		center.y = rad;
		for(int i=0;i<dia;i++) {
			iconX = round(x+i-rad);
			for (int j=0;j<dia;j++) {
				iconY = round(y+j-rad);
				packedBackColor = getRawPixel(iconX,iconY);
				backColor.setColor(&packedBackColor);
				current.x = i;
				current.y = j;
				dist = distance(center,current);
				percent = distMapper.map(dist);
				backColor.blend(color,percent);
				setPixel(iconX,iconY,&backColor);
			}
		}
	} else if (x>0 && y>0 && dia<=2 && color) {
		setPixel(x,y,color);
	}
}

void iconEditScr::doAction(event* inEvent,point* locaPt) {

	int xPix;
	int yPix;
	
 	xPix = xMap.map(locaPt->x);
 	yPix = yMap.map(locaPt->y);
	switch(inEvent->mType) {									// We have an incoming event. Check the type.
		case touchEvent	:										// THE FINGER!
			doPixels(xPix,yPix,brushSize,&editColor);		// Do the painting.
		break;														// Enough! Lets go.
		case dragBegin		:										// Starting of a drag action.  
			dragging = true;
			savedTime = ourEventMgr.getTime();				// Grab the event manager repeat time..
			ourEventMgr.setTime(5,true);						// Make the time really really short.
			doPixels(xPix,yPix,brushSize,&editColor);		// Do the painting.
		break;								
		case dragOn			:										// Drag on event.
			doPixels(xPix,yPix,brushSize,&editColor);		// Do the painting.
		break;														// Done here.
		case liftEvent		:										// Got a lift event?
			if (dragging) {
				ourEventMgr.setTime(savedTime);				// Replace the old time.
				dragging = false;
			}
		break;
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