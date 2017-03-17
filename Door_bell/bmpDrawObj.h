#ifndef bmpDrawObj_h
#define bmpDrawObj_h

#include <drawObj.h>
#include <bmpPipe.h>

class bmpDrawObj : public drawObj {

public:
  bmpDrawObj(int x,int y,word width,word height,bmpPipe* inBackdrop);

virtual void  drawSelf(void);

        bmpPipe*  backdrop;
};

#endif
