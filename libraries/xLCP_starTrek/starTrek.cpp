#include "resizeBuff.h"
#include "colorRect.h"
#include "starTrek.h"
#include "idlers.h"
#include "sst.h"

#define TV_INSET 1

// Screen background
#define SBG_X     5
#define SBG_Y     26
#define SBG_W     240-10
#define SBG_H     140

// Text view
#define TV_X    SBG_X + TV_INSET
#define TV_Y    SBG_Y + TV_INSET
#define TV_W    SBG_W - TV_INSET - TV_INSET
#define TV_H    SBG_H - TV_INSET - TV_INSET

// edit text background
#define ETBG_X    SBG_X
#define ETBG_Y    SBG_Y + SBG_H + 5
#define ETBG_W    SBG_W
#define ETBG_H    12

// Edit text.
#define ET_X      ETBG_X + TV_INSET
#define ET_Y      ETBG_Y + 2
#define ET_W      ETBG_W - TV_INSET - TV_INSET
#define ET_H      10

#define MAX_LINES	32					// How many lines of text before we start choppin' 'em.


// *****************************************************
//                      starTrekKeyboard
// *****************************************************


starTrekKeyboard::starTrekKeyboard(starTrekPanel* inPanel,editLabel* inEditLabel,textView* inTextView,bool modal)
  : bmpKeyboard(inEditLabel,modal) { 

  mPanel			= inPanel;			// The panel we're living on.
  mEditLabel	= inEditLabel; 	// The display we live in will kill these two off.
  mTextView		= inTextView;		// So don't delete them.
}

  
starTrekKeyboard::~starTrekKeyboard(void) {  }


// Make the keys beep.
void starTrekKeyboard::keyClicked(keyboardKey* aKey) { mOSPtr->beep(); }


// Time to send a text command. The text, as it comes from the edit field has no \n.
// This works out well in that the receiving end, knowing the message is not complete
// can give us some time to get ready before adding the \n to fire off a response.
void starTrekKeyboard::handleMessage(char* msgBuff) {
 	
	trekComBuffer->addStr(msgBuff,false);
	trekComBuffer->addChar('\n');
	trekReplyBuffer->addStr(msgBuff,false);	// Echo to our own screen.
	trekReplyBuffer->addChar('\n');				// With a EOL char.
}


// Same idea as IOandKeys
void starTrekKeyboard::handleKey(keyCommands inEditCom) {

  int   numChars;
  
	if (inEditCom == enter) {           // Got the return key.
		mEditLabel->mSuccess = true;
		mEditLabel->endEditing();
		handleMessage(mEditLabel->buff);	// Do what the user wants with the message.
		mEditLabel->setValue("");			// Clear the editField.
		mEditLabel->beginEditing();
		bmpKeyboard::handleKey(chars);	// Set the keyboard back to chars.
	} else {
		keyboard::handleKey(inEditCom);	// Or, do the default suff.
	}
}



// *****************************************************
//                      starTrekUpdater
// *****************************************************


starTrekUpdater::starTrekUpdater(starTrekPanel* inPanel) {

  mPanel		= inPanel;
  mParsing	= false;
  mParser.addCmd(clearScr,"[2J");
  hookup();
}


starTrekUpdater::~starTrekUpdater(void) {  }


// Parse escape sequence chars. This is a little different because they use a set of
// chars as the terminator, not white space. So, we run the chars through 'till we see an
// end char. Run the end char through. Then, stuff in an EOL to terminate the parsing.
void starTrekUpdater::parseChar(char aChar) {
	
	int   command;

	command = mParser.addChar(aChar);								// Try parsing what we have. (It won't work, but keeps track.)
	if (aChar >= 0x40 && aChar <= 0x7E && aChar!= '[') {		// If its in the terminating set.. (But not '[')
		command = mParser.addChar(EOL);								// We need to fool the parser. So send the end char.
	}
	switch (command) {													// Check the results.
		case noCSI 		: break;											// Nothing to report, move along.
		case clearScr  :													// Found the clear screen sequence.
			mPanel->ourScreen->setText("");							// Clear the screen.
			mParsing = false;												// No longer in control sequence.
		break;																// our work is done. Lets go!
		default        : 
			mPanel->ourScreen->appendText(aChar);					// Oh well, pass it on to the screen. False alarm.
			mParsing = false;												// No longer in control sequence.	
		break;	
	}
}


void starTrekUpdater::idle(void) {

	char	aChar;
	
	while(!trekReplyBuffer->empty()) {				// While the reply buffer is not empty..
		aChar = trekReplyBuffer->readChar();		// We grab the next char out of the buffer.
		if (mParsing) {									// If we are parsing..
			parseChar(aChar);								// We pass the char to the parser.
		} else if (aChar=='\e') {						// Else, not parsing, but found a control char..
			mParsing = true;								// Start parsing. (Don't pass on the control char.)
		} else {												// Else, not parsing and not a control char..
			mPanel->ourScreen->appendText(aChar);  // Finally, pass this char to the screen.
		}
	}
}



// *****************************************************
//                      starTrekText
// *****************************************************


starTrekText::starTrekText(int inLocX, int inLocY, int inWidth,int inHeight,eventSet inEventSet)
	: textView(inLocX,inLocY,inWidth,inHeight,inEventSet) {  }


starTrekText::~starTrekText(void) {  }


void	starTrekText::appendText(char* text) {
	
	if (mManager.getNumLines()>MAX_LINES) {
		deleteText(0,strlen(text));
	}
	textView::appendText(text);
}


void	starTrekText::appendText(char aChar) {
	
	if (mManager.getNumLines()>MAX_LINES) {
		deleteText(0,1);
	}
	textView::appendText(aChar);
}

	




// *****************************************************
//                      starTrekPanel
// *****************************************************


starTrekPanel::starTrekPanel(int ourAppID)
  : panel(ourAppID) {
  
  mUpdater = new starTrekUpdater(this);
  quickExit = false;
}


starTrekPanel::~starTrekPanel(void) { if (mUpdater) delete mUpdater; }

          
void starTrekPanel::setup(void) {

	colorObj    			screenColor(LC_DARK_GREEN);
	colorRect*  			aRect;
	starTrekKeyboard*	theKeybaord;

	// For now, we are going to trust that the SD drive is all set up and running.
	SDDriveReady = true;

	// screen background.
	aRect = new colorRect(SBG_X,SBG_Y,SBG_W,SBG_H);
	aRect->setColor(&screenColor);
	addObj(aRect);

	// screen
	ourScreen = new starTrekText(TV_X,TV_Y,TV_W,TV_H);
	ourScreen->setTextSize(1);
	ourScreen->setTextColors(&green,&screenColor);
	addObj(ourScreen);

	// edit text background.
	aRect = new colorRect(ETBG_X,ETBG_Y,ETBG_W,ETBG_H);
	aRect->setColor(&screenColor);
	addObj(aRect);

	// editLabel
	editLabel* anEditLabel;
	anEditLabel = new editLabel(ET_X,ET_Y,ET_W,ET_H,"");
	anEditLabel->setColors(&green,&screenColor);
	anEditLabel->setJustify(TEXT_LEFT);
	anEditLabel->setEventSet(fullClick);
	anEditLabel->beginEditing();
	addObj(anEditLabel);

	// KEYBOARD
	theKeybaord = new starTrekKeyboard(this,anEditLabel,ourScreen,false);
	theKeybaord->loadKeys();
	addObj(theKeybaord);
  	
	arduinoTrekSetup("");
}


void starTrekPanel::drawSelf(void) { screen->fillScreen(&black); } 


void starTrekPanel::close(void) { quickExit = true; }


void starTrekPanel::loop(void) {

	arduinoTrekLoop();
	if (quickExit) {
		panel::close();
		return;
	}
	prout((char*)"Do you want to play again?");
	if (!ja()) {
		skip(1);
  		prout((char*)"May the Great Bird of the Galaxy roost upon your home planet.");
  		sleep(4000);
		close();
	}
}
 


// We are being closed down.
void starTrekPanel::closing(void) {
  
  if (mUpdater) {
    delete mUpdater;
    mUpdater = NULL;
  }
  if (trekComBuffer) {
  	delete(trekComBuffer);
  	trekComBuffer = NULL;
  }
  if (trekReplyBuffer) {
  	delete(trekReplyBuffer);
  	trekReplyBuffer = NULL;
  }
} 
