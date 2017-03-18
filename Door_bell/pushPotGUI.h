#ifndef pushPotGUI_h
#define pushPotGUI_h

// When you can't have a pointing device, like a touchscreen
// or a mouse, you may need some other way to navigate a GUI. 
// One way is with a button/pot. Its a 10k Ohm pot that you
// can push down like a button. Hence the name pushPotGUI.
//
// Lets talk about how a pushPotGUI works. The basic idea is
// that the GUI selects something and then is ignored. Possibly 
// the calling program will blank the screen or what have you.
// Then at, random times, the GUI will be reset and it'll need
// to run until it calls something that ends it all over again.
//
// So the master program will create the GUI then when it needs
// it, call restart() Then it feeds it raw pot values and 
// button clicks. The calling program keeps control of the
// push/pot because ,it may want to use the inputs somewhere
// else while the pushPotGUI is dormant.
//
// So what is it? Its ment to be mixed in with the drawGroup
// class or one of its diriviteves to create a controlling 
// group class. There is one global controlPtr that the calling
// program will set by calling becomeContol(). Then through
// whatever actions happen the controller can change around
// causing subObjects to be focused. In this way the button
// click will call the correct action().
//
// Looking at it a little lower down. We use the pot value to
// scroll the global "focus" around in our subObjects. When a 
// button click comes in, just call the action() metho on whomever
// has the focus at that moment. 


// At least, that's the plan..

class pushPotGUI {

  public:
  pushPotGUI(void);
  ~pushPotGUI(void);

  virtual void    reset(void);          // Stuff to do when we're started or restarted.
  virtual void    gainControl(void);    // Being told we now have control.
  virtual void    doPotVal(int aVal);   // Calliing program sends us in raw pot values.
 
};

extern  pushPotGUI*  controlPtr;

void  setControlPtr(pushPotGUI* newController);
bool  potVal(int aVal);
bool  buttonClick(void);


#endif
