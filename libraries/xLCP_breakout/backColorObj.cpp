#include "backColorObj.h"

backColorObj::backColorObj(void) { }
backColorObj::~backColorObj(void) { }
void backColorObj::setBackColor(colorObj* aColor) { setColor(aColor); }
void backColorObj::setBackColor(byte r,byte g,byte b) { setColor(r,g,b); }