#include "editable.h"

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
				mCurrentChar = inKey->theChar;						// Store the letter..
				handleInputKey();										// Let our offspring deal with it.
			break;
			case backspace	: handleBackspaceKey();	break;	// These three? Let our offspring deal with them.
			case arrowFWD	: handleArrowFWDKey();	break;
			case arrowBack	: handleArrowBackKey();	break;
			case enter		: 											// These next three can be exit keys.
				mCurrentChar = inKey->theChar;						// Store the letter..
				if (mExitOnEnter) {									// If we're exiting on enter..
					mSuccess = true;									// And we call enter-exits a success.
					endEditing();										// All done.
				}
				handleEnterKey();										// And, let our offspring deal with it.
			break;
			case cancel		: endEditing();			break;	// Defiantly an exit key.
			case ok			:											// Also defiantly an exit key.
				mSuccess = true;										// But OK keys are being called successes.
				endEditing();											// Wrap up editing session.
			break;
		}
	}
}


void editable::beginEditing(void) { mEditing = true; }
			
void editable::handleInputKey(void) {  }

void editable::handleBackspaceKey(void) {  }

void editable::handleArrowFWDKey(void) {  }

void editable::handleArrowBackKey(void) {  }

void editable::handleEnterKey(void) {  }

void editable::handleCancelKey(void) {  }

void editable::handleOkKey(void) {  }

void editable::endEditing(void) { mEditing = false; }


