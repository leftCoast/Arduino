#ifndef bmpDrawObj_h
#define bmpDrawObj_h

#include <drawObj.h>
#include <bmpPipe.h>

// IF you have a .bmp file you are using for the background of your drawing..
// You can inherit this and use it to make drawingObj(s) that have the ability
// to draw on that bitmap as a background.
// 
// The way to use this is to call bmpDrawObj::drawSelf(); in the beginning of 
// Your drawSelf(); method. This will stamp out an alignd rectangle of the background
// bitmap on your spot for you to draw on. It also works if your a part of a group. (of groups)

class bmpDrawObj : public drawObj {

public:
  bmpDrawObj(int x,int y,word width,word height,bmpPipe* inBackdrop);
  ~bmpDrawObj(void);
  
virtual void  drawSelf(void);

        bmpPipe*  backdrop;
};

#endif
