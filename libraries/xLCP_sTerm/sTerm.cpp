#include "colorRect.h"
#include "sTerm.h"


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
//                      sTermKeyboard
// *****************************************************


sTermKeyboard::sTermKeyboard(sTerm* inPanel,editLabel* inEditLabel,textView* inTextView,bool modal)
  : bmpKeyboard(inEditLabel,modal) { 

  mPanel			= inPanel;			// The panel we're living on.
  mEditLabel	= inEditLabel; 	// The display we live in will kill these two off.
  mTextView		= inTextView;		// So don't delete them.
}

  
sTermKeyboard::~sTermKeyboard(void) {  }


// Make the keys beep.
void sTermKeyboard::keyClicked(keyboardKey* aKey) { OSPtr->beep(); }


// Time to send a text command. The text, as it comes from the edit field has no \n.
// This works out well in that the receiving end, knowing the message is not complete
// can give us some time to get ready before adding the \n to fire off a response.
void sTermKeyboard::handleMessage(const char* msgBuff) {
 	
 	mPanel->ourScreen->appendText(msgBuff);	// Echo it.
 	mPanel->ourScreen->appendText("\n");		// With a EOL char.
 	mPanel->ourOutStream->println(msgBuff);	// And send it out the stream with OLD char.
}


// Same idea as IOandKeys
void sTermKeyboard::handleKey(keyCommands inEditCom) {
  
	if (inEditCom == enter) {           			// Got the return key.
		mEditLabel->mSuccess = true;					// Tell the label we're good with the edits.
		mEditLabel->endEditing();						// And to shut down editing.
		handleMessage(mEditLabel->getTextBuff());	// Do what the user wants with the message.
		mEditLabel->setValue("");						// Clear the editField.
		mEditLabel->beginEditing();					// Ok, reopen edit field for business.
		bmpKeyboard::handleKey(chars);				// Set the keyboard back to chars.
	} else {													// Else?
		keyboard::handleKey(inEditCom);				// Do the default suff.
	}
}



// *****************************************************
//                      sTermText
// *****************************************************


sTermText::sTermText(int inLocX, int inLocY, int inWidth,int inHeight,eventSet inEventSet)
	: textView(inLocX,inLocY,inWidth,inHeight,inEventSet) {  }


sTermText::~sTermText(void) {  }


void	sTermText::appendText(const char* text) {
	
	if (mManager.getNumLines()>MAX_LINES) {
		deleteText(0,strlen(text));
	}
	textView::appendText(text);
}


void	sTermText::appendText(char aChar) {
	
	if (mManager.getNumLines()>MAX_LINES) {
		deleteText(0,1);
	}
	textView::appendText(aChar);
}

	
	
// *****************************************************
//                      sTerm
// *****************************************************


sTerm::sTerm(int ourAppID)
  : panel(ourAppID) {
  
  ourInStream = &Serial;
  ourOutStream = &Serial;
}


sTerm::~sTerm(void) { }

          
void sTerm::setup(void) {

	colorObj    			screenColor(LC_DARK_GREEN);
	colorRect*  			aRect;
	sTermKeyboard*	theKeybaord;

	// screen background.
	aRect = new colorRect(SBG_X,SBG_Y,SBG_W,SBG_H);
	aRect->setColor(&screenColor);
	addObj(aRect);

	// screen
	ourScreen = new sTermText(TV_X,TV_Y,TV_W,TV_H);
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
	theKeybaord = new sTermKeyboard(this,anEditLabel,ourScreen,false);
	theKeybaord->loadKeys();
	addObj(theKeybaord);
  	
}


void sTerm::setStream(Stream* inStream,Stream* outStream) {

	ourInStream		= inStream;
	ourOutStream	= outStream;
}


void sTerm::drawSelf(void) { screen->fillScreen(&black); } 


//void sTerm::close(void) {  }


void sTerm::loop(void) {

	char	aChar;
	
	if (ourInStream->available()) {
		aChar = ourInStream->read();
		ourScreen->appendText(aChar);
	}
}
 


// We are being closed down.
void sTerm::closing(void) { } 
