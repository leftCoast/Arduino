#ifndef drawObj_h
#define drawObj_h

#include "screenObj.h"


// *********************  
// Base class for a rectangle. 
// *********************

class rect {

public :
  rect(void);
  rect(word x, word y, word inWidth,word inHeight);
  rect(TS_Point location, word inWidth,word inHeight);
  ~rect(void);

  virtual void  setLocation(TS_Point inLoc);
  virtual void  setLocation(word x, word y);
  virtual void  setSize(word inWidth,word inHeight);
  virtual void  setRect(rect inRect);                   // Got a rect? Make this one the same.
  virtual word  maxX(void);                             // Where's our last pixel?
  virtual word  maxY(void);                             // Same as obove but in the Y direction.
  virtual word  minX(void);                             // Where's our first pixel?
  virtual word  minY(void);                             // Same as obove but in the Y direction.
  virtual bool  inRect(TS_Point inPt);                  // Is this point in us?

//protected:
  TS_Point location;
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
  drawObj(TS_Point inLoc, word inWidth,word inHeight,boolean inClicks=false);
  drawObj(word locX, word locY, word inWidth,word inHeight,boolean inClicks=false);
  ~drawObj();

          void  draw(void);                    // Call this one. Don't inherit this one.
  virtual void  drawSelf(void);                // Inherit this one and make it yours.
  virtual void  doAction(TS_Point where);      // Override me for action!
  
protected:
  boolean        needRefresh;
  boolean        wantsClicks;
};


viewMgr : public idler {
    
public:
                    viewMgr(void);
                    ~viewMgr(void);
    
            void    addObj(drawObj* newObj);
    virtual void    idle(void);
    
            drawObj*    theList;    // Pointer to top of list;
            drawObj*    theTouched; // Pointer to who's been touched.
};

#endif

