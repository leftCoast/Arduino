#ifndef panel_h
#define panel_h

// The panel object. Base class for swapping in and out of different objects
// that want to use the screen. The plan is that the OS can swap panels back
// and forth as the user chooses different things he wants to do. Like
// different apps. Each panel has its own setup() and loop() functions you can
// fill in to pretend your in a "real" sketch. Wow, just like a real boy!

#include <drawObj.h>
#include <timeObj.h>
#include <menuBar.h>

enum menuBarChoices { noMenuBar, emptyMenuBar, closeBoxMenuBar };


// These two have to go. Need to bring these out from
// the displayObj. But, I'm in a hurry and I'm lazy.
#define PANEL_WIDTH   240
#define PANEL_HEIGHT  320

class menuBar;

class panel : public drawGroup {

	public:
				panel(int panelID,menuBarChoices menuBarChoice=closeBoxMenuBar,eventSet inEventSet=noEvents);
	virtual	~panel(void);
	
				int	getPanelID();
	virtual	void	setup(void);
	virtual	void	loop(void);
	virtual	void	drawSelf(void);
	virtual	void	close(void);
	virtual	void	closing(void);
	//virtual	void	sleep(unsigned long);
  
				int		mPanelID;   // What panal am I?
				menuBar*	mMenuBar;	// My menu bar, if so desired.
				timeObj	mTimer;     // We'll use it for the sleep() thing.
};


#endif
