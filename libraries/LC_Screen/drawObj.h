#ifndef drawObj_h
#define drawObj_h

#include <lists.h>
#include <idlers.h>
#include "screen.h"

// point, I know what you're thinking. It should be here. But its in displayObj.h.
// Its needed by both sides.
  
// ***************************************************************
// Base class for a rectangle. 
// ***************************************************************

class rect {

public :
  rect(void);
  rect(int inX, int inY, word inWidth,word inHeight);
  ~rect(void);

  			  void  setLocation(int inX, int inY);
  			  void  setSize(word inWidth,word inHeight);
  			  void  setRect(rect* inRect);                  // Got a rect? Make this one the same.
  			  void  setRect(point* inPt1,point* inPt2);	    // Or two points..
          int  maxX(void);                             // Where's our last pixel?
          int  maxY(void);                             // Same as obove but in the Y direction.
          int  minX(void);                             // Where's our first pixel?
          int  minY(void);                             // Same as obove but in the Y direction.
  			  bool inRect(int inX, int inY);                 // Is this point in us?

//protected:
  int   x;
  int   y;
  word  width;
  word  height;
};


// ***************************************************************
// Base class for an object that can be drawn on the screen.
// Possibly clicked on.
// ***************************************************************

class drawObj : public rect, public dblLinkListObj {

public:
  drawObj();
  drawObj(word inLocX, word inLocY, word inWidth,word inHeight,boolean inClicks=false);
  ~drawObj();
    
          boolean   wantRefresh(void);
          word		scrX(void);												// ScrX() returns the global screen x location.
          word		scrY(void);												// ScrY() returns the global screen x location.
          rect		scrRect();												// Our rect in global coordinates.
          void      draw(void);                    		// Call this one. Don't inherit this one.
  virtual void      drawSelf(void);                		// Inherit this one and make it yours.
  		    void	clickable(boolean inWantsClicks);
  virtual boolean   acceptClick(point where);
  virtual void      clickOver(void);
  virtual void      doAction(void);      								// Override me for action!
          void		setCallback(void(*funct)(void));		// Or use a callback?
  
protected:
  boolean     needRefresh;
  boolean     wantsClicks;
  boolean     clicked;
  drawObj*		parentObj;
  void			  (*callback)(void);
};


// ***************************************************************
// Here we have the management of the screen. Get the clicks,
// find drawObjs to act on the clicks and tell the objects when to
// redraw themselves.
//
// "viewList" is the global that does all these things during
// idle time.
// ***************************************************************

class viewMgr : public idler {
    
public:
                    viewMgr(void);
                    ~viewMgr(void);
    
            void    addObj(drawObj* newObj);
            boolean checkClicks(void);
            void    checkRefresh(void);
    virtual void    idle(void);
    
            drawObj*    theList;    // Pointer to top of list;
            drawObj*    theTouched; // Pointer to who's been touched.
};

extern viewMgr viewList;

#endif

