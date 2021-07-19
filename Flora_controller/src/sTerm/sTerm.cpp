#include "resizeBuff.h"
#include "colorRect.h"
#include "sTerm.h"
#include "../../floraOS.h"
#include  "idlers.h"


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



// *****************************************************
//                      sKeyboard
// *****************************************************


sKeyboard::sKeyboard(editLabel* inEditLabel,textView* inTextView,bool modal)
  : ourKeyboard(inEditLabel,modal) { 

  mEditLabel  = inEditLabel; // The display we live in will kill these two off.
  mTextView   = inTextView;   // So don't delete them.
}

  
sKeyboard::~sKeyboard(void) {  }


// Time to send a text command. The text, as it comes from the edit field, has no \n.
// This works out well in that the receiving end, knowing the message is complete
// can give us some time to get ready before adding the \n to fire off a response.
void sKeyboard::handleMessage(char* msgBuff) {

	byte  numChars;
	char* outBuff;

	outBuff = NULL;                                    	// Going to use resizeBuff ALWAYS init to NUll.
	numChars = strlen(msgBuff) + 2;                      	// Num bytes = the bytes of the string, a command byte and the null terminator.
	if (resizeBuff(numChars,&outBuff)) {                	// See if we can grab a buffer.
	 outBuff[0] = (char)textCom;                        	// Next byte is the command byte. "We're sending a text string."
	 strcpy(&(outBuff[1]),msgBuff);                     	// The text string.
	 if (ourComPort.sendBuff(outBuff,numChars,false)) {	// Send it, no reply because we should get an uncotrolled flood of text.
		while(ourComPort.isSending()) {                   	// We'll spin 'till its at least shipped off.
		  ourComPort.sleep(QCOM_SLEEP_TIME);            	// Have a nap, let the UI amuse the user.
		}
		resizeBuff(0,&outBuff);                          	// Recycle the outBuff.
		mTextView->appendText(msgBuff);                  	// Echo to our own screen.
		mTextView->appendText('\n');                     	// With a EOL char.
		return;															// Completed successfully, exit.                      
	 }                                          
	}
	mTextView->appendText("Not enough RAM for message buffer.");  // Tell Mrs user we're not happy.
	mTextView->appendText('\n');
}


// Same idea as IOandKeys
void sKeyboard::handleKey(keyCommands inEditCom) {

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
//                      sTermUpdater
// *****************************************************

sTermUpdater::sTermUpdater(sTermPanel* inPanel) {

  mPanel = inPanel;
  hookup();
}


sTermUpdater::~sTermUpdater(void) {  }


void sTermUpdater::idle(void) {

  while(!mPanel->mReplyBuff->empty()) {                             // While the reply buffer is not empty..
    mPanel->ourScreen->appendText(mPanel->mReplyBuff->readChar());  // Add each char to the screen.
  }
}



// *****************************************************
//                      sTermPanel
// *****************************************************


sTermPanel::sTermPanel(void)
  : panel(sTermApp) {
  
  mReplyBuff = new textBuff(1000);
  mUpdater = new sTermUpdater(this);
}


sTermPanel::~sTermPanel(void) {
  
  if (mUpdater) delete mUpdater;
  if (mReplyBuff) delete mReplyBuff;
}

          
void sTermPanel::setup(void) {

  colorObj    screenColor(LC_DARK_GREEN);
  colorRect*  aRect;
  sKeyboard*  theKeybaord;

  ourComPort.disableBG(true);                         // Shut down automatic update machine. We want the line for ourselves.
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
  theKeybaord = new sKeyboard(anEditLabel,ourScreen,false);
  theKeybaord->loadKeys();
  addObj(theKeybaord);
}


void sTermPanel::drawSelf(void) { screen->fillScreen(&black); } 



void sTermPanel::loop(void) {

  timeObj timer(10);

  if (Serial1.available()) {								// If we have an incoming char..
    timer.start();											// Start our timer.
    while (!mReplyBuff->full()&&!timer.ding()) {	// While our buffer is NOT full and the timer is not expired..   
      while (Serial1.available()) {						// While we have something to grab from the serial port..
        mReplyBuff->addChar(Serial1.read());			// Grab a char from the serial port and stuff it into our buffer.
        timer.start();										// We got one, reset the timer.
      }
    }
  }
}


// We are being closed down. Release the com line and re-enable the 
// automatic update machine.
void sTermPanel::closing(void) {
  
  ourComPort.disableBG(false);
  if (mUpdater) {
    delete mUpdater;
    mUpdater = NULL;
  }
} 
