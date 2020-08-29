#include "resizeBuff.h"
#include "colorRect.h"
#include "starTrek.h"
#include "../../floraOS.h"
#include  "idlers.h"
#include "sst.h"

#define TV_INSET  3

// Screen back ground
#define SBG_X     5
#define SBG_Y     26
#define SBG_W     240-10
#define SBG_H     140

// Text view
#define TV_X    SBG_X + TV_INSET
#define TV_Y    SBG_Y + TV_INSET
#define TV_W    SBG_W - TV_INSET - TV_INSET
#define TV_H    SBG_H - TV_INSET - TV_INSET

// edit text back ground
#define ETBG_X    SBG_X
#define ETBG_Y    SBG_Y + SBG_H + 5
#define ETBG_W    SBG_W
#define ETBG_H    12

// Edit text.
#define ET_X      ETBG_X + TV_INSET
#define ET_Y      ETBG_Y + 2
#define ET_W      ETBG_W - TV_INSET - TV_INSET
#define ET_H      10

#define COMBUFF_BYTES	80
#define REPLYBUFF_BYTES	500

extern void doMakeMoves(void);

// Our output buffer that the ported code will see.
textBuff*	trekComBuffer;

// Their reply buffer.
textBuff*	trekReplyBuffer;


// Our screen that we'll send text to.
textView* ourTextView;


// Block 'till we get a char. I guess this is the waiting room.
int getch() {
	Serial.println("in getch()"); 
  while (trekComBuffer->empty()) ourOS.mPanel->sleep(10);
}


// And where it all goes out.. Well for now, we'll stuff it in here.
void proutn(char *s) { 

	Serial.println(s);
	trekReplyBuffer->addStr(s);
}


void proutCh(char c) {
	
	Serial.println(c);
	trekReplyBuffer->addChar(c);
}

// *****************************************************
//                      starTrekKeyboard
// *****************************************************


starTrekKeyboard::starTrekKeyboard(editLabel* inEditLabel,textView* inTextView,bool modal)
  : ourKeyboard(inEditLabel,modal) { 

  mEditLabel  = inEditLabel; // The display we live in will kill these two off.
  mTextView   = inTextView;   // So don't delete them.
}

  
starTrekKeyboard::~starTrekKeyboard(void) {  }


// Time to send a text command. The text, as it comes from the edit field has no \n.
// This works out well in that the receiving end, knowing the message is not complete
// can give us some time to get ready before adding the \n to fire off a response.
void starTrekKeyboard::handleMessage(char* msgBuff) {
 	
 	Serial.println("in handleMessage()");
 	Serial.println(msgBuff); 
	trekComBuffer->addStr(msgBuff,false);
	trekComBuffer->addStr("\n");
	mTextView->appendText(msgBuff);                     // Echo to our own screen.
	mTextView->appendText('\n');                        // With a EOL char.
}


// Same idea as IOandKeys
void starTrekKeyboard::handleKey(keyCommands inEditCom) {

  int   numChars;
  
  if (inEditCom == enter) {           // Got the return key.
    mEditLabel->mSuccess = true;
    mEditLabel->endEditing();
    handleMessage(mEditLabel->buff);  //Do what the user wants with the message.
    mEditLabel->setValue("");         // Clear the editField.
    mEditLabel->beginEditing();
  } else {
    keyboard::handleKey(inEditCom); // Or, do the default suff.
  }
}



// *****************************************************
//                      starTrekUpdater
// *****************************************************


starTrekUpdater::starTrekUpdater(starTrekPanel* inPanel) {

  mPanel = inPanel;
  hookup();
}


starTrekUpdater::~starTrekUpdater(void) {  }


void starTrekUpdater::idle(void) {

  while(!trekReplyBuffer->empty()) {                             // While the reply buffer is not empty..
    mPanel->ourScreen->appendText(trekReplyBuffer->readChar());  // Add each char to the screen.
  }
}



// *****************************************************
//                      starTrekPanel
// *****************************************************


starTrekPanel::starTrekPanel(void)
  : panel(starTrekApp) { mUpdater = new starTrekUpdater(this); }


starTrekPanel::~starTrekPanel(void) { if (mUpdater) delete mUpdater; }

          
void starTrekPanel::setup(void) {
Serial.println("in setup");
	colorObj    			screenColor(LC_DARK_GREEN);
	colorRect*  			aRect;
	starTrekKeyboard*	theKeybaord;

	// screen background.
	aRect = new colorRect(SBG_X,SBG_Y,SBG_W,SBG_H);
	aRect->setColor(&screenColor);
	addObj(aRect);

	// screen
	ourScreen = new textView(TV_X,TV_Y,TV_W,TV_H);
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
	theKeybaord = new starTrekKeyboard(anEditLabel,ourScreen,false);
	theKeybaord->loadKeys();
	addObj(theKeybaord);
  
	trekComBuffer = new textBuff(COMBUFF_BYTES);
	trekReplyBuffer = new textBuff(REPLYBUFF_BYTES);
	prelim();
	fromcommandline = 0;
}


void starTrekPanel::drawSelf(void) { screen->fillScreen(&black); } 


void starTrekPanel::loop(void) {
Serial.println("in loop() calling setupsst()");
	 setupsst();

	 if (alldone) {
		score(0);
		alldone = 0;
	 } else {
Serial.println("calling makemoves()");Serial.flush();
	 	doMakeMoves();
	 }
	 skip(2);
	 stars();
	 skip(1);
	 if (tourn && alldone) {
		prout((char*)"Do you want your score recorded?");
		if (ja()) {
			chew2();
			freeze(FALSE);
		}
	 }
	 prout((char*)"Do you want to play again?");
	 if (!ja()) close();
}
 


// We are being closed down.
void starTrekPanel::closing(void) {
  
  skip(1);
  prout((char*)"May the Great Bird of the Galaxy roost upon your home planet.");
  sleep(2000);
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
