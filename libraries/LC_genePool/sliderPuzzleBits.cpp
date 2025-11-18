#include <sliderPuzzleBits.h>
#include <offscreen.h>


#define NUM_SQR   4 
#define SQR_SIZE 60
#define BRD_SIZE NUM_SQR * SQR_SIZE


#define IMAGE_PATH "/system/images/lake.bmp"
class sliderBoard;


// ************** sliderRect **************


sliderRect::sliderRect(sliderBoard* inBoard,int inID)
  :iconButton(0,0,IMAGE_PATH,SQR_SIZE) {

    ourBoard  = inBoard;
    ourID     = inID;
    visable   = true;
    setEventSet(fullClick);
  }



sliderRect::~sliderRect(void) {  }


void sliderRect::setVisable(bool yesNo) { visable = yesNo; }


bool sliderRect::isClear(void) { return !visable; }


void sliderRect::doAction(void) { ourBoard->clicked(this); }

				
void sliderRect::swapWith(sliderRect* inRect) {

  int xLoc;
  int yLoc;

  if (visable && inRect->isClear()) {
    xLoc = x;
    yLoc = y;
    x = inRect->x;
    y = inRect->y;
    inRect->x = xLoc;
    inRect->y = yLoc;
    setNeedRefresh();
    inRect->setNeedRefresh();
  }
}


void sliderRect::drawOffscreen(void) {

	offscreen	disp;
	colorObj		aPixel;
	
	disp.beginDraw(ourBoard->ourBmp,x,y);
	iconButton::drawSelf();
	disp.endDraw();
	for(int i=1;i<ourBoard->ourBmp->getHeight()-1;i++) {
		aPixel = ourBoard->ourBmp->getColor(0,i);
		aPixel.blend(&white,30);
		ourBoard->ourBmp->setColor(0,i,&aPixel);
		
		aPixel = ourBoard->ourBmp->getColor(x+drawObj::width-1,i);
		aPixel.blend(&black,30);
		ourBoard->ourBmp->setColor(x+drawObj::width-1,i,&aPixel);
		
	}
	for(int i=1;i<ourBoard->ourBmp->getHeight()-1;i++) {
		aPixel = ourBoard->ourBmp->getColor(i,0);
		aPixel.blend(&white,30);
		ourBoard->ourBmp->setColor(i,0,&aPixel);
		
		aPixel = ourBoard->ourBmp->getColor(i,0);
		aPixel.blend(&white,30);
		ourBoard->ourBmp->setColor(i,0,&aPixel);
		
		aPixel = ourBoard->ourBmp->getColor(i,y+drawObj::height-1);
		aPixel.blend(&black,30);
		ourBoard->ourBmp->setColor(i,y+drawObj::height-1,&aPixel);
	}
	screen->blit(x,y,ourBoard->ourBmp);
}


void sliderRect::drawSelf(void) {

	
	if (visable) {
		if (ourBoard->ourBmp) {
			drawOffscreen();
		} else {
			iconButton::drawSelf();
			screen->drawRect(this,&white);
		}
	} else {
		screen->fillRect(this,&black);
	}
}


// ************* sliderBoard **************



sliderBoard::sliderBoard(int x, int y)
  : drawGroup(x,y,BRD_SIZE,BRD_SIZE) {
  
  ourBmp = new bitmap(SQR_SIZE,SQR_SIZE);
}


sliderBoard::~sliderBoard(void) { if (ourBmp) delete(ourBmp); }


void sliderBoard::setup(void) {
	
	sliderRect* aSlider;
	int			numSquares;
	int			i;
	//cardIndex	deck(NUM_SQR * NUM_SQR);
	
	dumpDrawObjList();
	numSquares = NUM_SQR * NUM_SQR;
	i = 1;
	for(int col=0;col<NUM_SQR;col++) {
		for(int row=0;row<NUM_SQR;row++) {
			aSlider = new sliderRect(this,i);
			if (aSlider) {
				aSlider->setLocation(col * SQR_SIZE,row * SQR_SIZE);
				aSlider->setSourceOffset(aSlider->x,aSlider->y);
				if (i==numSquares) {
					aSlider->setVisable(false);
				}
				i++;
				addObj(aSlider);
			}
		}
	}
	
}
	
	
void sliderBoard::setImage(const char* path) {   }


void sliderBoard::scramble() {  }


sliderRect* sliderBoard::findSlider(rect* aRect) {

	rect*	trace;
	int	numRects;
	
	numRects = numObjects();
	for (int i=0;i<numRects;i++) {	
		trace = (rect*)getObj(i);
		if (trace->isSameAs(aRect)) {
			return (sliderRect*)trace;
		}
	}
	return NULL; 
}


void sliderBoard::clicked(sliderRect* theClicked) {
  
	rect			testRect;
	sliderRect*	foudRect;
	
	
	testRect.setRect(theClicked);			
	testRect.y = testRect.y - SQR_SIZE;		// Look up.
	foudRect = findSlider(&testRect);
	if (foudRect) {
		if (foudRect->isClear()) {
			theClicked->swapWith(foudRect);
			return;
		}
	}
	testRect.setRect(theClicked);
	testRect.x = testRect.x + SQR_SIZE;		// look right.
	foudRect = findSlider(&testRect);
	if (foudRect) {
		if (foudRect->isClear()) {
			theClicked->swapWith(foudRect);
			return;
		}
	}
	testRect.setRect(theClicked);
	testRect.y = testRect.y + SQR_SIZE;		// look down.
	foudRect = findSlider(&testRect);
	if (foudRect) {
		if (foudRect->isClear()) {
			theClicked->swapWith(foudRect);
			return;
		}
	}
	testRect.setRect(theClicked);
	testRect.x = testRect.x - SQR_SIZE;		// look left.
	foudRect = findSlider(&testRect);
	if (foudRect) {
		if (foudRect->isClear()) {
			theClicked->swapWith(foudRect);
			return;
		}
	}
}


void sliderBoard::drawSelf(void) { screen->fillRect(this,&black); }


