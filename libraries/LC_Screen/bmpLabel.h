#ifndef bmpLabel_h
#define bmpLabel_h

#include <bmpPipe.h>
#include <label.h>

class bmpLabel : public label {

public:
  bmpLabel(int inX,int inY,word width,word height,char* inText,bmpPipe* inBackdrop);
  
virtual void  drawSelf(void);

				bmpPipe*	backdrop;
};

#endif

