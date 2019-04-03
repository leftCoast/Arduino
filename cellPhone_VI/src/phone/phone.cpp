
#include "phone.h"
#include <cellCommon.h>
#include <drawDelete.h>
#include <resizeBuff.h>
#include <PNLabel.h>
#include "cellManager.h"
#include	"../../cellOS.h"
#include "../cellListener/cellListener.h"

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



// *****************************************************
// *******************  phCloseBtn  ********************
// *****************************************************

phCloseBtn::phCloseBtn(phone* inPhone)
	: closeBtn(inPhone) { mPhone = inPhone; }

phCloseBtn::~phCloseBtn(void) {  }


void phCloseBtn::doAction(void) {

	mPhone->startHangup();
   mPhone->mNeedClose = true;
}

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

  width = width + COL_GAP;  // All the buttons are smaller, we're "special".
  mState = isIdle;
}


callControl::~callControl(void) { }


// Not only is there a bunch of drawing code for this fancy button, we also
// mix in calls to the message text about what's going on in the world of
// cell phone hardware. This is kinda' a hack, mixing things, but it was
// such a good spot to do this. Seeing as we know the state of what's going
// on at the moment.
void callControl::drawSelf(void) {

  hookup();
  screen->setTextSize(TEXT_SIZE);
  switch(mState) {
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
      mPhone->out("Click X to refuse call.");
    break;
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


// Our actions. We have three states we can be in idle, ringning or talking.
// When we get a click, its time to change state. All three methods will
// fire off our state in a new direction.
void callControl::doAction(void) {

  switch(mState) {
    case isIdle       : mPhone->startCall();    break;
    case hasIncoming  : mPhone->answerCall(); break;
    case isConnected  : mPhone->startHangup();  break;
  }
}


// callControl manages the phone status in the background. It can initiate a
// call upon creation, via the pleasCall variable. That's somewhat of a
// special task. The rest are keeping track of the phone object state
// changes and making sure we reflect those.
void callControl::idle() {

	if (pleaseCall) {
		mPhone->numDisplay->setValue(pleaseCall->mPN);
		pleaseCall = NULL;
		doAction();
	}
  switch(mState) {
    case isIdle       : 
      if(callIncoming) {
        mState = hasIncoming;
        setNeedRefresh();
      } else if (mPhone->mConnected) {
        mState = isConnected;
        setNeedRefresh();
      }
    break;
    case hasIncoming  :
      if (mPhone->mConnected) {
        mState = isConnected;
        setNeedRefresh();
      }
    break;
    case isConnected  :
      if (!mPhone->mConnected) {
        mState = isIdle;
        setNeedRefresh();
      }
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
  : panel(phoneApp,noEvents) {

  mRawPN          = NULL;
  mCallingID      = -1;
  mConnected      = false;
  mHangupID       = -1;
  mNeedClose      = false;
}


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
  
  menuBar* ourMenuBar = new menuBar((panel*)this,false,true);
  addObj(ourMenuBar);
  
  phCloseBtn* ourCloseBtn = new phCloseBtn(this);
  ourMenuBar->addObj(ourCloseBtn);
  Serial.println("Finished setup()");Serial.flush();
}


// Seeing as we are a panel, we have a loop() call. This is used for our
// "main" focus. Typically watching for user interaction.
void phone::loop(void) {

	if (!mConnected && mNeedClose) {
  		close();
	}
	checkCall();
	checkHangup();
}


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
      startHangup();
      mNeedClose = true;
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


// Someone thinks there is a call coming in. How do they know this stuff?
// Anyway, this is where the command to "Pick up the damn phone!" Is
// generated.
void phone::answerCall(void) {

  if (!mConnected) {
    out("Answering..");
    mCallingID = ourCellManager.sendCommand(pickUp,true);
  }
}


// Starting a call sequence. Starting a call can take a LONG time in
// computer land. So, we'll just fire off the command, and deal with getting
// things hooked up, or not, during loop() time.
void phone::startCall(void) {

	char*	numBuff;
	
	numBuff = NULL;																			// Ok, init numBuff for later.
	if (!mConnected) {																		// If we're not connected..
		if (	statusReg.networkStat==NS_registeredHome||
				statusReg.networkStat==NS_registeredRoaming ) {
			if (resizeBuff(numDisplay->getNumChars()+1,(uint8_t**)&numBuff)) {	// If we can allocate the text buffer..
				numDisplay->getText(numBuff);													// Grab the number off the display.
				filterPNStr(numBuff);															// Filter the formatting out of it.
				mCallingID = ourCellManager.sendCommand(makeCall,numBuff,true);	// Send the call command.
				out(numBuff);
				resizeBuff(0,(uint8_t**)&numBuff);											// Recycle numBuff.
				out("Calling..");																// Show the user we're doing it.
			} else {																					// Or else we didn't get the RAAM?
				out("Memory error?!");															// Show the user we ran out of memory.
			}
		} else {
			out("No network.");
		}
	}																								// Already connected? Just ignore everything.
}


// Just like starting a call, doing the hangup sequence can take a long
// time. So, we just fire off the command and let the loop() code take care
// of managing it.
void phone::startHangup(void) {

  if (mConnected && mHangupID==-1) {								// If we are connected and not already trying to hang up.
    out("Hanging up..");												// Show were going to try hanging up.
    mHangupID = ourCellManager.sendCommand(hangUp,true);		// Create the command and send it on its way.
  }
}


// loop() wants us to check the status of our makeCall command. It may be
// still working on making the call, it could have failed, possibly timed
// out? Whatever, we deal with it and take action appropriately here.
void phone::checkCall(void) {

	int           numBytes;
	byte          reply;
  
	if (mCallingID!=-1) {														// Working on a call?
		if (ourCellManager.progress(mCallingID)==com_holding) {		// We found it and its holding a reply?
			numBytes = ourCellManager.numReplyBytes(mCallingID);		// This is how big the reply is.
			if (numBytes==1) {													// We're looking for a one byte reply.
				ourCellManager.readReply(mCallingID,&reply);				// Grab the byte.
				if (reply==0) {													// Is it a zero?
					mConnected = true;											// Cool!, we're connected!
					out("Connected.");											// Show that the call's going through.
				} else {
					out("Connect error.");										// FONA passed back an error.
				}
			} else {																	// Internal communication error.
				ourCellManager.dumpBuff();                            // Unknown what it is. Toss it.
				out("Internal error.");
			}
			mCallingID = -1;                                        // In any case, we're done with the command.
		} else if (ourCellManager.progress(mCallingID)==com_missing) {
			out("No response.");
			mCallingID = -1;
		}
	}
}


// loop() wants us to check the status of our hangUp command. Basically
// this is the same kind of method as checkCall(). Just checking a different
// command with a little different style of coding.
void phone::checkHangup(void) {

  int   numBytes;
  byte  reply;
  
  if (mHangupID!=-1) {                                      // Working on a call?
    switch (ourCellManager.progress(mHangupID)){            // See what's going on here..
      case com_standby  : 
      case com_working  :  break;                           // Nothing we can do here.
      case com_holding  :                                   // Holding means there's a reply waiting for us.
        numBytes = ourCellManager.numReplyBytes(mHangupID); // This is how big the reply is.
        if (numBytes==1) {                                  // We're looking for a one byte reply.
          ourCellManager.readReply(mHangupID,&reply);       // Grab the byte.
          if (reply==0) {                                   // Is it a zero?
            mConnected = false;                             // Cool!, we're disconnected!
            out("Disconnectd.");                            // Tell the world!
          } else {
            out("Hangup error.");                           // Troubles somewhere.
          }
        } else {
          ourCellManager.dumpBuff();                        // Unknown what it is. Toss it.
          out("Reply error.");                              // Something weird going on..
        }
        mHangupID = -1;													// This command is done. Shouldn't we note that?
      break;
      case com_complete :												// All this means is it gave us back a reply. (Any reply)
      	out("A reply arrived.");
      break;                             
      case com_missing  :                                   // Possibly timed out or we already got it.
      	out("No response.");
        	mConnected = false;											// If the FONA crashed, we're not connected anymore..
        	mHangupID = -1;
      break;
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
