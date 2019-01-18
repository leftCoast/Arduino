#ifndef ltlOS_h
#define ltlOS_h

#include <iconButton.h>
#include "panel.h"

// Look, just start your numbering at 2 alright?
#define	NO_PANEL_ID		0	// You have to have this guy.
#define	HOME_PANEL_ID	1


// The user will expect icons to click on for panel swapping.
// You can use this. Give it a panel ID and a full path to a
// 32x32 bit .bmp file. Use these to populate your home panel,
// and it does the rest.

class appIcon : public iconButton {
  
  public:
          appIcon(int xLoc,int yLoc,int message,char* path);	// Path to .bmp icon dwg.
  virtual ~appIcon(void);
  
  virtual void  doAction(void);

          int  mMessage;
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
class litlOS :  public idler {

	public:
				litlOS(void);
				litlOS(int homeID);
	virtual	~litlOS(void);

	virtual	int		begin(void);					// The global world is online, do hookups. Returns 0, always
	virtual	panel*	createPanel(int panelID);	// INHERIT THIS GUY AND CREATE YUR OWN CUSTOM PANELS.
	virtual	void		launchPanel(void);			// Dispose of current and launch a newly created panel.
	virtual	void		loop(void);						// Tell the current panel its loop time.
	
				panel*	mPanel;
};

extern  int nextPanel;

#endif
