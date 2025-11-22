#include <sliderPuzzleBits.h>
#include <offscreen.h>


#define SAVE_FILE_NAME	"SAVEFILE"
#define SAVE_BUFF_BYTES	40				// Has to be biffer that #squares * 2.

// ************** sliderRect **************


sliderRect::sliderRect(sliderBoard* inBoard,int inID)
  :iconButton(0,0,inBoard->ourImagePath.getPath(),SQR_SIZE) {

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
	for(int i=1;i<ourBoard->ourBmp->getWidth()-1;i++) {
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
  
	saveFileName	= NULL;
	numSquares		= 0;
	ourBmp			= NULL;
	ourBmp			= new bitmap(SQR_SIZE,SQR_SIZE);
}


sliderBoard::~sliderBoard(void) { 

	if (ourBmp) delete(ourBmp);
	freeStr(&saveFileName);
}


void sliderBoard::setup(filePath* imagePath) {
	
	sliderRect* aSlider;
	int			numSquares;
	int			i;

	ourImagePath.setPath(imagePath->getPath());
	//if (savePath) {
	//	ourSavePath.setPath(savePath->getPath());
	//}
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
	scramble();
}


void sliderBoard::blindSwap(sliderRect* rectA,sliderRect* rectB) {

	int xLoc;
	int yLoc;
	
	xLoc = rectA->x;
	yLoc = rectA->y;
	rectA->setRect(rectB);
	rectB->x = xLoc;
	rectB->y = yLoc;
	rectA->setNeedRefresh();
	rectB->setNeedRefresh();
}

	
void sliderBoard::scramble() {

	int			IDA;
	int			IDB;
	sliderRect* rectA;
	sliderRect* rectB;
	cardIndex	deck(numObjects());
	
	while(deck.cardsLeft()) {
		IDA = deck.dealCard();
		IDB = deck.dealCard();
		IDA--;
		IDB--;
		rectA = (sliderRect*)getObj(IDA);
		rectB = (sliderRect*)getObj(IDB);
		if (rectA && rectB) {
			blindSwap(rectA,rectB);
		}
	}
}


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
		


// Reads a line of text into a buffer with number of bytes. From an o=already open file. 
// Stops reading if it sees a null char or a newline char.
void sliderBoard::readLine(File src,char* buff,int numBytes) {

	int				i;
	int				maxI;
	char				aChar;
			
	maxI = numBytes-1;			// Leave some room for '\0'.
	i = 0;							// Clear i.
	buff[i] = '\0';				// Drop in a '\0' just in case it ain't used.
	aChar = src.read();			// Grab the first char.
	while(aChar!='\0'				// If it ain't NULL char..
		&& aChar!='\n' 			// If it ain't newline..
		&& (uint8_t)aChar!=255	// If it ain't end of file..	
		&& i<maxI) {				// If we ain't run out of buffer. Then..
		buff[i] = aChar;			// Stuff in the char.
		i++;							// Bump up for the next read.
		buff[i] = '\0';			// Drop a NULL char here, In case we're done.
		aChar = src.read();		// read the next char.
	}
}
		

// Reads all the data and returns if it's successful or not. Get it all and does it still
// match?
bool sliderBoard::readState(void) {

	File	saveFile;
	char	buff[SAVE_BUFF_BYTES];
	int	i;
	

	saveFile = SD.open(ourSavePath.getPath(), FILE_READ);
	if (saveFile) {
		readLine(saveFile,buff,SAVE_BUFF_BYTES);
		heapStr(&saveFileName,buff);
		readLine(saveFile,buff,SAVE_BUFF_BYTES);
		numSquares = atoi(buff);
		if (numSquares==(NUM_SQR*NUM_SQR)) {
			readLine(saveFile,(char*)locList,SAVE_BUFF_BYTES);
			saveFile.close();
			for(i=0;i<numSquares*2;i++) {
				locList[i] = (byte)(locList[i])-1;
			}
			return true;
		}
	}
	return false;					
}


void sliderBoard::writeState(void) {
	
	File	saveFile;
	int	i;
	
	saveFile = SD.open(ourSavePath.getPath(),FILE_WRITE);
	if (saveFile) {
		saveFile.seek(0);
		saveFile.println(saveFileName);
		saveFile.println(numSquares);
		for(i=0;i<numSquares*2;i++) {
			saveFile.write((byte)(locList[i]+1));
		}
		saveFile.write((byte)(0));
		saveFile.close();
	}
}

				
				
void sliderBoard::drawSelf(void) { screen->fillRect(this,&black); }


