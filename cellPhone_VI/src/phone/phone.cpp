
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
#define KEY_H       30

#define ROW_GAP     50

#define KEY_R0      30
#define KEY_R1      KEY_R0 + ROW_GAP
#define KEY_R2      KEY_R1 + ROW_GAP
#define KEY_R3      KEY_R2 + ROW_GAP
#define KEY_R4      KEY_R3 + ROW_GAP
#define KEY_R5      KEY_R4 + ROW_GAP

#define COL_GAP     70

#define KEY_C1      30
#define KEY_C2      KEY_C1 + COL_GAP
#define KEY_C3      KEY_C2 + COL_GAP

#define EB_X    KEY_C1
#define EB_Y    KEY_R0
#define EB_W    KEY_W+COL_GAP+20
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
byte	byteBuff;		// Need an address for a byte? Here ya' go.

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


// Its amazing how much code it takes just to "draw" a button. Is it active?
// Clicked? Inactive? Is it a character? A symbol? Its all here for all our
// standard phone buttons.
void phoneBtn::drawSelf(void) {

  int         inset = 5;                      // All the hand tweaking to
  int         arrowX1 = x+inset+3;            // setup the delete key arrow.
  int         arrowY1 = y+inset-1;            // After all that, Julie insists it
  int         arrowX2 = width-(2*(inset+3));  // Looks like an empty Batt. icon.
  int         arrowY2 = height-(2*inset);
  rect        arrowRect(arrowX1,arrowY1,arrowX2,arrowY2);
  drawDelete  arrow(&arrowRect);

    
  if (mKeystroke[0]=='X') {
    if (clicked) {
      screen->fillRoundRect(x+1,y,width,height,KEY_RAD,&backColor);
      screen->fillRoundRect(x,y,width,height,KEY_RAD,&black);
      screen->setTextColor(&redButtonColor);
    } else {
      screen->fillRoundRect(x+1,y,width,height,KEY_RAD,&redButtonHighlight);
      screen->fillRoundRect(x,y,width,height,KEY_RAD,&redButtonColor);
      screen->setTextColor(&black);
    }
  } else {
    if (clicked) {
      screen->fillRoundRect(x+1,y,width,height,KEY_RAD,&lightbButtonColorHit);
      screen->setTextColor(&white);
      arrow.setColors(&redButtonColor,&lightbButtonColorHit);
    } else {
      screen->fillRoundRect(x+1,y,width,height,KEY_RAD,&lightbButtonColorHit);  // Gives a cool highlight.
      screen->fillRoundRect(x,y,width,height,KEY_RAD,&lightbButtonColor);
      screen->setTextColor(&textSelectColor);
      arrow.setColors(&redButtonColor,&lightbButtonColor);
    }
  }
  if (mKeystroke[0]=='D') {
    arrow.stamp();
  } else {
    screen->setTextSize(TEXT_SIZE);
    screen->setCursor(x+(KEY_W-CHAR_WIDTH)/2, y + ((height-TEXT_HEIGHT)/2));
    screen->drawText(mKeystroke);
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
	mSweepID				= -1;
	mOurCallerID[0]	= '\0';
	mState				= wakeUp;
	graceTimer.setTime(GRACE_TIME);
}


// We're going down, blindly send out callerID on as well as a hangup command.
callControl::~callControl(void) {

	byteBuff = 1;
	ourCellManager.sendCommand(callerID,1,&byteBuff,false);	// Make sure we leave this turned on.
	mHangupID = ourCellManager.sendCommand(hangUp,false); 	// And make sure we hang up the phone .
}


// Not only is there a bunch of drawing code for this fancy button, we also
// mix in calls to the message text about what's going on in the world of
// cell phone hardware.
void callControl::drawSelf(void) {

	contact*	aContact;
	
	hookup();
	screen->setTextSize(TEXT_SIZE);
	switch(mState) {
		case wakeUp			:
		case isIdle       :
			if (clicked) {
				screen->fillRoundRect(x+1,y,width,height,KEY_RAD,&backColor);
				screen->fillRoundRect(x,y,width,height,KEY_RAD,&lightbButtonColor);
				screen->setTextColor(&green);
			} else {
				screen->fillRoundRect(x+1,y,width,height,KEY_RAD,&greenButtonHighlight);
				screen->fillRoundRect(x,y,width,height,KEY_RAD,&greenbuttonColor);
				screen->setTextColor(&white);
			}
			screen->setCursor(x+(KEY_W-CHAR_WIDTH)/2, y + ((height-TEXT_HEIGHT)/2));
			screen->drawText("Call");
		break;
		case hasIncoming  :
			if (clicked) {
				screen->fillRoundRect(x,y,width,height,KEY_RAD,&black);
				screen->setTextColor(&green);
			} else {
				screen->fillRoundRect(x+1,y,width,height,KEY_RAD,&greenButtonHighlight);
				screen->fillRoundRect(x,y,width,height,KEY_RAD,&greenbuttonColor);
				screen->setTextColor(&white);
			}
			screen->setCursor(x+(KEY_W-CHAR_WIDTH)/2, y + ((height-TEXT_HEIGHT)/2));
			screen->drawText("Answer");
			if (mOurCallerID[0]=='\0') {
				mPhone->out("Unknown caller.");
			} else {
				aContact = ourBlackBook->findByPN(mOurCallerID);
				if (aContact) {
					mPhone->out(aContact->mNickName);
				} else {
					 PNLabel formatter(1,1,1,1,1);
					 char	buff[30];
					 formatter.setValue(mOurCallerID);
					 mPhone->out(formatter.mFormattedPN);
				}
			}
		break;
		case connecting	:
			screen->fillRoundRect(x+1,y,width,height,KEY_RAD,&greenButtonHighlight);
			screen->fillRoundRect(x,y,width,height,KEY_RAD,&greenbuttonColor);
			screen->setTextColor(&white);
			screen->setCursor(x+(KEY_W-CHAR_WIDTH)/2, y + ((height-TEXT_HEIGHT)/2));
			screen->drawText("...");
		break;
		case hangingUp		:
			screen->fillRoundRect(x+1,y,width,height,KEY_RAD,&redButtonHighlight);
			screen->fillRoundRect(x,y,width,height,KEY_RAD,&redButtonColor);
			screen->setTextColor(&white);
			screen->setCursor(x+(KEY_W-CHAR_WIDTH)/2, y + ((height-TEXT_HEIGHT)/2));
			screen->drawText("...");
		case isConnected  :
			if (clicked) {
				screen->fillRoundRect(x,y,width,height,KEY_RAD,&backColor);
				screen->fillRoundRect(x,y,width,height,KEY_RAD,&black);
				screen->setTextColor(&redButtonColor);
			} else {
				screen->fillRoundRect(x+1,y,width,height,KEY_RAD,&redButtonHighlight);
				screen->fillRoundRect(x,y,width,height,KEY_RAD,&redButtonColor);
				screen->setTextColor(&white);
			}
			screen->setCursor(x+(KEY_W-CHAR_WIDTH)/2, y + ((height-TEXT_HEIGHT)/2));
			screen->drawText("Hangup");
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
		case readCallerID	: break;																			// We do nothing in the intermediate states.
		case connecting	: break;											
		case hangingUp		: break;
		case isConnected	:
			mHangupID = ourCellManager.sendCommand(hangUp,true);									// Create the command and send it on its way. 
    		mPhone->out("Disconnecting..");																// Show were going to try hanging up.
    		mState = hangingUp;
		break;
  }
}


bool callControl::haveService(void) {

	return statusReg.networkStat==NS_registeredHome || statusReg.networkStat==NS_registeredRoaming;
}


// callControl also manages the phone status in the background. It can initiate a
// call upon creation, via the pleasCall variable. That's somewhat of a
// special task. The rest are keeping track of the phone object state
// changes and making sure we reflect those.
void callControl::idle() {
	
	int   numBytes;
	byte  reply;
   
  	//mPhone->out(statusReg.callStat);
	switch(mState) {
		case wakeUp 			:
			mState = isIdle;												// No matter what, wakeUp is done. We want isIdle now.									
			if (pleaseCall) {												// We've been woken up to make this call.
				mPhone->numDisplay->setValue(pleaseCall->mPN);	// Grab the number off the saved contact.
				pleaseCall = NULL;										// Clear out the saved contact ptr.
				doAction();													// Since we are now in "isIdle" we call doAction to connect.
				setNeedRefresh();											// Show it.
			}
		break;
		case isIdle       	:															// Waiting to see what happens.
			if (graceTimer.ding()) {													// Wait a bit for the state to reach us "through channels".
				if(statusReg.callStat==CS_ringingIn) {								// If we have a connection ringing in..
					mOurCallerID[0] = '\0';												// Clear this for the new call.
					mSweepID = ourCellManager.sendCommand(sweepUNS,true);		// Send the sweep buffer. (look for caller ID)
					mState = readCallerID;												// Set the state to deal with it.
				}
			}
		break;
		case hasIncoming  	:
			if (statusReg.callStat==CS_ready) {										// Not connected but no longer ringing..
				byteBuff = 1;
				ourCellManager.sendCommand(callerID,1,&byteBuff,false);		// In any case, turn this back on.
				mState = isIdle;															// I guess they gave up, back to isIdle.
				setNeedRefresh();															// As always, show it.
			}
		break;
		case	readCallerID	:											// We're looking for a caller ID.
			checkSweep();													// We do the checking here.
		break;
		case	connecting		:											// A connection attempt is in process.
			checkCall();													// We do the checking inhere.
		break;
		case	hangingUp		:											// Trying to hang up the phone.
			checkHangup();													// Again, we check the progress in here.
		break;
		case isConnected  	:															// We're connected..
			if (graceTimer.ding()) {													// Wait a bit for the state to reach us "through channels".
				if (statusReg.callStat==CS_ready) {									// If CS_ready means we lost connection.
					byteBuff = 1;															// Set addressable byte to 1.
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
			byteBuff = 1;
			ourCellManager.sendCommand(callerID,1,&byteBuff,false);	// In any case, turn this back on.
			graceTimer.start();												// Start the grace timer. (Status lags and can mess us up.)
			mHangupID = -1;													// This command is done.
			setNeedRefresh();													// Show whatever changed.
		break;
		case com_complete :													// We missed something?													
			mState = isConnected;												// Assume failure.
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
			byteBuff = 1;
			ourCellManager.sendCommand(callerID,1,&byteBuff,false);	// In any case, turn this back on.
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


// We want to strip everything but the caller ID out of rawID. If we are
// successful, we'll leave a '\0' at its end for you. We'll either successfully
// do this, or just shred rawID trying.
bool callControl::filterCallerID(char* rawID,byte numChars) {
	
	char* outBuff;
	byte	trace;
	byte	index;
	bool	success;
	
	Serial.print("numChars : ");Serial.println(numChars);
	Serial.print("[");
	for(byte i=0;i<numChars;i++) {Serial.print((byte)(rawID[i]));Serial.print(",");}
	//Serial.print(rawID);
	Serial.println("]");
	
	success = false;														// init success.
	if (numChars>2) {														// It has to be more that two to be anything.
		outBuff = NULL;													// init the outBuff.
		if (resizeBuff(numChars,(uint8_t**)&outBuff)) {			// Stretch out the outBuff to fit our raw string.
			memcpy(outBuff,rawID,numChars);							// Dump the raw buff into the outBuff.
			index = 0;														// init index.
			trace = 1;														// init trace. Ignore first byte, its length.
			while(outBuff[trace]!='"'&&trace<numChars) {			// find first ".
				trace++;
			}														
			if (trace<numChars) {										// If we didn't run out, were pointing at the first ".
				trace++;														// Point at first char past the first ".
				while(outBuff[trace]!='"'&&trace<numChars) {		// Find the next ".
					rawID[index]=outBuff[trace];						// As we go along, start refilling the rawID with the chars.
					trace++;													// bump.
					index++;													// bump.
				}
				if (trace<numChars) {									// If we didn't run out, were pointing at the second ".
					rawID[index]='\0';									// End the string here.
					filterPNStr(rawID);									// Drop it into our filter function to strip out junk.
					success = strlen(rawID)>0;							// If we have anything left, we call it good.
				}
			}
			resizeBuff(0,(uint8_t**)&outBuff);						// Recycle the buffer.
		}
	}
	return success;														// Return the result.
}

 
// idle() wants us to check the status of our get called ID command. It may be
// still working on making the call, it could have failed, possibly timed
// out? Whatever, we deal with it and take action appropriately here.
void callControl::checkSweep(void) {

	int   numBytes;
	uint8_t	reply[22];

	switch (ourCellManager.progress(mSweepID)){						// See what's going on here..
		case com_standby  : 
		case com_working  :  break;										// Nothing we can do here.
		case com_holding  :													// Holding means there's a reply waiting for us.
			numBytes = ourCellManager.numReplyBytes(mSweepID);		// This is how big the reply is.
			Serial.print("Holding : ");Serial.print(numBytes);
			ourCellManager.readReply(mCallingID,reply);				// Grab the data.
			Serial.print(" [");Serial.print((char*)reply);Serial.println("]");
			if (numBytes>11) {												// We got something?												
				if (filterCallerID((char*)reply,numBytes)) {			// Attempt to isolate a caller ID from the raw data.
					strcpy(mOurCallerID,(char*)reply);					// If we got something, save it.
				}
			}
			mSweepID = -1;														// This command is done.
			mState = hasIncoming;											// Set the state.
			setNeedRefresh();													// Show whatever changed.
		break;
		case com_complete :													// We missed something?													
			mState = isIdle;													// Assume failure.
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
  : panel(phoneApp,noEvents) { mRawPN = NULL; }


phone::~phone(void) { if (mRawPN) free(mRawPN); }


// Being a panel we are a draw object. actually a drawGroup object. So we
// can add viewable things to ourselves and let the user interact with them.
// Hence, phone buttons, labels etc. This is where we "build" our screen of
// things like buttons, controls & labels.
void phone::setup(void) {

  pBtndel   = new phoneBtn(KEY_C3,KEY_R0,'D',this);
  
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
  
  menuBar* ourMenuBar = new menuBar((panel*)this,true,true);
  addObj(ourMenuBar);

}


// Seeing as we are a panel, we have a loop() call. This is used for our
// "main" focus. Typically watching for user interaction.
void phone::loop(void) { }


// For drawing ourselves. Not much to do because we are basically just a
// background for all the bits in our group. They all take care of drawing
// themselves. (As it should be.)
void phone::drawSelf(void) {

  screen->fillScreen(&backColor);
  screen->fillRect(0,0,width,MENU_BAR_H,&menuBarColor);
}


// Fielding keystrokes. We built all the buttons and gave them keystrokes.
// This is where we respond to these as the user clicks on stuff.
void phone::keystroke(char inKey) {

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
    case '#'   :
      addChar(inKey);
      numDisplay->setValue(mRawPN);
    break;
    case 'D'   :
      deleteChar();
      numDisplay->setValue(mRawPN);
    break;
    case 'X'   : 
      // This case no longer exists.
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
