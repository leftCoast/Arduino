#include <editLabel.h>
#include <resizeBuff.h>
#include <mapper.h>

//#include <debug.h>

editLabel::editLabel()
	: label(),
	timeObj(CURSOR_BLINK) { init(); }


editLabel::editLabel(const char* inText)
	: label(inText),
	timeObj(CURSOR_BLINK) { init(); }


editLabel::editLabel(const char* inText, int inSize)
	: label(inText,inSize),
	timeObj(CURSOR_BLINK) { init(); }


editLabel::editLabel(int inLocX, int inLocY, int inWidth,int inHeight)
	: label(inLocX,inLocY,inWidth,inHeight),
	timeObj(CURSOR_BLINK) { init(); }


editLabel::editLabel(int inLocX, int inLocY, int inWidth,int inHeight,const char* inText)
	: label(inLocX,inLocY,inWidth,inHeight,inText),
	timeObj(CURSOR_BLINK) { init(); }


editLabel::editLabel(int inLocX, int inLocY, int inWidth,int inHeight,const char* inText,int textSize)
	: label(inLocX,inLocY,inWidth,inHeight,inText,textSize),
	timeObj(CURSOR_BLINK) { init(); }


editLabel::editLabel(rect* inRect,const char* inText,int textSize)
	: label(inRect,inText,textSize),
	timeObj(CURSOR_BLINK) { init(); }


editLabel::editLabel(label* aLabel)
	: label(aLabel),
	timeObj(CURSOR_BLINK) { init(); }


editLabel::~editLabel(void) { 
	
	resizeBuff(0,&editBuff);
	resizeBuff(0,&backupBuff);
	resizeBuff(0,&tempBuff);
}


void editLabel::init(void) { 
	
	editBuff		= NULL;	// Buffers must all start at NULL.
	backupBuff	= NULL;
	tempBuff		= NULL;
	start();					// Get the cursor blink clock running.
}


void editLabel::beginEditing(void) { 

	int numBytes;

	hookup();												// Code's running, good time to call this.
	numBytes = strlen(buff)+1;							// Lets see what we have to start with.
	if (resizeBuff(numBytes,&backupBuff)) {		// We'll see if we can allocate enough to work with.
		if (resizeBuff(numBytes,&editBuff)) {
			if (resizeBuff(numBytes,&tempBuff)) {
				resizeBuff(0,&tempBuff);				// Good, we can recycle this one for now.
				strcpy(backupBuff,buff);				// Get a copy in case things go wrong.
				strcpy(editBuff,buff);					// Get a copy to edit.
				editable::beginEditing();				// All set, let the parents fire up editing.
				setInitalPointers();						// Setup initial edit index, cursor, firstChar.
				cursorOnOff = true;						// Lest see that cursor right off!
				showText();									// Now that we are "editing" lets show it.
			}
		}
	}
}


void editLabel::endEditing(void) { 
	
	editable::endEditing();			// Shut off the editor.
	resizeBuff(0,&tempBuff);		// Done with this, recycle; (Should already be NULL anyway.)
	if (mSuccess) {					// If editing was a success..
		setValue(editBuff);			// We give back the entire edited string.
	} else {								// Else we want to go back..
		setValue(backupBuff);		// We reset the displayed text to the original.
	}
	resizeBuff(0,&editBuff);		// Recycle the edit buffer.
	resizeBuff(0,&backupBuff);		// And finally, recycle the backup buffer.
}


void editLabel::setIndex(int newIndex) { 

	int numChars;
	
	numChars = strlen(editBuff);		// Count the characters once.
	if (newIndex>numChars) {			// If they ask for an index beyond the string..
		index = numChars;					// They get the index just at the end.
	} else if (newIndex<0) {			// Else if they ask for an index before the start of the string..
		newIndex = 0;						// The get the beginning index.
	} else {									// Else, the new size is within bounds.
		index = newIndex;					// We update the index with what they asked for.
	}
	showText();								// Using this new index, display it appropriately.
}


int editLabel::getIndex(void) { return index; }			// Just return it. (I wonder who'd want to see it?)
	

void editLabel::handleInputKey(void) { 

	int numBytes;
	
	numBytes = strlen(editBuff)+2;						// The plan is to add a character.
	if (resizeBuff(numBytes,&tempBuff)) {				// Grab some temp memory.
		memcpy(tempBuff,editBuff,index);					// Store the first block of characters to temp memory.
		tempBuff[index] = mCurrentChar;					// Add the new character.
		strcpy(&tempBuff[index+1],&editBuff[index]);	// Now add in the second have of the string to temp memory.
		if (resizeBuff(numBytes,&editBuff)) {			// Reallocate the edit buffer to fit the new string.
			strcpy(editBuff,tempBuff);						// Copy the new string beck into the edit buffer.
			setIndex(index+1);								// Set the index up one.
		}
		resizeBuff(0,&tempBuff);							// Recycle the temp memory.
		showText();												// Lest see the result.
	}
}


void editLabel::handleBackspaceKey(void) { 
	
	int numBytes;
	
	if (index) {														// You cant backspace at the beginning of the string.
		numBytes = strlen(editBuff)+1;							// Get the string length (including '\0').
		memcpy(&editBuff[index-1],&editBuff[index],numBytes-index);	// Plunk the tail over the deleted character.
		setIndex(index-1);											// Set the index down one.
		showText();
	}
}


void editLabel::handleArrowFWDKey(void) { 

	setIndex(index+1);
	showText();
}


void editLabel::handleArrowBackKey(void)  { 

	setIndex(index-1);
	showText();
}
	
	
void editLabel::drawSelf(void) { 

	int cursX;
	int cursY;
	int cursH;
	
	
	if (mEditing) {																// But IF we're editing..
		cursH = getTextHeight();												// Figure out where the cursor is.
		screen->drawVLine(x-1,y,cursH,&backColor);							// Just in case its there. Normal drawing can't erase the cursor.
		label::drawSelf();														// Draw like we normally do.
		cursX = x + CHAR_WIDTH*textSize*cursor - 1;						// We put a LOT of effort into locating the cursor. Use it now.
		cursY = y;																	// Always here.
		if(cursorOnOff) {															// If its on..
			screen->drawVLine(cursX,cursY,cursH,&textColor);			// We draw it in.
		} else {																		// Else..
			screen->drawVLine(cursX,cursY,cursH,&backColor);			// We erase it.
		}
	} else {																			// Else, not editing..
		label::drawSelf();														// Draw like we normally do.
	}
}


void editLabel::doAction(event* inEvent,point* locaPt) { 
	
	int		touchLoc;
	int		deltaX;

	mapper	touchMapper(0,width,0,getViewChars()); 
	
	if (mEditing) {
		touchLoc = round(touchMapper.map(locaPt->x));
		deltaX = touchLoc - cursor;
		setIndex(index + deltaX-1);
	}
}


// Just starting to edit. set up initial conditions.
void editLabel::setInitalPointers(void) { 
	
	int numChars;
	int viewChars;
	
	numChars = strlen(editBuff);						// editBuff is our actual string now.
	viewChars = getViewChars();						// How many can we display?	
	if(numChars<=viewChars) {							// This is the case where the string fits the view width.
		switch(justify) {									// What is best for each case?
			case TEXT_RIGHT	:							// *** RIGHT
				index = numChars;							// Set the edit point to the (right) end of the text.
				cursor = viewChars;						// This sets the cursor to the end of the view display.
			break;
			case TEXT_LEFT		:							// *** LEFT
				index = numChars;							// Again set the edit index to the end of the text.
				cursor = index;							// And now both cursor and edit index count from the same point. 								
			break;
			case TEXT_CENTER	:							// *** Center
				index = numChars/2;						// Typically these start at the middle of the text.
				cursor = viewChars/2;					// And the cursor should track the index like in left justify.
			break;
		}
		if (numChars==viewChars) {						// Does the string exactly fit the view width?																
			firstChar = 0;									// firstChar is zero. Our editing "box" is set to the start of the string.
		} else {
			firstChar = -1;								// firstChar is set to -1 as a flag that its un-set. No editing box.
		}
	} else {													// Else we have more string than we can display,,
		switch(justify) {									// Again, what is best for each case?													
			case TEXT_RIGHT	:							// *** RIGHT
				index = numChars;							// Sme as when the string actually fits. 
				cursor = viewChars;						// Also the same, end of view.
				firstChar = numChars - viewChars;	// Our edit window starts viewChars back from the end.
			break;
			case TEXT_LEFT		:							// *** LEFT
				index = viewChars;						// We set it to what we can see. Is this ok?
				cursor = index;							// cursor follows.
				firstChar = 0;								// Edit box is sitting at start of string, it what we can see.							
			break;
			case TEXT_CENTER	:							// *** Center	
				index = numChars/2;						// We'll start at the middle of the text you can see.
				cursor = viewChars/2;					// And the cursor should start at the same point (once scrolled);
				firstChar = index - viewChars/2;		// Edit box start at index minus half the editing width.
			break;
		}
	}
}

			
// Display it appropriately. Oh boy..
void editLabel::showText(void) { 

	int numChars;
	int viewChars;
	bool	done;
	
	numChars = strlen(editBuff);																	// editBuff is our actual string now.
	viewChars = getViewChars();																	// How many can we display?
	if(numChars<=viewChars) {																		// Basically, if the string fits.
		switch(justify) {																				// We do the work to locate the cursor.
			case TEXT_RIGHT	: cursor = viewChars - numChars + index;		break;	// For right justify, view width - string length plus our edit index.
			case TEXT_LEFT		: cursor = index; 									break;	// For left justify, always the index itself.
			case TEXT_CENTER	: cursor = index + (viewChars-numChars)/2;	break;	// For center justify, kinda' like 1/2 text right.
		}
		if (numChars==viewChars) {																	// Does the string exactly fit the view width?																
			firstChar = 0;																				// firstChar is zero. Our editing "box" is set to the start of the string.
		} else {
			firstChar = -1;																			// firstChar is set to -1 as a flag that its un-set.
		}
		setValue(editBuff);																			// Our cursor is located, the string fits, let the parents take over.
	} else {
		done = false;																					// We need to scroll the edit box 'till the cursor shows.
		while(!done) {																					// We should only need one click. But.. Unforeseen forces a loop.
			if (index-firstChar>viewChars) firstChar++;										// The cursor is beyond the edit box, scroll right.
			else if (index<firstChar) firstChar--;												// The cursor is before the edit box, scroll left.
			else done = true;																			// Not off either end? We're done!
		}
		if (resizeBuff(viewChars+1,&tempBuff)) {												// Now we clip out a string segment for the edit box.
			tempBuff[0]='\0';																			// "Clear" the temp buffer.
			strncat(tempBuff,&editBuff[firstChar],viewChars);								// Stuff in just what we should see.
			cursor = index - firstChar;															// Set the cursor.
			setValue(tempBuff);																		// Show them the scrolled text.
			resizeBuff(0,&tempBuff);																// Recycle, reuse.
		}
	}
}


// We use the idle() method to flash the cursor.
void editLabel::idle(void) {

	if (mEditing) {
		if(ding()) {                              // Uggh! Get to work!
			cursorOnOff = !cursorOnOff;
			needRefresh = true;
			start();
		}
	}
}
		