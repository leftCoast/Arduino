#ifndef bmpLabel_h
#define bmpLabel_h

#include <bmpPipe.h>
#include <label.h>

class bmpLabel : public label, public bmpPipe {

public:
  bmpLabel(int inX,int inY,word width,word height,char* inText);
  
virtual  void drawBitmap(int x,int y);
virtual void  drawSelf(void);

};

#endif

