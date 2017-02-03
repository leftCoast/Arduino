
#include "bmpLabel.h"

bmpLabel::bmpLabel(int inX,int inY,word width,word height,char* inText) 
  : label(inX,inY,width,height,inText) { 
    
    rect temp;
    
    temp.x = inX;
    temp.y = inY;
    temp.width = width;
    temp.height = height;
    setSourceRect(temp);
  }

void bmpLabel::drawBitmap(int x,int y) {
  
  int trace;
  int endY;
  int srcY;
  
  if (haveInfo) {                                                                                      
    endY = y+sourceRect.height;
    srcY = sourceRect.y;
    for (trace=y; trace<=endY;trace++) {
      bmpFile.seek(filePtr(x,srcY++));
      drawLine(x,trace);
      label::drawSelf();
    }       
  }
}

void bmpLabel::drawSelf(void) { drawBitmap(x,y); }

