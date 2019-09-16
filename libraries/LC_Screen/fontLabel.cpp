#include "fontLabel.h"
#include "Fonts/FreeSans9pt7b.h"

#define Y_SANS9_OFFSET  13
 
 
 
fontLabel::fontLabel(int inX, int inY, int inWidth,int inHeight)
  : label(inX,inY,inWidth,inHeight) { }

  
fontLabel::~fontLabel(void) {  }


void fontLabel::drawSelf(void) {
  
  y = y + Y_SANS9_OFFSET;
  screen->setFont(&FreeSans9pt7b);
  label::drawSelf();
  screen->setFont(NULL);
  y = y - Y_SANS9_OFFSET;
}
