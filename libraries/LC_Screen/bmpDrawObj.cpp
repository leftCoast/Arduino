#include "bmpDrawObj.h"


bmpDrawObj::bmpDrawObj(int x,int y,word width,word height,bmpPipe* inBackdrop)
  : drawObj(x,y,width,height) { backdrop = inBackdrop; }


bmpDrawObj::~bmpDrawObj(void) { }


// All the drawing does is to erase the rectangle by pasting over the
// bitmap. This gets it ready for the inherited to draw to.
void bmpDrawObj::drawSelf(void) {

  rect   temp;
      
  temp = (rect) *this;                            // Get our local rectangle.
  temp.x = screen->gX(temp.x);                    // Convert to global.
  temp.y = screen->gY(temp.y);
  backdrop->setSourceRect(temp);                  // Set the copy rect in global.
  backdrop->drawBitmap(x,y);                      // Draw in local.
}


