#include <sliderPuzzle.h>	
#include <SDTools.h>
#include <filePath.h>

// **************  nextImage

	
nextImage::nextImage(int inX,int inY,const char* path)
	: iconButton(inX,inY,path) { ourGame = NULL; }
	
	
nextImage::~nextImage(void) {  }

			
void nextImage::setGame(sliderPuzzle* inPtr) { ourGame = inPtr; }


void nextImage::doAction(void) {

	if (ourGame) {
		OSPtr->beep();
		ourGame->doNextImage();
	}
}



// **************  scramble


scramble::scramble(int inX,int inY,const char* path)
	: iconButton(inX,inY,path) { ourGame = NULL; }
	
	
scramble::~scramble(void) {  }

				
void scramble::setGame(sliderPuzzle* inPtr) { ourGame = inPtr; }


void scramble::doAction(void) {

	if (ourGame) {
		OSPtr->beep();
		ourGame->doScramble();
	}
}



// **************  sliderPuzzle


sliderPuzzle::sliderPuzzle(int ourAppID)
  : panel(ourAppID) { ourFileIndex = NULL; }


sliderPuzzle::~sliderPuzzle(void) { if (ourFileIndex) delete(ourFileIndex); }


void sliderPuzzle::setup(void) {
  
	nextImage*	nextBtn;
	scramble*	mixBtn;
	
	ourBoard = new sliderBoard(0,24);
	if (ourBoard) {
		ourBoard->setup();
		addObj(ourBoard);
		if (setFilePath("next.bmp")) {
			nextBtn = new nextImage(68,283,mFilePath);
			nextBtn->setGame(this);
			addObj(nextBtn);
		}
		if (setFilePath("reload.bmp")) {
			mixBtn = new scramble(140,283,mFilePath);
			mixBtn->setGame(this);
			addObj(mixBtn);
		}
		buildPixIndex();
	}
}


void sliderPuzzle::buildPixIndex(void) {
	/* IN PROCESS!!
	filePath		imageFolder;
	pathItem*	trace;
	int			numFiles;
	
	if (setFilePath("pix/")) {
		imageFolder.setPath(mFilePath);
		trace = imageFolder.childList;
		numFiles = 0;;
		while(trace) {
			if (isPixFile(trace)) {
				numFiles++;
			}
			trace  = (pathItem*)trace->dllNext;
		}
		if (numFiles) {
			ourFileIndex = new cardIndex(numFiles);
		}
	}
	*/
}


void sliderPuzzle::doNextImage(void) { 

	int		pixNum;
	filePath	pixFolder;
	int		fileNum;
	/* IN PROCESS!!
	if (ourFileIndex) {
		if (!ourFileIndex->getNumRemain()) {
			ourFileIndex->loadList();
		}
		pixNum = ourFileIndex->dealCard();
		if (pixNum) {
			if (setFilePath("pix/")) {
				pixFolder.setPath(mFilePath);
				trace = pixFolder.childList;
				fileNum = 0;;
				while(trace) {
					if (isPixFile(trace)) {
						fileNum++;
						if (fileNum==pixNum) {
							pixFolder.pushItem(trace);
							
							return;
						}
					}
					trace  = (pathItem*)trace->dllNext;
				}
			}
		}
	}
	*/ 
}


void sliderPuzzle::drawSelf(void) {

	colorObj	barColor;
	
	screen->fillScreen(&black);
	barColor.setColor(&blue);
	screen->drawHLine(0,241+24,240,&barColor);
	barColor.blend(&white,50);
	screen->drawHLine(0,241+25,240,&barColor);
	barColor.setColor(&blue);
	barColor.blend(&black,50);
	screen->drawHLine(0,241+26,240,&barColor);
}


void sliderPuzzle::doScramble(void) {

	
	ourBoard->scramble();
}


void sliderPuzzle::loop(void) { }


void sliderPuzzle::closing(void) {  }


