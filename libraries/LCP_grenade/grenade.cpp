#include <grenade.h>
#include <resizeBuff.h>
#include <SD.h>
#include <strTools.h>
#include <cardIndex.h>


#define UNCONNECTED_ANALOG_PIN	A1

#define NEXT_BTN_N	"next.bmp"
#define NEXT_BTN_X	53
#define NEXT_BTN_Y	240
#define NEXT_BTN_W	135
#define NEXT_BTN_H	60

#define THE_WORD_N	"theList.txt"
#define THE_WORD_X	20
#define THE_WORD_Y	140
#define THE_WORD_W	200
#define THE_WORD_H	16
#define THE_WORD_FR	100
#define THE_WORD_BLK	"---"

#define GAME_MS		1000.0 * 120
#define BEEP_PERIOD	1000
#define BEEP_PERCENT	90

#define SECOND_MULT	2
#define SECOND_PER	500
#define THIRD_MULT	2.5
#define THIRD_PER		300
#define FOURTH_MULT	3
#define FOURTH_PER	200

#define MESS_X			20
#define MESS_Y			185
#define MESS_W			200
#define MESS_H			8
#define MESS_START	"Click NEXT to begin."
#define MESS_PLAY		"Guess the words!"
#define MESS_RESET	"Click NEXT to restart."
#define MESS_SCORE	"Click this round's winner."

#define BACK_COLOR	&black

#define BLUE_CROSS	"bCross32.bmp"
#define B_BTN_X		148
#define B_BTN_Y		50
#define B_BTN_W		32
#define B_BTN_H		32

#define B_LBL_X		138
#define B_LBL_Y		85
#define B_LBL_W		60
#define B_LBL_H		8
#define B_LBL_T		"Blue team"

#define BS_LBL_X		B_BTN_X - 10
#define BS_LBL_Y		B_BTN_Y - 20
#define BS_LBL_W		B_BTN_W + 20
#define BS_LBL_H		16

#define G_BTN_X		60
#define G_BTN_Y		50
#define G_BTN_W		32
#define G_BTN_H		32

#define G_LBL_X		50
#define G_LBL_Y		85
#define G_LBL_W		60
#define G_LBL_H		8
#define G_LBL_T		"Green team"

#define GS_LBL_X		G_BTN_X - 10
#define GS_LBL_Y		G_BTN_Y - 20
#define GS_LBL_W		G_BTN_W + 20
#define GS_LBL_H		16

#define MAX_SCORE		5

#define	EXP_SND_MS	1000


grenade::grenade(int ourAppID)
	: panel(ourAppID) {
	
	theWord		= NULL;
	wordIndex	= NULL;
	nextButton	= NULL;
	ourBeeper	= NULL;
	ourPlayer	= NULL;
	wordCount	= 0;
	ourState		= pregame;
	greenScore	= 0;
	blueScore	= 0;
}



grenade::~grenade(void) {

	if (wordIndex) delete wordIndex;
	if (ourBeeper) delete ourBeeper;
	if (ourPlayer) delete ourPlayer;
}


void grenade::setup(void) {
	
	long int	seed;
	
	seed = analogRead(UNCONNECTED_ANALOG_PIN);	// Tired of playing the same old game every time?
	randomSeed(seed);
	
	setFilePath(BLUE_CROSS);
	blueCross = new blueBtn(B_BTN_X,B_BTN_Y,B_BTN_W,B_BTN_H,mFilePath);
	if (blueCross) {
		blueCross->setGreyedOut(true);
		blueCross->setGrenade(this);
		addObj(blueCross);
	}
	label* blueLbl = new label(B_LBL_X,B_LBL_Y,B_LBL_W,B_LBL_H,B_LBL_T);
	if (blueLbl) {
		blueLbl->setColors(&blue,BACK_COLOR);
		addObj(blueLbl);
	}
	blueScoreLbl = new label(BS_LBL_X,BS_LBL_Y,BS_LBL_W,BS_LBL_H,"0");
	if (blueScoreLbl) {
		blueScoreLbl->setTextSize(2);
		blueScoreLbl->setJustify(TEXT_RIGHT);
		blueScoreLbl->setColors(&white,BACK_COLOR);
		addObj(blueScoreLbl);
	}
	
	greenCross = new greenBtn(G_BTN_X,G_BTN_Y,G_BTN_W,G_BTN_H,OSPtr->stdIconPath(cross32));
	if (greenCross) {
		greenCross->setGreyedOut(true);
		greenCross->setGrenade(this);
		addObj(greenCross);
	}
	
	label* greenLbl = new label(G_LBL_X,G_LBL_Y,G_LBL_W,G_LBL_H,G_LBL_T);
	if (greenLbl) {
		greenLbl->setColors(&green,BACK_COLOR);
		addObj(greenLbl);
	}
	
	greenScoreLbl = new label(GS_LBL_X,GS_LBL_Y,GS_LBL_W,GS_LBL_H,"0");
	if (greenScoreLbl) {
		greenScoreLbl->setTextSize(2);
		greenScoreLbl->setJustify(TEXT_RIGHT);
		greenScoreLbl->setColors(&white,BACK_COLOR);
		addObj(greenScoreLbl);
	}
	
	if(setFilePath(NEXT_BTN_N)) {
		nextButton = new nextBtn(NEXT_BTN_X,NEXT_BTN_Y,NEXT_BTN_W,NEXT_BTN_H,mFilePath);
		if (nextButton) {
			nextButton->setGrenade(this);
			addObj(nextButton);
		}
	}
	wordCount = countWordList();
	theWord = new quickLabel(THE_WORD_X,THE_WORD_Y,THE_WORD_W,THE_WORD_H);
	if (wordCount && theWord) {
		wordIndex = new cardIndex(wordCount);
		if (wordIndex) {
			theWord->setTextSize(2);
			theWord->setJustify(TEXT_CENTER);
			theWord->setColors(&white,BACK_COLOR);
			theWord->setValue(THE_WORD_BLK);
			addObj(theWord);
		}
	}
	
	ourBeeper = new alarmBeeper(OSPtr->getTonePin(),GAME_MS,NOTE_A3,NOTE_G2);
	playTimer.setTime(GAME_MS,false);
	message = new label(MESS_X,MESS_Y,MESS_W,MESS_H,MESS_START);
	if (message) {
		message->setJustify(TEXT_CENTER);
		message->setColors(&white,BACK_COLOR);
		addObj(message);
	}
	ourPlayer = new toneObj(OSPtr->getTonePin());
	createSounds();
	if (
		blueCross
		&& blueLbl
		&& blueScoreLbl
		&& greenCross
		&& greenLbl
		&& greenScoreLbl
		&& nextButton
		&& wordCount
		&& theWord
		&& wordIndex 
		&& ourBeeper 
		&& message
		) { 
		ourState = waiting;						// Got all the bits? Then we wait.
	} else { 
		ourState = error;
	}
}


void grenade::createSounds(void) {

	explodeSnd.addNote(NOTE_F1,EXP_SND_MS);
	
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



void grenade::drawSelf(void) { screen->fillScreen(BACK_COLOR); }


int grenade::countWordList(void) {

	File			wordList;
	char			theChar;
	int			count;
	bool			done;
	
	count = 0;
	if(setFilePath(THE_WORD_N)) {
		wordList = SD.open(mFilePath,FILE_READ);
		if (wordList) {
			done = false;
			while(!done) {
				theChar = wordList.read();
				if (theChar==(char)-1) {
					done = true;
				} else {
					if (theChar=='\n') {
						count++;
					}
				}
			}
			if (count && theChar!='\n') {
				count++;
			}
			wordList.close();
		}
	}
	return count;
}

	
void grenade::showWinner(void) {
	
	
	screen->fillScreen(BACK_COLOR);
	winTune.startTune(ourPlayer);
	if (greenScore>blueScore) {
		theWord->setColors(&green,BACK_COLOR);
		theWord->setValue("GREEN WINS!!");
	} else {
		theWord->setColors(&blue,BACK_COLOR);
		theWord->setValue("BLUE WINS!!");
	}
	ourState =  winning;
}

		
void grenade::nextBtnClick(void) {

	switch(ourState) {
		case pregame	: break;						// Do nothing.
		case waiting	:								// Start a game.
			OSPtr->beep();							// Hear the beep.
			message->setValue(MESS_PLAY);			// Tell 'em what to do.
			chooseWord();								// Choose the first word.
			ourBeeper->setOnOff(true);				// Fire up beeper.
			playTimer.start();						// Start up our timer.
			ourState = playing;						// Now we're playing.
		break;											//
		case playing	:								// Game in process.
			OSPtr->beep();							// Hear the beep.
			chooseWord();								// Next word.
		break;											//
		case exploding	: 								// All of these.. Do nothing.
		case scoring	:								//
		case winning	: break;						//
		case error		: close();					// Give up something's broken.
	}
}



void grenade::greenBtnClick(void) {

	if (ourState==scoring) {
		OSPtr->beep();								// Hear the beep.
		greenScore++;									// Bump the score.
		greenScoreLbl->setValue(greenScore);	// Show the score.
		if (greenScore>=MAX_SCORE) {				// If we won..
			showWinner();								// Show that we won!
		} else {											// Else..
			blueCross->setGreyedOut(true);		// Highlight the blue cross as active.
			greenCross->setGreyedOut(true);		// Highlight the green cross as active.
			nextButton->setGreyedOut(false);		// The NEXT button is now inactive.
			message->setValue(MESS_RESET);		// Tell 'em time to reset.
			ourState = waiting;						// Back to waiting.
		}
	}
}


void grenade::blueBtnClick(void) {
	
	if (ourState==scoring) {
		OSPtr->beep();								// Hear the beep.
		blueScore++;									// Bump the score.
		blueScoreLbl->setValue(blueScore);		// Update the score.
		if (blueScore>=MAX_SCORE) {				// If we won..
			showWinner();								// Show that we won!
		} else {											// Else..
			blueCross->setGreyedOut(true);		// Highlight the blue cross as active.
			greenCross->setGreyedOut(true);		// Highlight the green cross as active.
			nextButton->setGreyedOut(false);		// The NEXT button is now inactive.
			message->setValue(MESS_RESET);		// Tell 'em time to reset.
			ourState = waiting;						// And we wait for it.
		}
	}
}


void grenade::chooseWord(void) {
	
	
	File			wordList;
	int			wordNum;
	char			aChar;
	char			buff[20];
	bool			done;
	int			count;
	
	if(setFilePath(THE_WORD_N)) {									// Calc. the path to the word list file.
		wordList = SD.open(mFilePath,FILE_READ);				// See if we can open this file.
		if (wordList) {												// If we got the file open..
			wordNum = wordIndex->dealCard();						// Get a word index from the card index.
			if (wordNum==-1) {										// If we ran out of choices..
				wordIndex->shuffle();								// Grab them all up and reload the list.
				wordNum = wordIndex->dealCard();					// Try dealing again, should work.
			}																//
			wordNum--;													// 'Cause we start at zero, not one.
			while(wordNum>0) {										// We'll count down words 'till we get to zero.
				if (wordList.read(&aChar,1)) {					// Read a char.
					if (aChar=='\n') {								// If it's a newline..
						wordNum--;										// Bump down the word num.
					}														//
				}															//
			}																//
			count = 0;													// count is for count of chars here.
			done = false;												// Should NOT be at the end of file.
			while(!done) {												// Loop about to copy the word.
				if(wordList.read(&aChar,1)) {						// If we can read a char..
					if (aChar!='\n') {								// If the char is NOT newline.
						buff[count] = aChar;							// We save off this char.
						count++;											// And bump up the string index.
					} else {												// Else it WAS an newline..
						buff[count] = '\0';							// That's it. Terminate the string.
						done = true;									// Flag the end.
					}														//
				} else {													// ELse we ran out of file..
					buff[count] = '\0';								// Terminate the string.
					done = true;										// Flag the end.	
				}															//
			}																//
			wordList.close();											// Close up the file.
			theWord->setValue(buff);								// Set the word to the screen.
		}
	}
}



void grenade::loop(void) {

	switch(ourState) {
		case pregame	:	break;						// Should NEVER be here.
		case waiting	:	break;						// Nothing to do..
		case playing	:									// Need to check timer.
			if (playTimer.ding()) {						// If the timer has expired..
				theWord->setValue(THE_WORD_BLK);		// Blank out the word.
				message->setValue("");					// Tell 'em nothin'.
				playTimer.reset();						// Play timer off.
				ourState = exploding;					// Let's show an explosion.
			}													//
		break;												// Off to the races..
		case exploding	:									// Explosion!!
			explodeSnd.startTune(ourPlayer);			// Let's here it.
			screen->fillScreen(&white);				// White
			sleep(100);										// Let others run for a bit...
			screen->fillScreen(BACK_COLOR);			// black
			sleep(100);										// Let others run for a bit...
			screen->fillScreen(&white);				// white
			sleep(100);										// Let others run for a bit...
			screen->fillScreen(BACK_COLOR);			// black;
			blueCross->setGreyedOut(false);			// Highlight the blue cross as active.
			greenCross->setGreyedOut(false);			// Highlight the green cross as active.
			nextButton->setGreyedOut(true);			// The NEXT button is now inactive.
			message->setValue(MESS_SCORE);			// Tell 'em time to score.	
			setNeedRefresh();								// Need a redraw.
			ourState = scoring;							// Now we are scoring.
		break;
		case scoring	: break;
		case winning	:
			if (!winTune.playing()) {
				blueCross->setGreyedOut(true);			// Highlight the blue cross as inactive.
				greenCross->setGreyedOut(true);			// Highlight the green cross as inactive.
				nextButton->setGreyedOut(false);			// The NEXT button is now active.
				theWord->setColors(&white,BACK_COLOR);	// Reset the colors.
				theWord->setValue(THE_WORD_BLK);			// Blank out the word.
				message->setValue(MESS_RESET);			// Tell 'em time to reset.
				setNeedRefresh();
				blueScore = 0;
				blueScoreLbl->setValue(blueScore);		// Update the score.
				greenScore = 0;
				greenScoreLbl->setValue(greenScore);	// Update the score.
				ourState = waiting;
			}
		break;	
		case error		: break;
	}
}


void grenade::closing(void) { if (ourBeeper) ourBeeper->setOnOff(false); }



// *************** quickLabel ***************

quickLabel::quickLabel(int inLocX, int inLocY, int inWidth,int inHeight)
	: label(inLocX,inLocY,inWidth,inHeight) { savedStr = NULL; }
	
	
quickLabel::~quickLabel(void) { free(savedStr); }
	
	
void quickLabel::setColors(colorObj* tColor, colorObj* bColor) {

	savedBColor.setColor(bColor);
	label::setColors(tColor,bColor);
	transp = true;
}


void quickLabel::setValue(const char* str) {

	if (buff) {
		heapStr(&savedStr,buff);
	}
	label::setValue(str);
}


void quickLabel::eraseSelf(void) {

	colorObj	savedTcolor;
	char*		newStr;
	
	if (savedStr) {
		newStr = NULL;								// Start our saved new string as NULL.
		heapStr(&newStr,buff);					// Allocate and stuff the new string in there.
		heapStr(&buff,savedStr);				// Reallocate the buff for the old saved string.
		savedTcolor.setColor(&textColor);	// Save off the old text color.
		textColor.setColor(&savedBColor);	// Pop in our saved background color.
		drawSelf();									// Now this -should- erase the old text.
		textColor.setColor(&savedTcolor);	// Reset the text color.
		heapStr(&buff,newStr);					// Reallocate the buff for the new string.
		free(newStr);								// Recycle the new string.
	}
}	
	


// *************** nextBtn ***************


nextBtn::nextBtn(int inX,int inY,int inWidth,int inHeight,const char* bmpPath)
	: bmpObj(inX,inY,inWidth,inHeight,bmpPath) {
	
	ourGrenadePtr	= NULL;
	setEventSet(touchLift);
}
	
	 
nextBtn::~nextBtn(void) { }


void nextBtn::setGrenade(grenade* inPtr) { ourGrenadePtr = inPtr; }


void nextBtn::doAction(void) {

	if (ourGrenadePtr) {
		ourGrenadePtr->nextBtnClick();
	}
}
	

// *************** greenBtn ***************


greenBtn::greenBtn(int inX,int inY,int inWidth,int inHeight,const char* bmpPath)
	: bmpObj(inX,inY,inWidth,inHeight,bmpPath) {
	
	ourGrenadePtr = NULL;
	setEventSet(touchLift);
}
	
	 
greenBtn::~greenBtn(void) { }


void greenBtn::setGrenade(grenade* inPtr) { ourGrenadePtr = inPtr; }


void greenBtn::doAction(void) {

	if (ourGrenadePtr) {
		ourGrenadePtr->greenBtnClick();
	}
}


// *************** blueBtn ***************


blueBtn::blueBtn(int inX,int inY,int inWidth,int inHeight,const char* bmpPath)
	: bmpObj(inX,inY,inWidth,inHeight,bmpPath) {
	
	ourGrenadePtr = NULL;
	setEventSet(touchLift);
}
	
	 
blueBtn::~blueBtn(void) { }


void blueBtn::setGrenade(grenade* inPtr) { ourGrenadePtr = inPtr; }


void blueBtn::doAction(void) {

	if (ourGrenadePtr) {
		ourGrenadePtr->blueBtnClick();
	}
}



// *************** alarmBeeper ***************


alarmBeeper::alarmBeeper(int inPin, float totalMs,int inHiHz, int inLowHz)
	:squareWave() {
	
	tonePin = inPin;
	alarmTimer.setTime(totalMs/4.0,false);
	hiHz	= inHiHz;
	lowHz	= inLowHz;
	mult	= 1;
	hiLow = true;
	setPeriod(BEEP_PERIOD);
	setPercent(BEEP_PERCENT);
}


alarmBeeper::~alarmBeeper(void) { noTone(tonePin); }
	
	
void alarmBeeper::pulseOn(void) {
	

	if (hiLow) {
		tone(tonePin,round(hiHz*mult));
	} else {
		tone(tonePin,round(lowHz*mult));
	}
	hiLow = !hiLow;
}
	
	
void alarmBeeper::pulseOff(void) {

	float	fraction;
	
	noTone(tonePin);
	fraction = alarmTimer.getFraction();
	if (mult==1&&fraction<=0.75 && fraction >0.5) {
		mult		= SECOND_MULT;
		setPeriod(SECOND_PER);
		setPercent(BEEP_PERCENT);
	} else if (mult==SECOND_MULT&&fraction<=0.5 && fraction >0.25) {
		mult		= THIRD_MULT;
		setPeriod(THIRD_PER);
		setPercent(BEEP_PERCENT);
	}  else if (mult==THIRD_MULT&&fraction <0.25) {
		mult		= FOURTH_MULT;
		setPeriod(FOURTH_PER);
		setPercent(BEEP_PERCENT);
	}
}
	
	
void alarmBeeper::setOnOff(bool onOff) {

	squareWave::setOnOff(onOff);
	if (onOff){
		setPeriod(BEEP_PERIOD);
		setPercent(BEEP_PERCENT);
		mult		= 1;
		alarmTimer.start();
		ourState = phase1;
	} else {
		noTone(tonePin);
	}
}


void alarmBeeper::idle(void) {

	squareWave::idle();
	switch(ourState) {
		case  waiting	:	break;
		case  phase1	:
			if (alarmTimer.ding()) {
				setPeriod(SECOND_PER);
				setPercent(BEEP_PERCENT);
				mult		= SECOND_MULT;
				alarmTimer.stepTime();
				ourState = phase2;
			}
		break;
		case  phase2	:
			if (alarmTimer.ding()) {
				setPeriod(THIRD_PER);
				setPercent(BEEP_PERCENT);
				mult		= THIRD_MULT;
				alarmTimer.stepTime();
				ourState = phase3;
			}
		break;
		case  phase3	:
			if (alarmTimer.ding()) {
				setPeriod(FOURTH_PER);
				setPercent(BEEP_PERCENT);
				mult		= FOURTH_MULT;
				alarmTimer.stepTime();
				ourState = phase4;
			}
		break;
		case  phase4	:
			if (alarmTimer.ding()) {
				setOnOff(false);
				alarmTimer.reset();
				ourState = waiting;
			}
		break;
	}
}



