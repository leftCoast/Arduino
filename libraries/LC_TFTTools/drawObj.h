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
  virtual bool  inRect(TS_Point inPt);                     // Is this point in this rect?

//protected:
  TS_Point location;
  word  width;
  word  height;
};


// *********************  
// Base class for an object that can be drawn on the screen.
// *********************

class drawObj : public rect {

public:
  drawObj();
  drawObj(TS_Point inLoc, word inWidth,word inHeight);
  ~drawObj();

          boolean  getRefresh(void);    // These are for later. When we get clipping support.
          void     setRefresh(void);    //
          void     draw(void);          // Don't inherit this one.
  virtual void     drawSelf(void);      // Inherit this one and make it yours.
  
protected:
  boolean        needRefresh;
};


#endif

