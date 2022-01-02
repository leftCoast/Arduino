#include <editable.h>
#include <Arduino.h>

editable::editable(void) {

	mCurrentChar = '\0';		// Just a default.
	mEditing = false;			// We start up not editing.
	mSuccess = false;			// We've not been a success yet.
	mExitOnEnter = false;	// Its an option. You want it? Set it.	
}


editable::~editable(void) {  }


void editable::setExitOnEnter(bool trueFalse) {	mExitOnEnter = trueFalse; }


void editable::handleKeystroke(keystroke* inKey) {


	if (mEditing) {
		switch(inKey->editCommand) {
			case	shift		:
			case	number	:
			case	symbol	:	break;								// These three we can ignore.
			case	input		:											// Some sort of standard character has been typed.
				mCurrentChar = inKey->theChar;					// Store the letter..
				handleInputKey();										// Let our offspring deal with it.
			break;
			case backspace	: handleBackspaceKey();	break;	// Some of these have default
			case arrowFWD	: handleArrowFWDKey();	break;	// actions that the offspring can
			case arrowBack	: handleArrowBackKey();	break;	// overwrite.
			case enter		: handleEnterKey();		break;						
			case cancel		: handleCancelKey();		break;
			case ok			: handleOkKey();			break;
		}
	}
}


void editable::beginEditing(void) { mEditing = true; }
			
void editable::handleInputKey(void) {  }

void editable::handleBackspaceKey(void) {  }

void editable::handleArrowFWDKey(void) {  }

void editable::handleArrowBackKey(void) {  }


void editable::handleEnterKey(void) {

	if(mExitOnEnter) {
		mSuccess = true;
		endEditing();
	} else {
		handleInputKey();
	}
}


void editable::handleCancelKey(void) {

	mSuccess = false;
	endEditing();
}


void editable::handleOkKey(void) {

	mSuccess = true;
	endEditing();
}


void editable::endEditing(void) { mEditing = false; }


char editable::getCurrentChar(void) {return mCurrentChar; }


bool editable::getEditing(void) {return mEditing; }


bool editable::getSuccess(void) {return mSuccess; }


bool editable::getExitOnEnter(void) {return mExitOnEnter; }
				
				
				
