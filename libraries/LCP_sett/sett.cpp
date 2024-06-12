#include <sett.h>
#include <strTools.h>


#define CARD_IMAGE	"card.bmp"
#define SELECT_IMAGE	"select.bmp"
#define CARD_W			60
#define CARD_H			74
#define SYMBOL_W		40
#define SYMBOL_H		16
#define GRID_X			55
#define GRID_Y			20
#define GRID_SPACE_X	5
#define GRID_SPACE_Y	5

#define RELOAD_IMAGE	"reload.bmp"
#define RELOAD_X		10
#define RELOAD_Y		40

#define MSG_MS			5000
#define LBL_X			30
#define LBL_Y			3
#define LBL_W			260
#define LBL_H			10





#define BACK_COLOR	&black

#define UNCONNECTED_ANALOG_PIN  A1



// **************************************************
// ******************  sett *************************
// **************************************************


sett::sett(int ourAppID)
	: panel(ourAppID) {
	
	cardPath = NULL;						// Init to NULL.
	setFilePath(CARD_IMAGE);			// Get the file path for BG.
	heapStr(&cardPath,mFilePath);		// Create a local copy of the path.
	
	selectPath = NULL;					// Init to NULL.
	setFilePath(SELECT_IMAGE);			// Get the file path for selected BG.
	heapStr(&selectPath,mFilePath);	// Create a local copy of the path.
	
	selectList	= new selectQ();
	for(int i=0;i<MAX_CARDS;i++) {
		cardPtrs[i] = NULL;
	}
	ourPlayer	= new toneObj(OSPtr->getTonePin());
	groupIndex = 0;
}


sett::~sett(void) { 

	freeStr(&cardPath);
	freeStr(&selectPath);
	if (selectList) delete(selectList);
	if (ourPlayer) delete ourPlayer;
}


void sett::setup(void) {
	
	long int		seed;
	reloadBtn*	theBtn;
	
	seed = analogRead(UNCONNECTED_ANALOG_PIN);								// Tired of playing the same old game every time?
	randomSeed(seed);																	// It's an antenna. Very random.	
	mesgPtr = new label(LBL_X,LBL_Y,LBL_W,LBL_H);							// Create the message label.
	if (mesgPtr) {																		// If we got a label..
		mesgPtr->setColors(&yellow,&black);										// Set it's colors.
		mesgPtr->setJustify(TEXT_LEFT);											// Set the text justify.
		mesgPtr->setTextSize(1);													// Size.
		addObj(mesgPtr);																// Add it to our screen.
		if (setFilePath(RELOAD_IMAGE)) {											// Ask for the file path to the button icon.
			theBtn = new reloadBtn(RELOAD_X,RELOAD_Y,mFilePath,this);	// Create the reload button.
			if (theBtn) {																// If we got it..
				addObj(theBtn);														// Add it to the screen.
				cardIndexList = new cardIndex(TOTAL_CARDS);					// Create the card index list.
				if (cardIndexList) {													// If we got it..
					ourPlayer = new toneObj(mOSPtr->getTonePin());			// Set up the beeper stuff.
					createSounds();													// Make up some ditties.
					dealCards(GRID_X,GRID_Y);										// Do the initial deal.
					ourState = playing;												// Got all the bits? We play!
					return;																// Bail out now to see what user does.
				}																			//
			}																				//
		}																					//
	}																						//
	ourState = error;																	// Our state is.. We have an error.
}


void sett::reloadGame(void) {

	if (ourState != error) {
		dealCards(GRID_X,GRID_Y);
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
	
	setFilePath("ONESET.MID");
   oneSet.createTune(mFilePath);
   oneSet.adjustSpeed(4);

}


void sett::drawSelf(void) { screen->fillScreen(BACK_COLOR); }



void sett::dealCards(int srtX,int srtY) {
	
	int			cardNum;
	int			i;
	settCard*	aCard;
	
	groupIndex = 0;																// Clear out the count of pairs.
	clearSelect();																	// Clear out the select list. If any.
	for(i=0;i<MAX_GROUPS;i++) {												// For all the saved card groups..
		groupList[i].clearCards();												// Clear out our saved sets. If any.
	}																					//
	for(i=0;i<MAX_CARDS;i++) {													// Go through the list..
		if (cardPtrs[i]) {														// If it ain't NULL..
			delete(cardPtrs[i]);													// Delete the card. It'll remove itself.
			cardPtrs[i] = NULL;													// NULL out the item as a flag.
		}																				//
	}																					//
	i = 0;																			// Reset the card count.
	for (int row=0;row<Y_CARDS;row++) {										// For every row..
		for (int col=0;col<X_CARDS;col++) {									// For every column..
			cardNum = cardIndexList->dealCard();							// Get a card index from the card list.
			aCard = new settCard(cardNum,cardPath,selectPath,this);	// Create the card.
			if (aCard) {															// If we got a card.
				cardPtrs[i++] = aCard;											// Add it to our list.
				aCard->setLocation(srtX+col*CARD_W,srtY+row*CARD_H);	// Set it's location.
				addObj(aCard);														// Add it to the screen.
			}
		}
	}
}
				
				
// We need to clear them.
void sett::clearSelect(void) { selectList->dumpList(); };


bool sett::duplicate(void) {

	settCard*	card1;
	settCard*	card2;
	settCard*	card3;
	cardGroup	tempSet;
	bool			found;
	int			i;
	
	card1 = ((cardListObj*)selectList->getByIndex(0))->cardPtr;
	card2 = ((cardListObj*)selectList->getByIndex(1))->cardPtr;
	card3 = ((cardListObj*)selectList->getByIndex(2))->cardPtr;
	tempSet.setCards(card1,card2,card3);
	found = false;
	i = 0;
	while(!found && i<6) {
		found = groupList[i++].isSame(&tempSet);
	}
	return found;
}


void sett::addSet(void) {

	settCard*	card1;
	settCard*	card2;
	settCard*	card3;
	
	card1 = ((cardListObj*)selectList->getByIndex(0))->cardPtr;
	card2 = ((cardListObj*)selectList->getByIndex(1))->cardPtr;
	card3 = ((cardListObj*)selectList->getByIndex(2))->cardPtr;
	groupList[groupIndex++].setCards(card1,card2,card3);
}


// A card wants to be selected.
void sett::selectMe(settCard* inCard) {

	cardListObj*	aCardObj;
	char				buffs[25];
	char				buffi[5];
	
	aCardObj = new cardListObj(inCard);
	selectList->push(aCardObj);
	if (selectList->isSett()) {
		if (!duplicate()) {
			addSet();
			if (groupIndex==6) {
				winTune.startTune(ourPlayer);
				ourState = winning;
			} else {
				strcpy(buffs,"Congrats on sett # ");
				itoa(groupIndex,buffi,10);
				strcat(buffs,buffi);
				strcat(buffs," of 6");
				setMsg(buffs,MSG_MS);
				oneSet.startTune(ourPlayer);
				ourState = scoring;
			}
		} else {
			setMsg("Already got that one.",MSG_MS);
		}
	}
}


// A card wants to be unselected.
void sett::unSelectMe(settCard* inCard) { selectList->deSelectObj(inCard); }
	
	
void  sett::setMsg(const char* text,float ms) {

	mesgPtr->setValue(text);
	msgTimer.setTime(ms);
}
							
void sett::loop(void) {

	switch(ourState) {
		case pregame	:
		
		break;						
		case waiting	:
		
		break;						
		case playing	:
			
		break;								
		case scoring	:
			if (!oneSet.playing()) {
				clearSelect();
				ourState = playing;
			}
		break;
		case winning	:
			
		break;	
		case error		: 
		
		break;
	}
	if (msgTimer.ding()) {
		mesgPtr->setValue("");
		msgTimer.reset();
	}
}


void sett::closing(void) {  }



// **************************************************
// ******************* reloadBtn *******************
// **************************************************	


reloadBtn::reloadBtn(int xLoc,int yLoc,const char* path,sett* inGame)
	: iconButton(xLoc,yLoc,path) { gamePtr = inGame; }


reloadBtn::~reloadBtn(void) {  }


	
void reloadBtn::doAction(void) {

	OSPtr->beep();
	if (gamePtr) {
		gamePtr->reloadGame();
	}
}
				


// **************************************************
// ******************* cardGroup ********************
// **************************************************


cardGroup::cardGroup(void) { clearCards(); }


cardGroup::~cardGroup(void) { }


void  cardGroup::clearCards(void) {
	
	groupCards[0] = NULL;
	groupCards[1] = NULL;
	groupCards[2] = NULL;
}


void cardGroup::setCards(settCard* card1,settCard* card2,settCard* card3) {

	groupCards[0] = card1;
	groupCards[1] = card2;
	groupCards[2] = card3;
}


bool cardGroup::foundCard(settCard* aCard) {

	
	if (groupCards[0] == aCard) return true;
	if (groupCards[1] == aCard) return true;
	if (groupCards[2] == aCard) return true;
	return false;
}

	
bool cardGroup::isSame(cardGroup* aCardGroupPtr) {

	if (foundCard(aCardGroupPtr->groupCards[0])
		&&foundCard(aCardGroupPtr->groupCards[1])
		&&foundCard(aCardGroupPtr->groupCards[2])) return true;
	return false;
}
				


// **************************************************
// ******************* cardListObj *******************
// **************************************************


cardListObj::cardListObj(settCard* inCard) { cardPtr = inCard; }


cardListObj::~cardListObj(void) { if (cardPtr) cardPtr->deSelect(); }



// **************************************************
// ******************** selectQ *********************
// **************************************************


selectQ::selectQ(void) {  }


selectQ::~selectQ(void) {  }

	
void selectQ::push(linkListObj* newObj) {

	cardListObj* oldCardObj;

	queue::push(newObj);
	if (getCount()>3) {
		oldCardObj = pop();
		delete(oldCardObj);
	}
}


void selectQ::deSelectObj(settCard* aCard) {
	
	bool			done;
	int			i;
	cardListObj* trace;
	
	i = 0;
	done = false;
	while(!done) {
		trace = getByIndex(i++);
		if (trace) {
			if (trace->cardPtr==aCard) {
				unlinkObj(trace);
				delete(trace);
				done=true;
			}
		} else {
			done=true;
		}
	}
}


bool selectQ::colorFail(settCard* selected[]) {

	bool	allMatch;
	bool	noneMatch;
	
	allMatch =	(selected[0]->ourColor == selected[1]->ourColor)
					&&(selected[0]->ourColor == selected[2]->ourColor);
	noneMatch = (selected[0]->ourColor != selected[1]->ourColor)
					&& (selected[0]->ourColor != selected[2]->ourColor)
					&& (selected[2]->ourColor != selected[1]->ourColor);
	return !(allMatch||noneMatch);
}


bool selectQ::shapeFail(settCard* selected[]) {

	bool	allMatch;
	bool	noneMatch;
	
	allMatch =	(selected[0]->ourShape == selected[1]->ourShape)
					&&(selected[0]->ourShape == selected[2]->ourShape);
	noneMatch = (selected[0]->ourShape != selected[1]->ourShape)
					&& (selected[0]->ourShape != selected[2]->ourShape)
					&& (selected[2]->ourShape != selected[1]->ourShape);
	return !(allMatch||noneMatch);
}


bool selectQ::fillFail(settCard* selected[]) {

	bool	allMatch;
	bool	noneMatch;
	
	allMatch =	(selected[0]->ourFill == selected[1]->ourFill)
					&&(selected[0]->ourFill == selected[2]->ourFill);
	noneMatch = (selected[0]->ourFill != selected[1]->ourFill)
					&& (selected[0]->ourFill != selected[2]->ourFill)
					&& (selected[2]->ourFill != selected[1]->ourFill);
	return !(allMatch||noneMatch);
}


bool selectQ::numFail(settCard* selected[]) {

	bool	allMatch;
	bool	noneMatch;
	
	allMatch =	(selected[0]->ourItemCount == selected[1]->ourItemCount)
					&&(selected[0]->ourItemCount == selected[2]->ourItemCount);
	noneMatch = (selected[0]->ourItemCount != selected[1]->ourItemCount)
					&& (selected[0]->ourItemCount != selected[2]->ourItemCount)
					&& (selected[2]->ourItemCount != selected[1]->ourItemCount);
	return !(allMatch||noneMatch);
}


bool  selectQ::isSett(void) {
	
	settCard*	selected[3];
	
	if (getCount()<3) return false;
	for(int i=0;i<3;i++) {
		selected[i] = ((cardListObj*)(getByIndex(i)))->cardPtr;
	}
	if (colorFail(selected)) return false;
	if (shapeFail(selected)) return false;
	if (fillFail(selected)) return false;
	if (numFail(selected)) return false;
	return true;
}

 
// **************************************************
// ******************** settCard ********************
// **************************************************


settCard::settCard(int cardNum,const char* cardPath,const char* selectPath,sett* inGame)
	: bmpFlasher(GRID_X,GRID_Y,CARD_W,CARD_H,selectPath,cardPath) {
	
	int	cardType;
	
	setEventSet(fullClick);
	ourGame	= inGame;
	selected	= false;
	pulseOff();
	
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


settCard::~settCard(void) { if (selected) ourGame->unSelectMe(this); }
	
					
// Responding to a user click..	
void settCard::toggleSelect(void) {

	selected = !selected;
	if (selected) {
		pulseOn();
		ourGame->selectMe(this);
	} else {
		pulseOff();
		ourGame->unSelectMe(this);
	}
	setNeedRefresh();
}


// Game wants this selection cleared..
void settCard::deSelect(void) {

	pulseOff();
	selected	= false;
	setNeedRefresh();
}


void settCard::doAction(void) { 

	OSPtr->beep();
	toggleSelect();
}


void settCard::drawSelf(void) {

	char		fName[8];
	bmpObj*	symbolBmp;
	int		symX;
	int		symY[5];
	
	bmpFlasher::drawSelf();
	
	// Calculate different placements of symbols.
	symX = x + ((CARD_W - SYMBOL_W)/2);					
	symY[1] = y + (CARD_H/2) - (SYMBOL_H/2);
	symY[0] = symY[1] - (SYMBOL_H*2)+10;
	symY[2] = symY[1] + (SYMBOL_H*2)-10;
	symY[3] = symY[0] + 10;
	symY[4] = symY[2] - 10;
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
					symbolBmp->setLocation(symX,symY[3]);
					symbolBmp->drawSelf();
					symbolBmp->setLocation(symX,symY[4]);
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




				