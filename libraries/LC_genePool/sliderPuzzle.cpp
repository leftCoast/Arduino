#include <sliderPuzzle.h>
#include <SDTools.h>
#include <filePath.h>
#include <bmpImage.h>


// **************  nextImage

	
nextImage::nextImage(int inX,int inY,const char* path)
	: iconButton(inX,inY,path) { ourGame = NULL; }
	
	
nextImage::~nextImage(void) {  }

			
void nextImage::setGame(sliderPuzzle* inPtr) { ourGame = inPtr; }


void nextImage::doAction(void) {

	if (ourGame) {
		OSPtr->beep();
		ourGame->chooseImage();
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



// **************  pixIndex


// pixFileItem

pixFileItem::pixFileItem(const char* inName)
	:linkListObj() {
	
	fileName = NULL;
	heapStr(&fileName,inName);
}


pixFileItem::~pixFileItem(void) { freeStr(&fileName); }


char* pixFileItem::getName(void) { return fileName; }



// pixIndex

pixIndex::pixIndex(void) 
	:linkList() { }


pixIndex::~pixIndex(void) {  }


// This does a complete sanity checking
bool pixIndex::isPixFile(const char* fileName) {

	if (MacOSFilter(fileName)) {						// If it passes the Mac filter..
		if (extensionMatch(".BMP",fileName)) {		// IF it passes the extension test..
			return true;									// Not an MacOS thing, has .bmp.. Good.
		}														//
	}															//
	return false;											// If we got here? Fail.
}


void pixIndex::buildList(filePath* imagePath) {

	pathItem*		trace;
	pixFileItem*	newNode;
	
	dumpList();										   			// Make sure we got a fresh start.
	trace = imagePath->getChildList();						// Grab a pointer to the child list.
	while(trace) {													// While we have a child..
		if (isPixFile(trace->getName())) {					// Check if it's a good file.
			newNode = new pixFileItem(trace->getName());	// Create item..
			if (newNode) {
				addToTop(newNode);								// Save it to the list.
			}															//
		}																//
		trace  = (pathItem*)trace->dllNext;					// Trace hops to the next child.
	}																	//
}																			


char* pixIndex::getName(int cardIndex) {

	pixFileItem*	anItem;
	
	if (cardIndex>0) {												// Sanity, we got valid index?
		cardIndex--;													// card indexes start at 1, we start at 0;
		anItem = (pixFileItem*)getByIndex(cardIndex);		// Choose that file name.
		if (anItem) {													// Got it?
			return anItem->getName();								// Pass back the file name.
		}																	//
	}																		//
	return NULL;														// If we got here? Something went wrong.
}


// **************  sliderPuzzle


sliderPuzzle::sliderPuzzle(int ourAppID)
  : panel(ourAppID) {
  
	ourCardDeck		= NULL;  
  }


sliderPuzzle::~sliderPuzzle(void) { if (ourCardDeck) delete(ourCardDeck); }





void sliderPuzzle::setup(void) {
  
	nextImage*	nextBtn;
	scramble*	mixBtn;
	int			fileCount;
	bool			success;
	
	success = false;																	// Well, it aint' one yet..
	if (setImagePath()) {															// If we setup our image path..
		ourPixIndex.buildList(&ourImagePath);									// Build the pixIndex list.
		fileCount = ourPixIndex.getCount();										// How many we got?
		if (fileCount) {																// If we got any image to show..
			ourCardDeck = new cardIndex(fileCount);							// Create our card index.
			if (ourCardDeck) {														// If we managed that..
				ourBoard = new sliderBoard(0,24);								// Build the slider board.
				if (ourBoard) {														// If we were able to build it..
					addObj(ourBoard);													// Hand off board to our screen list.
					if (chooseImage()) {												// If we can choose an image.
						if (setFilePath("next.bmp")) {							// Locate our next button file.
							nextBtn = new nextImage(68,283,mFilePath);		// Have a go at creating this button.
							if (nextBtn) {												// If we got a button.
								nextBtn->setGame(this);								// Tell the button we are their game.
								addObj(nextBtn);										// Add the button to our screen list.
								if (setFilePath("reload.bmp")) {					// Locate the reload button file.
									mixBtn = new scramble(140,283,mFilePath);	// Have a go at creating this button.
									if (mixBtn) {										// If we got a button.
										mixBtn->setGame(this);						// Tell it we're their game.
										addObj(mixBtn);								// Add it to our screen list.
										success = true;								// We got here? We're good!
									}														//
								}															//
							}																//
						}																	//
					}																		//
				}																			//
			}																				//												
		}																					//
	}																						//
	if (!success) close();															// If we failed? Call close.				
}



bool sliderPuzzle::setImagePath(void) {

	if (setFilePath("pix/")) {					// Build path string to our image folder.
		ourImagePath.setPath(mFilePath);		// Setup "path" with this path string.
		return true;								// Return, we're done.
	}													//
	ourImagePath.reset();						// Reset the image path, we failed.
	return false;
	
}


bool sliderPuzzle::chooseImage() {
	
	int	cardIndex;
	char*	name;
	
	cardIndex = ourCardDeck->dealCard();				// Grab a card index
	if (!cardIndex) {											// Didn't get one?
		ourCardDeck->shuffle();								// Reshuffle the cards.
		cardIndex = ourCardDeck->dealCard();			// Try again.
	}																//
	name = ourPixIndex.getName(cardIndex);				// Grab the name that matches.
	if (name) {													// Got one?
		ourImagePath.pushChildItemByName(name);		//	Push it on the path.
		ourBoard->setup(&ourImagePath);					// Given this image, reset the board.
		ourImagePath.popItem();								// Reset the path.
		return true;											// Now that's a seccess!
	}																//
	return false;												// Didn't make it.	
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


void sliderPuzzle::doScramble(void) { ourBoard->scramble(); }


void sliderPuzzle::loop(void) { }


void sliderPuzzle::closing(void) {  }


