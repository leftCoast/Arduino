#ifndef keystroke_h
#define keystroke_h

// Both sides need to see this so it ends up in its own private file.
// Go figure.


// Something being edited needs to know what action to take when a keystroke
// comes in. This should cover it nicely. Actually, input, backspace, arrowFWD & arrowBack
// are the only things an edit field needs to deal with. The rest are used internally by
// the keyboard.
//
// 9/21/2019 - Well now I've added cancel & ok for doing modal keyboards. (The bmp keyboard
// can be modal) - jim lee

enum  keyCommands { input, shift, number, symbol, backspace, arrowFWD, arrowBack, enter, cancel, ok };


// The keyboard is resposible for assembling one of these and sending
// it on to what is being edited.
struct keystroke {
  keyCommands editCommand;  // Edit command or inputted charactor?
  char        theChar;      // If inputted charactor, here it is!
};

#endif
