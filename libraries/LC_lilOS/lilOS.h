#ifndef lilOS_h
#define lilOS_h

#include <iconButton.h>
#include <drawObj.h>
#include <menuBar.h>

// Look, just start your numbering at 2 alright?
#define	NO_PANEL_ID		0		// You have to have this guy.
#define	HOME_PANEL_ID	1

#define	PATH_LEN			100	// Size of buffer for creating file paths.

// The list of standard icons supplied by the OS for the applications to use.
enum stdIcons	{
	mask22, mask32, app32, check22, check32, copy32, cross22, cross32, cut32, edit22,
	edit32, fNew22, fNew32, fSave22, fSave32, fOpen22, fOpen32, FdrNew22, FdrNew32,
	paste32, pref22, pref32, search22, search32, sort22, sort32, trashC22, trashC32,
	trashR22, trashR32, x22, x32
};
										
						
// The user will expect icons to click on for panel swapping.
// You can use this. Give it a panel ID and a full path to a
// 32x32 bit .bmp file. Use these to populate your home panel,
// and it does the rest.

class appIcon : public iconButton {
  
  public:
          appIcon(int xLoc,int yLoc,int message,char* path,int pix=32);	// Path to .bmp icon dwg.
  virtual ~appIcon(void);
  
  virtual void  doAction(void);

          int  mMessage;
};


// Class for building modal dialog boxes.
class modal :	public drawGroup {

	public:
				modal(rect* inRect,eventSet inEventSet=noEvents);
				modal(int x, int y, int width,int height,eventSet inEventSet=noEvents);
	virtual	~modal(void);
	
	virtual	bool	acceptEvent(event* inEvent,point* locaPt);	// Is this event for us? Yes, they ALL are.
	virtual	void	message(int inMsg);
	virtual	void	setSuccess(bool trueFalse);
				
				bool	success;
				bool	done;
};



// The base panel class

// The panel object. Base class for swapping in and out of different objects
// that want to use the screen. The plan is that the OS can swap panels back
// and forth as the user chooses different things he wants to do. Like
// different apps. Each panel has its own setup() and loop() functions you can
// fill in to pretend your in a "real" sketch. Wow, just like a real boy!

enum menuBarChoices { noMenuBar, emptyMenuBar, closeBoxMenuBar };


// These two have to go. Need to bring these out from
// the displayObj. But, I'm in a hurry and I'm lazy.
#define PANEL_WIDTH   240
#define PANEL_HEIGHT  320

class menuBar;
class lilOS;

class panel :	public drawGroup,
					public listener {

	public:
				panel(int panelID,menuBarChoices menuBarChoice=closeBoxMenuBar,eventSet inEventSet=noEvents);
	virtual	~panel(void);
	
				int	getPanelID();
	virtual	void	setup(void);
	virtual	void	loop(void);
	virtual	void	drawSelf(void);
	virtual	void	close(void);
	virtual	void	closing(void);
	virtual	void	handleCom(stdComs comID);
	
				int		mPanelID;   // What panal am I?
				menuBar*	mMenuBar;	// My menu bar, if so desired.
};


// Default home panel with panelID of HOME_PANEL_ID
class homePanel : public panel {

  public:
          homePanel(void);
  virtual ~homePanel(void);
  
  virtual void  setup(void);
  virtual void  loop(void);
  virtual void  drawSelf(void);
    
};


// Our base class for a little OS that switches "panels"
// for user interaction. These panels can be thought of
// as virtual Arduino sketches. Meaning? Everything on
// them must be able to "survive" being deleted when the
// user switches functions. You'll be able to get more
// info in the "panel" files.
class lilOS :  public idler {

	public:
				lilOS(void);
				lilOS(int homeID);
	virtual	~lilOS(void);

	virtual	int		begin(void);					// The global world is online, do hookups. Returns 0, always
	virtual	panel*	createPanel(int panelID);	// INHERIT THIS GUY AND CREATE YUR OWN CUSTOM PANELS.
	virtual	void		launchPanel(void);			// Dispose of current and launch a newly created panel.
	virtual	void		loop(void);						// Tell the current panel its loop time.
	
	// Calls to be overwritten by used version.
	virtual	void		beep(void) = 0;
	virtual	int		getTonePin(void) = 0;
	virtual	void		setBrightness(byte brightness) = 0;
	virtual	char*		getSystemFolder() = 0;
	virtual	char*		getPanelFolder(int panelID) = 0;
	virtual	char*		stdIconPath(stdIcons theIcon);
	
	
				char		pathBuff[PATH_LEN];			// Workspace for building up file paths.
				panel*	mPanel;
				bmpMask	icon32Mask;
				bmpMask	icon22Mask;
};

extern int		nextPanel;
extern lilOS*	ourOSPtr;


#endif
