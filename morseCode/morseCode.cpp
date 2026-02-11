#include <morseCode.h>



letter::letter(void) {

	callback = NULL;
	ourState	= waiting;
}


letter::~letter(void) { }


// When the key needs to go up or down, we'll call this function.
void letter::setCallback(void (*funct)(bool)) {

    callback = funct;
    hookup();
}


// Drop a letter in here and it should automatically do the morse code of that letter.
// How ever you choose, by coding the setKey() routine.
void letter::setLetter(char aLetter) {
	 
	 ourLetter   = toupper(aLetter);
    strokeIndex = 0;
    ourState    = starting;
}


// True means pushing the key down. False is to let it up.
void letter::setKey(bool down) {

	if (callback) {		// If we have a callback..
   	callback(down);	// call it.
   }
}


// Are we done and ready for the next letter?
bool letter::letterDone(void) { return ourState == waiting; }


// This is just a lot of typing.. Choose the four strokes we want for the letter we got.
void letter::setupStrokes(void) {

	stroke[0] = nothing;
	stroke[1] = nothing;
	stroke[2] = nothing;
	stroke[3] = nothing;
	stroke[4] = nothing;
	switch (ourLetter) {
		case 'A'  :
			stroke[0] = aDot;
			stroke[1] = aDash;
		break;
		case 'B'  :
			stroke[0] = aDash;
			stroke[1] = aDot;
			stroke[2] = aDot;
			stroke[3] = aDot;
		break;
		case 'C'  :
			stroke[0] = aDash;
			stroke[1] = aDot;
			stroke[2] = aDash;
			stroke[3] = aDot;
		break;
		case 'D'  :
			stroke[0] = aDash;
			stroke[1] = aDot;
			stroke[2] = aDot;
		break;
		case 'E'  :
			stroke[0] = aDot;
		break;
		case 'F'  :
			stroke[0] = aDot;
			stroke[1] = aDot;
			stroke[2] = aDash;
			stroke[3] = aDot;
		break;
		case 'G'  :
			stroke[0] = aDash;
			stroke[1] = aDash;
			stroke[2] = aDot;
		break;
		case 'H'  :
			stroke[0] = aDot;
			stroke[1] = aDot;
			stroke[2] = aDot;
			stroke[3] = aDot;
		break;
		case 'I'  :
			stroke[0] = aDot;
			stroke[1] = aDot;
		break;
		case 'J'  :
			stroke[0] = aDot;
			stroke[1] = aDash;
			stroke[2] = aDash;
			stroke[3] = aDash;
		break;
		case 'K'  :
			stroke[0] = aDash;
			stroke[1] = aDot;
			stroke[2] = aDash;
		break;
		case 'L'  :
			stroke[0] = aDot;
			stroke[1] = aDash;
			stroke[2] = aDot;
			stroke[3] = aDot;
		break;
		case 'M'  :
			stroke[0] = aDash;
			stroke[1] = aDash;
		break;
		case 'N'  :
			stroke[0] = aDash;
			stroke[1] = aDot;
		break;
		case 'O'  :
			stroke[0] = aDash;
			stroke[1] = aDash;
			stroke[2] = aDash;
		break;
		case 'P'  :
			stroke[0] = aDot;
			stroke[1] = aDash;
			stroke[2] = aDash;
			stroke[3] = aDot;
		break;
		case 'Q'  :
			stroke[0] = aDash;
			stroke[1] = aDash;
			stroke[2] = aDot;
			stroke[3] = aDash;
		break;
		case 'R'  :
			stroke[0] = aDot;
			stroke[1] = aDash;
			stroke[2] = aDot;
		break;
		case 'S'  :
			stroke[0] = aDot;
			stroke[1] = aDot;
			stroke[2] = aDot;
		break;
		case 'T'  :
			stroke[0] = aDash;
		break;
		case 'U'  :
			stroke[0] = aDot;
			stroke[1] = aDot;
			stroke[2] = aDash;
		break;
		case 'V'  :
			stroke[0] = aDot;
			stroke[1] = aDot;
			stroke[2] = aDot;
			stroke[3] = aDash;
		break;
		case 'W'  :
			stroke[0] = aDot;
			stroke[1] = aDash;
			stroke[2] = aDash;
		break;
		case 'X'  :
			stroke[0] = aDash;
			stroke[1] = aDot;
			stroke[2] = aDot;
			stroke[3] = aDash;
		break;
		case 'Y'  :
			stroke[0] = aDash;
			stroke[1] = aDot;
			stroke[2] = aDash;
			stroke[3] = aDash;
		break;
		case 'Z'  :
			stroke[0] = aDash;
			stroke[1] = aDash;
			stroke[2] = aDot;
			stroke[3] = aDot;
		break;
		
		case '0'  :
			stroke[0] = aDash;
			stroke[1] = aDash;
			stroke[2] = aDash;
			stroke[3] = aDash;
			stroke[4] = aDash;
		break;
		case '1'  :
			stroke[0] = aDot;
			stroke[1] = aDash;
			stroke[2] = aDash;
			stroke[3] = aDash;
			stroke[4] = aDash;
		break;
		case '2'  :
			stroke[0] = aDot;
			stroke[1] = aDot;
			stroke[2] = aDash;
			stroke[3] = aDash;
			stroke[4] = aDash;
		break;
		case '3'  :
			stroke[0] = aDot;
			stroke[1] = aDot;
			stroke[2] = aDot;
			stroke[3] = aDash;
			stroke[4] = aDash;
		break;
		case '4'  :
			stroke[0] = aDot;
			stroke[1] = aDot;
			stroke[2] = aDot;
			stroke[3] = aDot;
			stroke[4] = aDash;
		break;
		case '5'  :
			stroke[0] = aDot;
			stroke[1] = aDot;
			stroke[2] = aDot;
			stroke[3] = aDot;
			stroke[4] = aDot;
		break;
		case '6'  :
			stroke[0] = aDash;
			stroke[1] = aDot;
			stroke[2] = aDot;
			stroke[3] = aDot;
			stroke[4] = aDot;
		break;
		case '7'  :
			stroke[0] = aDash;
			stroke[1] = aDash;
			stroke[2] = aDot;
			stroke[3] = aDot;
			stroke[4] = aDot;
		break;
		case '8'  :
			stroke[0] = aDash;
			stroke[1] = aDash;
			stroke[2] = aDash;
			stroke[3] = aDot;
			stroke[4] = aDot;
		break;
		case '9'  :
			stroke[0] = aDash;
			stroke[1] = aDash;
			stroke[2] = aDash;
			stroke[3] = aDash;
			stroke[4] = aDot;
		break;
	}
}


// It's time to mash down the key, or.. Perhaps not?
bool letter::startStroke(void) {

	ourTimer.setTime(SPACE_MS);						// If we fail, this is the desired result.
	if (strokeIndex>4) {									// We're looking at running past the index here..
		return false;										// So it's a fail.
	} else if (stroke[strokeIndex]==nothing) {	// Valid index but no stroke..
		return false;										// Fail again.
	} else if (stroke[strokeIndex]==aDot) {		// Got a dot?
		setKey(true);										// Mash the key down!
		ourTimer.setTime(DOT_MS);						// Hold it down for this long.
		return true;										// We're a success!
	} else {													// No other choice left but..
		setKey(true);										// A dash. Mash the ket down!
		ourTimer.setTime(DASH_MS);						// Hold it down for this much.
		return true;										// And we are a success!
	}															//
}


// Idle runs the show behind the scenes. Think of it as a little piece of loop() that it
// keeps and uses to run itself.
void letter::idle(void) {

	switch(ourState) {							// In idle, we check state..
		case starting			:					// Starting state.
			strokeIndex = 0;						// Just in case..
			setupStrokes();						// Set up the letter we got.
			if (startStroke()) {					// If the first key down stroke is a success..
				ourState = waitForKey;			// We wait for the correct time to lift the key.
			} else {									// Else, not a success?
				ourState = waitForSpace;		// We'll pause for a blank space.
			}											//
		break;										// And that's that.
		case waitForKey		:					// Waiting for the time to lift the key..
    		if (ourTimer.ding()) {				// If the timer goes off..
    			setKey(false);						// Lift that key.
    			ourTimer.setTime(PAUSE_MS);	// Set timer for pausing time between dits and dahs.
    			ourState = waitKeyPause;		// We'll go wait for that to expire.
    		}											//
		break;										// Wash hands and exit the building.
		case waitKeyPause	:						// Waiting for the time between dits and dahs to expire.
    		if (ourTimer.ding()) {				// Time's up?
    			strokeIndex++;						// Bump the index for the next stroke.
    			if (startStroke()) {				// If this key down stroke is a success.. 
					ourState = waitForKey;		// We'll go back to waiting for time to lift the key.
				} else {								// Else, we're done.
					ourState = waitForSpace;	// So we wait for time of a blank space in the text.
				}										//
    		}											//
		break;										// Again, we're done here.
		case waitForSpace	:						// Waiting for a space between words..
    		if (ourTimer.ding()) {				// If the timer goes off..
    			ourState = waiting;				// We are completely done! Can you believe it?
    		}											//
		break;										//
		case waiting			:					// All done with this letter..
    		// Do nothing and wait.				// We will sit and spin on waiting 'till the user does something with us.
		break;										//
	}													//
}