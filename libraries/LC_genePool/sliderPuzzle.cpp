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
  : panel(ourAppID) { }


sliderPuzzle::~sliderPuzzle(void) {  }


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
	}
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


void sliderPuzzle::doNextImage(void) { 

	filePath	imageFolder;
	char*		path;
	int		numFiles;
	
	path = NULL;
	if (setFilePath("pix/")) {
		imageFolder.setPath(mFilePath);
		numFiles = imageFolder.numChildItems();
		Serial.println(numFiles);
	
	ourBoard->setImage(path);
	} 
}


void sliderPuzzle::doScramble(void) {

	
	ourBoard->scramble();
}


void sliderPuzzle::loop(void) { }


void sliderPuzzle::closing(void) {  }


