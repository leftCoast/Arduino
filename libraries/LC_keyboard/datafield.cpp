#include "datafield.h"
//#include "debug.h"

// Minimum constructor. The thought is that this may be created as a global. So lets not
// make that an impossibility.
datafield::datafield(int x,int y,int w,int h)
	:drawGroup(x,y,w,h,fullClick) {
	
	mKeyboard	= NULL;
	mEditField	= NULL;
}


//We didn't actually allocate anything. So, nothing to do here.
datafield::~datafield(void) { }


// This is the call you would use in setup(), when putting your screen/panel together.
void datafield::begin(keyboard* inKeyboard,editLabel* inEditLabel,drawObj* background) {

	mKeyboard = inKeyboard;							// Save a link to the keyboard.
	if (background) addObj(background);			// If non-null, toss the background into our list.
	mEditField = inEditLabel;						// Save a link to the editor.
	mEditEvents = mEditField->getEventSet();	// Save off the edit field's event set.
	mEditField->setEventSet(noEvents);			// Shut off its events 'till we need them on.
	addObj(mEditField);								// Toss the editor into our list.
	hookup();
}
	

// We should only get a click when we arre NOT editing. Hence, on clicks, we set the focus
// pointer to us. This will fire off the editing sequence.	
void datafield::doAction(void) { setFocusPtr(this); }


// And here's the focus pointer call we need to start or end editing sequences.
void  datafield::setThisFocus(bool setLoose) {

	drawObj::setThisFocus(setLoose);
	if (mKeyboard && mEditField) {					// Sanity. Did they give us links we can use?
		if (setLoose) {									// If we get focus?
			setEventSet(noEvents);						// Mask ourselves to pass clicks to the editing field.
			mKeyboard->setEditField(mEditField);	// Swap keyboard's subject to our editing field.
			mEditField->beginEditing();				// Give our editing field a kick to get it going.
			mEditField->setEventSet(mEditEvents);	// Restore the editing field's events.
		} else {												// Ok, we are loosing focus. Either someone else is getting it, or we let it go in idle().
			mEditField->setEventSet(noEvents);		// Shut off its events again.
			if (mEditField->getEditing()) {			// If our editing session is still running..
				mEditField->handleOkKey();				// In this case the user clicked elsewhere. This is seen as "OK".
			}
			setEventSet(fullClick);						// Go back to catching clicks. So we can be restarted.						
		}
	}
}


// If the user clicks OK or cancel, we actually don't see it here. So, during idle time we
// check to see if the thing has shut down on us. If so, we quietly set the focus pointer
// to NULL, triggering everyone to do their stopping editing things. We shut off its event,
// turn on ours. Who knows what everyone else is doing?
void datafield::idle(void) {

	drawGroup::idle();							// If any of the parents need this..
	if (haveFocus()) {							// If we have focus..
		if (!mEditField->getEditing()) {		// And, if our editor is no longer editing..
			setFocusPtr(NULL);					// Editor must have terminated. So, set focus to NULL.
		}
	}
}
