#ifndef drawObj_h
#define drawObj_h

#include <lists.h>
#include <idlers.h>
#include "screen.h"


// ***************************************************************
// Base class for a rectangle. 
// ***************************************************************

class rect {

public :
  rect(void);
  rect(word x, word y, word inWidth,word inHeight);
  ~rect(void);

  virtual void  setLocation(word x, word y);
  virtual void  setSize(word inWidth,word inHeight);
  virtual void  setRect(rect* inRect);                  // Got a rect? Make this one the same.
  virtual void  setRect(point* inPt1,point* inPt2);	  // Or two points..
          word  maxX(void);                             // Where's our last pixel?
          word  maxY(void);                             // Same as obove but in the Y direction.
          word  minX(void);                             // Where's our first pixel?
          word  minY(void);                             // Same as obove but in the Y direction.
  virtual bool  inRect(word x, word y);                  // Is this point in us?

//protected:
  word  locX;
  word  locY;
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
          void      draw(void);                    // Call this one. Don't inherit this one.
  virtual void      drawSelf(void);                // Inherit this one and make it yours.
  		    void		  clickable(boolean inWantsClicks);
  virtual boolean   acceptClick(point where);
  virtual void      clickOver(void);
  virtual void      doAction(void);      				// Override me for action!
  			 void		  setCallback(void(*funct)(void));		// Or use a callback?
  
protected:
  boolean        needRefresh;
  boolean        wantsClicks;
  boolean        clicked;
  void			  (*callback)(void);
};


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

