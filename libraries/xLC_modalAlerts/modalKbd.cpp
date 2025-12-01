#include <modalKbd.h>


modalKbd*		theKbd	= NULL;
int				kbdUsers	= 0;



// **************************************************************
// *********************** modalKbd stuff ***********************
// **************************************************************


// Our modal keyboard constructor..
modalKbd::modalKbd(void)
	: bmpKeyboard(NULL,true) {		// Make me a modal with no editing field.
	
	ourListener = NULL;				// No listener.
	loadKeys();							// Load the keys. (Why not in the .bmp constructor?)
	viewList.addObj(this);			// And we add ourselves to the root view list.
}


// Nothing really to do for the destructor.
modalKbd::~modalKbd(void) { }


// Set our listener. The guy that wants to hear commands like okCmd.
void modalKbd::setListener(listener* inListener) { ourListener = inListener; }
	
	
// Set our listener. The guy that wants to hear commands like okCmd.
listener* modalKbd::getListener(void) { return ourListener; }

	
//	Basically what we really need is to lay down a black background for the keys.
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
// replace it later.	Save off who our last Listener was so we can reset that as well.
kbdUser::kbdUser(listener* inListener) {

	lastEditObj		= NULL;												// Nice to start off as a NULL.
	lastListener	= NULL;												// This one too.
	if (!theKbd) {															// If no keyboard..
		theKbd = new modalKbd;											// We create one.
		kbdUsers	= 0;														// And set the user count to zero.
	}																			//
	if (theKbd) {															// Ok. If we have, or just created, a keyboard..
		kbdUsers++;															// Bump up the user count.
		if (kbdUsers>1) {													// If there was a user before us..
			lastEditObj = (editLabel*)theKbd->getEditField();	// Save off the last modal's edit field.
			lastListener = theKbd->getListener();					// Save off a pointer to the last Listener.
		}
		theKbd->setListener(inListener);								// And now set to the new listener.
	}
}


// Destructor, bump down the count, if zero, delete the keyboard. If not set the last
// modal's edit field in there. And the last listenener as listenener.
kbdUser::~kbdUser(void) {
	
	kbdUsers--;											// Bump down the amount of keyboard users.
	if (!kbdUsers) {									// If we have no more keyboard users..
		if (theKbd) {									// If we have a keyboard..
			delete(theKbd);							// Dump the keyboard.
			theKbd = NULL;								// Flag it as gone.
		}
	} else {												// Else, there's still uers left..
		if (theKbd) {									// If we have a keyboard and their last edit object.
			theKbd->setListener(lastListener);	// Put back the old listener.
			theKbd->setEditField(lastEditObj);	// Set their edit field back in there.
			if (lastEditObj) {						// If we really did have an edit object..
				lastEditObj->beginEditing();		// And fire its editing back up for them.
			}
		}
	}
}


// This is just for switching edit fields. We don't save the last one here.
void kbdUser::setEditField(editable* inEditable) {

	editable*	oldEditable;
	
	if (theKbd) {									// If we have a keyboard..
		oldEditable = theKbd->getEditField();
		if (oldEditable) {
			oldEditable->mSuccess = true;
			oldEditable->endEditing();
		}
		theKbd->setEditField(inEditable);	// Set the new edit field.
		if (inEditable) {							// If there IS a new edit field..
			inEditable->beginEditing();		// Fire up editing.
		}
	}
}


void kbdUser::checkKbdEvent(event* inEvent,point* globalPt) {

	if (theKbd) {
		theKbd->acceptEvent(inEvent,globalPt);
	}
}

	