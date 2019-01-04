#ifndef panel_h
#define panel_h

#include <drawObj.h>
#include <timeObj.h>


// The panel object. Base class for swapping in and out of different objects
// that want to use the screen. The plan is that the OS can swap panels back
// and forth as the user chooses different things he wants to do. Like
// different apps. 

// These two have to go. Need to bring these out from
// the displayObj.

#define PANEL_WIDTH   240
#define PANEL_HEIGHT  320


class panel : public drawGroup {

  public:
          panel(int panelID,bool wantClicks);
  virtual ~panel(void);

           int  getPanelID();
  virtual void  setup(void);
  virtual void  loop(void);
  virtual void  drawSelf(void);
          void  close(void);
          void  delay(unsigned long);
  
          int     mPanelID;   // What panal am I?
          timeObj mTimer;     // We'll use it for the delay() thing.
};


#endif
