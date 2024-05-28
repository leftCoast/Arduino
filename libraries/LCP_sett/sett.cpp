#include <sett.h>
#include <strTools.h>

#define CARD_IMAGE	"card.bmp"
#define CARD_W			60
#define CARD_H			80
#define SYMBOL_W		40
#define SYMBOL_H		16
#define GRID_X			25
#define GRID_Y			20
#define GRID_SPACE_X	5
#define GRID_SPACE_Y	5

#define BACK_COLOR	&black

#define UNCONNECTED_ANALOG_PIN  A1


sett::sett(lilOS* ourOS,int ourAppID)
	: panel(ourAppID) { }


sett::~sett(void) {  }


void sett::setup(void) {
	
	long int	seed;
	
	seed = analogRead(UNCONNECTED_ANALOG_PIN);	// Tired of playing the same old game every time?
	randomSeed(seed);	
	cardList = new cardIndex(81);
	if (cardList) {
		dealCards(GRID_X,GRID_Y);
		ourPlayer = new toneObj(ourOSPtr->getTonePin());
		createSounds();
		ourState = playing;						// Got all the bits? Then we wait.
	} else {
		ourState = error;
	}
}


void sett::createSounds(void) {
	
	for (int i=0;i<4;i++) {
		winTune.addNote(NOTE_F4,E_NOTE);
		winTune.addNote(NOTE_A5,E_NOTE);
		winTune.addNote(NOTE_C5,E_NOTE);
	}
	for (int i=0;i<4;i++) {
		winTune.addNote(NOTE_G4,E_NOTE);
		winTune.addNote(NOTE_B5,E_NOTE);
		winTune.addNote(NOTE_D5,E_NOTE);
	}
	for (int i=0;i<4;i++) {
		winTune.addNote(NOTE_C3,E_NOTE);
		winTune.addNote(NOTE_E4,E_NOTE);
		winTune.addNote(NOTE_G4,E_NOTE);
		winTune.addNote(NOTE_C4,E_NOTE);
	}
	winTune.addNote(NOTE_C4,W_NOTE);
}


void sett::drawSelf(void) { screen->fillScreen(BACK_COLOR); }



void sett::dealCards(int srtX,int srtY) {
	
	int			cardNum;
	settCard*	aCard;
	char*			theBGPath;
	
	theBGPath = NULL;																	// NULL default. The only way to fly.
	setFilePath(CARD_IMAGE);														// Get the file path for BG.
	heapStr(&theBGPath,mFilePath);												// Create a local copy of the path.
	if (theBGPath) {																	// We got it?
		for (int row=0;row<Y_CARDS;row++) {										// For every row..
			for (int col=0;col<X_CARDS;col++) {									// For every column..
				cardNum = cardList->dealCard();									// Get a card index from the card list.
				aCard = new settCard(cardNum,theBGPath,this);				// Create the card.
				if (aCard) {															// If we got a card.
					aCard->setLocation(srtX+col*CARD_W,srtY+row*CARD_H);	// Set it's location.
					addObj(aCard);
				}
			}
		}
		freeStr(&theBGPath);
	}
}
	
	
void sett::loop(void) {

/*
	switch(ourState) {
		case pregame	:
		
		break;						
		case waiting	:
		
		break;						
		case playing	:
		
		break;								
		case scoring	:
		
		break;
		case winning	:
			
		break;	
		case error		: 
		
		break;
	}
	*/
}


void sett::closing(void) {  }

   
   
// ******************** settCard ********************



settCard::settCard(int cardNum,const char* ourPath,sett* inGame)
	: bmpObj(GRID_X,GRID_Y,CARD_W,CARD_H,ourPath) {
	
	int	cardType;
	
	ourGame	= inGame;
	
	// Calculate number of items on card.
	if (cardNum<=27) {
		ourItemCount = 1;
		cardType = cardNum;
	} else if (cardNum>27 && cardNum <=54) {
		ourItemCount = 2;
		cardType = cardNum-27;
	} else {
		ourItemCount = 3;
		cardType = cardNum-54;
	}
	
	// Calculate type of shape on card.
	if (cardType<=9)								ourShape = Diamond;
	else if (cardType>9 && cardType<=18)	ourShape = Oval;
	else												ourShape = Swishy;
	
	// Calculate color of card.
	if (cardType<=3)								ourColor = Blue;
	else if (cardType>=10&&cardType<=12) 	ourColor = Blue;
	else if (cardType>=1&&cardType<=21)	 	ourColor = Blue;
	if (cardType>=4 && cardType<=6)			ourColor = Green;
	else if (cardType>=13&&cardType<=15) 	ourColor = Green;
	else if (cardType>=22&&cardType<=24)	ourColor = Green;
	if (cardType>=7 && cardType<=9)			ourColor = Red;
	else if (cardType>=16&&cardType<=18) 	ourColor = Red;
	else if (cardType>=25&&cardType<=27)	ourColor = Red;
	
	// Calculate fill type on card.
	switch (cardType) {
		case 1	:
		case 4	:
		case 7	:
		case 10	:
		case 13	:
		case 16	:
		case 19	:
		case 22	:
		case 25	:	ourFill = Half;	break;
		case 2	:
		case 5	:
		case 8	:
		case 11	:
		case 14	:
		case 17	:
		case 20	:
		case 23	:
		case 226	:	ourFill = Open;	break;
		default	:	ourFill = Solid;	break;
	}
}


settCard::~settCard(void) {  }
	
	
void settCard::drawSelf(void) {

	char		fName[8];
	char*		symbolFilePath;
	bmpObj*	symbolBmp;
	int		symX;
	int		symY[3];
	
	bmpObj::drawSelf();
	
	// Calculate different placements of symbols.
	symX = x + ((CARD_W - SYMBOL_W)/2);					
	symY[1] = y + (CARD_H/2) - (SYMBOL_H/2);
	symY[0] = symY[1] - (SYMBOL_H*2);
	symY[2] = symY[1] + (SYMBOL_H*2);
	
	// Calculate our shape's file name.
	switch(ourShape) {
		case Diamond	: fName[0] = 'd'; break;
		case Oval		: fName[0] = 'o'; break;
		default			: fName[0] = 's'; break;
	}
	switch(ourColor) {
		case Blue	: fName[1] = 'b'; break;
		case Green	: fName[1] = 'g'; break;
		default		: fName[1] = 'r'; break;
	}
	switch(ourFill) {
		case Solid	: fName[2] = 's'; break;
		case Half	: fName[2] = 'h'; break;
		default		: fName[2] = 'o'; break;
	}
	fName[3] = '\0';
	strcat(fName,".bmp");
	
	// Now let's try for the path.
	if (ourGame->setFilePath(fName)) {
		symbolBmp = new bmpObj(symX,0,SYMBOL_W,SYMBOL_H,ourGame->mFilePath);
		if (symbolBmp) {
			switch(ourItemCount) {
				case 1 	: 
					symbolBmp->setLocation(symX,symY[1]);
					symbolBmp->drawSelf();
				break;
				case 2 	:
					symbolBmp->setLocation(symX,symY[0]);
					symbolBmp->drawSelf();
					symbolBmp->setLocation(symX,symY[2]);
					symbolBmp->drawSelf();
				break;
				default	:
					symbolBmp->setLocation(symX,symY[0]);
					symbolBmp->drawSelf();
					symbolBmp->setLocation(symX,symY[1]);
					symbolBmp->drawSelf();
					symbolBmp->setLocation(symX,symY[2]);
					symbolBmp->drawSelf();
				break;
			}
			delete(symbolBmp);
		}	
	}
}




				