#include <modalKbd.h>


modalKbd*		theKbd	= NULL;
int				kbdUsers	= 0;



// **************************************************************
// *********************** modalKbd stuff ***********************
// **************************************************************


modalKbd::modalKbd(void)
	: bmpKeyboard(NULL,true) {
	
	ourListener = NULL;
	loadKeys();
	viewList.addObj(this);
}


modalKbd::~modalKbd(void) { }


void modalKbd::setListener(listener* inListener) { ourListener = inListener; }
	
	
void modalKbd::drawSelf(void) { screen->fillRect(this,&black); }


// This is where we trap the keyboard cancel & ok commands to be passed on to our listener
// as stdComs.
void modalKbd::handleKey(keyCommands inEditCom) {
	
	if (ourListener) {									// If we actually have a listener..
		if (inEditCom==cancel) {						// keyCommands cancel.. 
			ourListener->handleCom(cancelCmd);		// Gets passed as stdComs cancelCmd.
		} else if (inEditCom==ok) {					// keyCommands ok.. 
			ourListener->handleCom(okCmd);			// Gets passed as stdComs okCmd.
		}
	}
	bmpKeyboard::handleKey(inEditCom);				// Then we let the keyboard run it too.
}



// **************************************************************
// *********************** kbdUser  stuff ***********************
// **************************************************************


// Constructor. Make sure there IS a keyboard. Save the last modal's edit field so we can
// replace it later.	
kbdUser::kbdUser(void) {

	if (!theKbd) {											// If no keyboard..
		theKbd = new modalKbd;							// We create one.
		kbdUsers	= 0;										// And set the user count to zero.
	}															//
	if (theKbd) {											// Ok, If we have a keyboard..
		kbdUsers++;											// Bump up the user count.
		lastEditObj = theKbd->getEditField();		// Save off the last modal's edit field.
		if (lastEditObj) {								// If we got an actual edit field..
			lastEditObj->endEditing();					// Shut off its editing.
		}
	}
}


// Destructor, bump down the count, if zero, delete the keyboard. If not set the last
// modal's edit field in there.
kbdUser::~kbdUser(void) {
	
	kbdUsers--;											// Bump down the amount of keyboard users.
	if (!kbdUsers) {									// If we have no more keyboard users..
		if (theKbd) {									// If we have a keyboard..
			delete(theKbd);							// Dump the keyboard.
			theKbd = NULL;								// Flag it as gone.
		}
	} else {												// Else, there's still uers left..
		if (theKbd && lastEditObj) {				// If we have a keyboard and their last edit object.
			theKbd->setEditField(lastEditObj);	// Set their edit field back in there.
			lastEditObj->beginEditing();			// And fire its editing back up for them.
		}
	}
}


void kbdUser::setListener(listener* inListener) {

	if (theKbd) {
		theKbd->setListener(inListener);
	}
}


void kbdUser::setEditField(editLabel* inEditable) {

	editLabel*	oldEditable;
	
	if (theKbd) {
		oldEditable = (editLabel*)theKbd->getEditField();
		if (oldEditable) {
			oldEditable->endEditing();
		}
		theKbd->setEditField(inEditable);
		if (inEditable) {
			inEditable->beginEditing();
		}
	}
}
	
	
	
	