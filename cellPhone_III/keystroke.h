#ifndef keystroke_h
#define keystroke_h

// Both sides need to see this so it ends up in its own private file.
// Go figure.


// Something being edited needs to know what action to take when a keystroke
// comes in. This should cover it nicely.
enum  keyCommands { input, shift, backspace, arrowFWD, arrowBack, enter };


// The keyboard is resposible for assembling one of these and sending
// it on to what is being edited.
struct keystroke {
  keyCommands editCommand;  // Edit command or inputtd charactor?
  char        theChar;      // If inputted charactor, here it is!
};



#endif
