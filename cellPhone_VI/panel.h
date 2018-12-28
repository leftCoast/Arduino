#ifndef panel_h
#define panel_h

#include <drawObj.h>

#include <fileBuff.h>


// The panel object. Base class for swapping in and out of different objects
// that want to use the screen. The plan is that the OS can swap panels back
// and forth as the user chooses different things he wants to do. Like
// different apps. Now, what about icons? Have no idea how that's supposed to
// work.

// These two have to go. Need to bring these out from
// the displayObj.

#define PANEL_WIDTH   240
#define PANEL_HEIGHT  320


class panel : public drawGroup,
              public fileBuff {

  public:
          panel(blockFile* inFile,unsigned long rootID,bool menu);
  virtual ~panel(void);

  virtual void  addObj(drawObj* newObj,bool menu);
  virtual void  addObj(drawObj* newObj);
  virtual void  drawSelf(void);
  
  virtual void  panelSetup(void);
  virtual void  panelLoop(void);
  
          drawGroup*  mMenuPanel;
          drawGroup*  mUserPanel; 
};


#endif
