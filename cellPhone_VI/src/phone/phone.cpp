
#include "phone.h"
#include <cellCommon.h>
#include <drawDelete.h>
#include <resizeBuff.h>
#include <PNLabel.h>
#include "cellManager.h"
#include	"../../cellOS.h"
#include "../cellListener/cellListener.h"
#include "../contacts/contactPanel.h"

#define STAT_TIME   2500  // ms between checks on status.


#define KEY_W       40
#define KEY_H       40 //30

#define ROW_GAP     50

#define KEY_R0      30
#define KEY_R1      KEY_R0 + ROW_GAP - 10
#define KEY_R2      KEY_R1 + ROW_GAP
#define KEY_R3      KEY_R2 + ROW_GAP
#define KEY_R4      KEY_R3 + ROW_GAP
#define KEY_R5      KEY_R4 + ROW_GAP

#define COL_GAP     50 //70

#define KEY_C1      50 //30
#define KEY_C2      KEY_C1 + COL_GAP
#define KEY_C3      KEY_C2 + COL_GAP

#define EB_X    KEY_C1 - 20
#define EB_Y    KEY_R0 + 5
#define EB_W    (KEY_W*3)+(COL_GAP-KEY_W)*2+40
#define EB_H    16

#define ET_INSET  3                     
#define ET_X      EB_X+ET_INSET
#define ET_Y      EB_Y+4
#define ET_W      EB_W-(2*ET_INSET)
#define ET_H      10

#define ST_X      ET_X
#define ST_Y      ET_Y + 14
#define ST_W      ET_W
#define ST_H      ET_H

#define KEY_RAD     4

#define TEXT_SIZE   2
#define CHAR_WIDTH  TEXT_SIZE * 6
#define TEXT_HEIGHT TEXT_SIZE * 8

#define BATT_X    199
#define BATT_Y    4

#define SIG_X     BATT_X + 15
#define SIG_Y     BATT_Y


contact*	pleaseCall;	// The cellOS will set this to NULL on bootup.

// *****************************************************
// ********************  phoneBtn  *********************
// *****************************************************


phoneBtn::phoneBtn(int x,int y,char inKey,phone* inPhone)
  : drawObj(x,y,KEY_W,KEY_H,touchLift) {
  
  mKeystroke[0] = inKey;
  mKeystroke[1] = '\0';
  mPhone = inPhone;
  mPhone->addObj(this);    // If we add to our phone object, we'll be auto deleted when its recycled.
}


phoneBtn::~phoneBtn(void) {  }


void phoneBtn::drawSelf(void) {

	bmpPipe	aPipe;
	rect		aRect;
	
	aRect.setRect(0,0,40,40);
	aPipe.setSourceRect(&aRect);
	switch(mKeystroke[0]) {
		case '0' :   aPipe.openPipe(ICON_ZERO40); break;
		case '1' :   aPipe.openPipe(ICON_ONE40); break;
		case '2' :   aPipe.openPipe(ICON_TWO40); break;
		case '3' :   aPipe.openPipe(ICON_THREE40); break;
		case '4' :   aPipe.openPipe(ICON_FOUR40); break;
		case '5' :   aPipe.openPipe(ICON_FIVE40); break;
		case '6' :   aPipe.openPipe(ICON_SIX40); break;
		case '7' :   aPipe.openPipe(ICON_SEVEN40); break;
		case '8' :   aPipe.openPipe(ICON_EIGHT40); break;
		case '9' :   aPipe.openPipe(ICON_NINE40); break;
		case 'D' :   aPipe.openPipe(ICON_DELETE40); break;
		case '*' :   aPipe.openPipe(ICON_STAR40); break;
		case '#' :   aPipe.openPipe(ICON_HASH40); break;
	}
	if (!clicked) {
		aPipe.drawImage(x,y);
	} else {
		screen->fillRoundRect(x,y,width,height,KEY_RAD,&lightbButtonColorHit);  // Gives a cool highlight.
	}
}


// Button got a click? send its saved keystroke character to our phone
// object.
void phoneBtn::doAction(void) { mPhone->keystroke(mKeystroke[0]); }


          
// *****************************************************
// *******************  callControl  *******************
// *****************************************************


// CallControl is a "special" button that controls the calling or hang-up
// commands for the phone. So it has a special size, special colors, special
// handling of its actions.. Its its own thing. Inherited from phoneBtn.
callControl::callControl(int x,int y,char inKey,phone* inPhone)
  : phoneBtn(x,y,inKey,inPhone) {

	width					= width + COL_GAP;  // All the buttons are smaller, we're "special".
	mCallingID			= -1;
	mHangupID			= -1;
	mState				= wakeUp;
	mSavedCallerID[0]	= '\0';
	graceTimer.setTime(GRACE_TIME);
}


// We're going down, blindly send out a hangup command.
callControl::~callControl(void) {

	mHangupID = ourCellManager.sendCommand(hangUp,false); 	// And make sure we hang up the phone .
}


// Not only is there a bunch of drawing code for this fancy button, we also
// mix in calls to the message text about what's going on in the world of
// cell phone hardware.
void callControl::drawSelf(void) {

	contact*	aContact;
	char		PNBuff[CID_BYTES];
	PNLabel	formatter(1,1,1,1,1);
	bmpPipe	aPipe;
	rect		aRect;
	
	hookup();
	aRect.setRect(0,0,width,height);
	aPipe.setSourceRect(&aRect);
	switch(mState) {
		case wakeUp			:
		case isIdle       :
			if (!clicked) {
				aPipe.openPipe(ICON_PICKUP90);
				aPipe.drawImage(x,y);
			} else {
				screen->fillRoundRect(x,y,width,height,KEY_RAD,&greenButtonHighlight);
			}
		break;
		case hasIncoming  :
			if (!clicked) {
				aPipe.openPipe(ICON_PICKUP90);
				aPipe.drawImage(x,y);
			} else {
				screen->fillRoundRect(x,y,width,height,KEY_RAD,&greenButtonHighlight);
			}
			if (statusReg.callerID[0]=='\0') {
				mPhone->out("Unknown caller.");
			} else {
				strcpy(PNBuff,statusReg.callerID);
				aContact = ourBlackBook->findByPN(PNBuff);
				if (aContact) {
					mPhone->out(aContact->mNickName);
				} else {
					 formatter.setValue(PNBuff);
					 mPhone->out(formatter.buff);
				}
			}
		break;
		case connecting	:
			aPipe.openPipe(ICON_GDOTS90);
			aPipe.drawImage(x,y);
		break;
		case hangingUp		:
			aPipe.openPipe(ICON_RDOTS90);
			aPipe.drawImage(x,y);
		case isConnected  :
			if (!clicked) {
				aPipe.openPipe(ICON_HANGUP90);
				aPipe.drawImage(x,y);
			} else {
				screen->fillRoundRect(x,y,width,height,KEY_RAD,&redButtonHighlight);
			}
		break;   
	}
}


// Our actions. We have three states we can effect. idle, ringning or talking.
// When we get a click, its time to change state. All three methods will
// fire off our state in a new direction.
void callControl::doAction(void) {

	char*	numBuff;
	
	switch(mState) {
		case wakeUp			: break;																			// You get only a couple microseconds here.
		case isIdle			: 
			numBuff = NULL;																					// Init numBuff for later.
			if (haveService()) {																				// If we have service..
				if (resizeBuff(mPhone->numDisplay->getNumChars()+1,(uint8_t**)&numBuff)) {	// If we can allocate the text buffer..
					mPhone->numDisplay->getText(numBuff);												// Grab the number off the display.
					filterPNStr(numBuff);																	// Filter the formatting out of it.
					mCallingID = ourCellManager.sendCommand(makeCall,numBuff,true);			// Send the call command.
					resizeBuff(0,(uint8_t**)&numBuff);													// Recycle numBuff.
					mPhone->out("Calling..");																// Show the user we're doing it.
					mState = connecting;																		// Slip into connecting state.
				} else {																							// We didn't get the RAM?
					mPhone->out("Memory error?!");														// Show the user we ran out of memory.
				}
			} else {
				mPhone->out("No service.");																// No cell service, tell' em.
			}
		break;
		case hasIncoming	: 
    		mCallingID = ourCellManager.sendCommand(pickUp,true);
    		mPhone->out("Answering..");
    		mState = connecting;
		break;
		case connecting	: break;																			// We do nothing in the intermediate states.										
		case hangingUp		: break;
		case isConnected	:
			mHangupID = ourCellManager.sendCommand(hangUp,true);									// Create the command and send it on its way. 
    		mPhone->out("Disconnecting..");																// Show were going to try hanging up.
    		mState = hangingUp;
		break;
  }
}


// Is there cell service available at the moment?
bool callControl::haveService(void) {

	return statusReg.networkStat==NS_registeredHome || statusReg.networkStat==NS_registeredRoaming;
}


// Do we have an active connection to someone right now?
bool callControl::connected(void) { return mState==isConnected; }


// callControl also manages the phone status in the background. It can initiate a
// call upon creation, via the pleasCall variable. That's somewhat of a
// special task. The rest are keeping track of the phone object state
// changes and making sure we reflect those.
void callControl::idle() {
	
	int   numBytes;
	byte  reply;
   
   /*
	switch (statusReg.callStat) {
		case 0  : mPhone->out("Ready"); break;
		case 1  : mPhone->out("No Status"); break;
		case 2  : mPhone->out("Unknown"); break;
		case 3  : mPhone->out("Ringing In"); break;
		case 4  : mPhone->out("Ringing Out"); break;
		default : mPhone->out(statusReg.callStat); break; 
	}
	*/
  	//mPhone->out(statusReg.statNum);
	switch(mState) {
		case wakeUp 			:
			mState = isIdle;												// NAs a default, wakeUp is done. We want isIdle now.									
			if (pleaseCall) {												// We've been woken up to make this call.
				mPhone->numDisplay->setValue(pleaseCall->mPN);	// Grab the number off the saved contact.
				pleaseCall = NULL;										// Clear out the saved contact ptr.
				doAction();													// Since we are now in "isIdle" we call doAction to connect.
				setNeedRefresh();											// Show it.
			} else if (statusReg.callStat==CS_ringingIn) {		// Else if we woke up to a connection ringing in..
				strcpy(mSavedCallerID,statusReg.callerID);		// Copy the new string to our saved space.
				mState = hasIncoming;									// Set the state to deal with it.
				setNeedRefresh();											// As always, show it.
			}
		break;
		case isIdle       	:											// Waiting to see what happens.
			if (graceTimer.ding()) {									// Wait a bit for the state to reach us "through channels".
				if(statusReg.callStat==CS_ringingIn) {				// If we have a connection ringing in..
					strcpy(mSavedCallerID,statusReg.callerID);	// Copy the new string to our saved space.
					mState = hasIncoming;								// Set the state to deal with it.
					setNeedRefresh();										// As always, show it.
				}
			}
		break;
		case hasIncoming  	:
			if (strcmp(mSavedCallerID,statusReg.callerID)) {	// The caller ID has changed?
				strcpy(mSavedCallerID,statusReg.callerID);		// Copy the new string to our saved space.
				setNeedRefresh();											// Force a redraw to see the result.
			}
			if (statusReg.callStat==CS_ready) {						// Not connected but no longer ringing..
				mState = isIdle;											// I guess they gave up, back to isIdle.
				setNeedRefresh();											// As always, show it.
			}
		break;
		case	connecting		:											// A connection attempt is in process.
			checkCall();													// We do the checking in here.
		break;
		case	hangingUp		:											// Trying to hang up the phone.
			checkHangup();													// Again, we check the progress in here.
		break;
		case isConnected  	:															// We're connected..
			if (graceTimer.ding()) {													// Wait a bit for the state to reach us "through channels".
				if (statusReg.callStat==CS_ready) {									// If CS_ready means we lost connection.
					mState = isIdle;														// So we go back to idle state.
					setNeedRefresh();														// And show it.
				}
			}	
		break;
	}
}


// idle() wants us to check the status of our hangUp command. Basically
// this is the same kind of method as checkCall(). Just checking a different
// command with a little different result.
void callControl::checkHangup(void) {

	int   numBytes;
	byte  reply;
   
	switch (ourCellManager.progress(mHangupID)){						// See what's going on here..
		case com_standby  : 
		case com_working  :  break;										// Nothing we can do here.
		case com_holding  :													// Holding means there's a reply waiting for us.
			numBytes = ourCellManager.numReplyBytes(mHangupID);	// This is how big the reply is.
			if (numBytes==1) {												// We're looking for a one byte reply.
				ourCellManager.readReply(mHangupID,&reply);			// Grab the byte.
				if (reply==0) {												// Is it a zero?
					mPhone->out("Disconnectd.");							// Tell the world!
					ourCellManager.patchStatCallStat(0);				// Status can last awhile, patch it with the new info.
					mState = isIdle;											// Set the state.
				} else {
					mPhone->out("Hangup error.");							// Troubles somewhere.
					mState = isConnected;
				}
			} else {
				ourCellManager.dumpBuff();									// Unknown what it is. Toss it.
				mPhone->out("Reply error.");								// Something weird going on..
				mState = isConnected;										// We assume hangup failed. Still connected.
			}
			graceTimer.start();												// Start the grace timer. (Status lags and can mess us up.)
			mHangupID = -1;													// This command is done.
			setNeedRefresh();													// Show whatever changed.
		break;
		case com_complete :													// We missed something?													
			mState = isConnected;											// Assume failure.
			setNeedRefresh();													// Show new state.
		case com_missing  :                                   	// Possibly timed out or we already got it.
			mPhone->out("No response.");									// Just so we know.
			mState = isIdle;													// If the FONA crashed, we're not connected anymore..
			mHangupID = -1;													// In any case we're done with this command.
			setNeedRefresh();													// Things changed, lets see it.
		break;
	}
}
 
 
// idle() wants us to check the status of our makeCall command. It may be
// still working on making the call, it could have failed, possibly timed
// out? Whatever, we deal with it and take action appropriately here.
void callControl::checkCall(void) {

	int   numBytes;
	byte  reply;
  	
	switch (ourCellManager.progress(mCallingID)){					// See what's going on here..
		case com_standby  : 
		case com_working  :  break;										// Nothing we can do here.
		case com_holding  :													// Holding means there's a reply waiting for us.
			numBytes = ourCellManager.numReplyBytes(mCallingID);	// This is how big the reply is.
			if (numBytes==1) {												// We're looking for a one byte reply.
				ourCellManager.readReply(mCallingID,&reply);			// Grab the byte.
				if (reply==0) {												// Is it a zero?
					mPhone->out("Connected.");								// Tell the world!
					ourCellManager.patchStatCallStat(4);				// Status can last awhile, patch it with the new info.
					mState = isConnected;									// Set the state.
				} else {
					mPhone->out("Connection failed.");					// Troubles somewhere.
					mState = isIdle;
				}
			} else {
				ourCellManager.dumpBuff();									// Unknown what it is. Toss it.
				mPhone->out("Internal error.");							// Something weird going on..
				mState = isIdle;												// We assume hangup failed. Still connected.
			}
			graceTimer.start();												// Start the grace timer. (Status lags and can mess us up.)
			mCallingID = -1;													// This command is done.
			setNeedRefresh();													// Show whatever changed.
		break;
		case com_complete :													// We missed something?													
			mState = isIdle;														// Assume failure.
			setNeedRefresh();													// Show new state.
		case com_missing  :                                   	// Possibly timed out or we already got it.
			mPhone->out("No response.");									// Just so we know.
			mState = isIdle;													// If the FONA crashed, we're not connected..
			mCallingID = -1;													// In any case we're done with this command.
			setNeedRefresh();													// Things changed, lets see it.
		break;
	}
}


// *****************************************************
// *********************  phone   **********************
// *****************************************************

// phone object. This is the phone screen you use to dial with. It is also
// the interface used to start calls from other areas of the cell phone
// program.
phone::phone(void) 
  : cellOSPanel(phoneApp) { mRawPN = NULL; }


phone::~phone(void) { if (mRawPN) free(mRawPN); }


// Being a panel we are a draw object. actually a drawGroup object. So we
// can add viewable things to ourselves and let the user interact with them.
// Hence, phone buttons, labels etc. This is where we "build" our screen of
// things like buttons, controls & labels.
void phone::setup(void) {

  pBtndel   = new phoneBtn(KEY_C3,KEY_R5,'D',this);
  
  pBtn7     = new phoneBtn(KEY_C1,KEY_R1,'7',this);
  pBtn8     = new phoneBtn(KEY_C2,KEY_R1,'8',this);
  pBtn9     = new phoneBtn(KEY_C3,KEY_R1,'9',this);
  
  pBtn4     = new phoneBtn(KEY_C1,KEY_R2,'4',this);
  pBtn5     = new phoneBtn(KEY_C2,KEY_R2,'5',this);
  pBtn6     = new phoneBtn(KEY_C3,KEY_R2,'6',this);
  
  pBtn1     = new phoneBtn(KEY_C1,KEY_R3,'1',this);
  pBtn2     = new phoneBtn(KEY_C2,KEY_R3,'2',this);
  pBtn3     = new phoneBtn(KEY_C3,KEY_R3,'3',this);
  
  pBtn0     = new phoneBtn(KEY_C1,KEY_R4,'0',this);
  pBtnStar  = new phoneBtn(KEY_C2,KEY_R4,'*',this);
  pBtnHash  = new phoneBtn(KEY_C3,KEY_R4,'#',this);
  
  pBtnCall  = new callControl(KEY_C1,KEY_R5,'C',this);

  theEditBase = new colorRect(EB_X,EB_Y,EB_W,EB_H);
  theEditBase->setColor(&editFieldBColor);
  addObj(theEditBase);
  
  numDisplay  = new PNLabel(ET_X,ET_Y,ET_W,ET_H,1);
  numDisplay->setColors(&textSelectColor,&editFieldBColor);
  addObj(numDisplay);

  stateDisplay  = new liveText(ST_X,ST_Y,ST_W,ST_H,100);
  stateDisplay->addAColor(0,&textColor);
  stateDisplay->addAColor(2500,&textColor);
  stateDisplay->addAColor(3000,&backColor);
  stateDisplay->hold();
  stateDisplay->setColors(&textColor,&backColor);  // Sets the transp variable to false;
  addObj(stateDisplay);
}


// Seeing as we are a panel, we have a loop() call. This is used for our
// "main" focus. Typically watching for user interaction.
void phone::loop(void) { }


// For drawing ourselves. Not much to do because we are basically just a
// background for all the bits in our group. They all take care of drawing
// themselves. (As it should be.)
void phone::drawSelf(void) {

  screen->fillScreen(&backColor);
  //screen->fillRect(0,0,width,MENU_BAR_H,&menuBarColor);
}


// Fielding keystrokes. We built all the buttons and gave them keystrokes.
// This is where we respond to these as the user clicks on stuff.
void phone::keystroke(char inKey) {
	
	char outStr[2];
	
	switch(inKey) {
		case '0'   :
		case '1'   :
		case '2'   :
		case '3'   :
		case '4'   :
		case '5'   :
		case '6'   :
		case '7'   :
		case '8'   :
		case '9'   : 
		case '*'   :
		case '#'   :															// If inKey is any of the  possible keypad numbers..
		if (pBtnCall->connected()) {										// If we are connected.. (We want to make the noise!)
			outStr[0] = inKey;												// Stuff in the keystroke char.
			outStr[1] = '\0';													// End of string.
			ourCellManager.sendCommand(touchTone,outStr,false);	// Send it on its way, no reply wanted.
		} else {																	// Else, not connected..
			addChar(inKey);													// Add this keystroke to the number buffer.
			numDisplay->setValue(mRawPN);									// Show it to the user.
		}
		break;
		case 'D'   :															// Oh! Delete a char?
			deleteChar();														// Do the delete.
			numDisplay->setValue(mRawPN);									// Show the edited number string.
		break;
	}  
}


// If it was a printable character we would like to show the user that we
// saw it. And that's what we're doing here. Basically managing a string
// buffer and growing it as characters come in.
void phone::addChar(char keyStroke) {
  
  int   numChars;
  char* newStr;
  
  if (!mRawPN) {
    mRawPN = (char*)malloc(2);
    mRawPN[0] = keyStroke;
    mRawPN[1] = '\0';
  } else {
    numChars = strlen(mRawPN);
    newStr = (char*)malloc(numChars+2);
    strcpy(newStr,mRawPN);
    newStr[numChars] = keyStroke;
    newStr[numChars+1] = '\0';
    free(mRawPN);
    mRawPN = newStr;
  }
}


// Or deleting characters if they call for a delete..
void phone::deleteChar(void) {

  if (mRawPN) {
    if (mRawPN[0]!='\0') {
      mRawPN[strlen(mRawPN)-1] = '\0';
    }
  }
}


// We have a staus area for sending messages to the user. It will show the
//message for a but then slowly fade away. I think its pretty cool.
// Originally developed for Allie's doorbell. This one sends a number.
void phone::out(int message) {
  
  stateDisplay->setColors(&textColor,&backColor);
  stateDisplay->setValue(message);
  stateDisplay->release(true);
}


// And this will send a string.
void phone::out(char* message) {
  
  stateDisplay->setColors(&textColor,&backColor);
  stateDisplay->setValue(message);
  stateDisplay->release(true);
}
