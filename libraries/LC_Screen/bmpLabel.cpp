
#include "bmpLabel.h"

bmpLabel::bmpLabel(int inX,int inY,word width,word height,char* inText,bmpPipe* inBackdrop) 
  : label(inX,inY,width,height,inText) { backdrop = inBackdrop; }
  

// We go through all this nonsense so we can redraw ourselves on every line.
// This actually hides the major flicker issue we would have otherwise.
void bmpLabel::drawSelf(void){
  
  File	bmpFile;
  int 	trace;
  int 	endY;
  int 	srcY;
  rect 	temp;
  
  if (backdrop) {
  	bmpFile = backdrop->getFile();
  	if (bmpFile) {
  		temp.x = x;
    	temp.y = y;
    	temp.width = width;
    	temp.height = height;
    	backdrop->setSourceRect(temp);                                                                                     
  		endY = y+height;
  		srcY = y;
  		for (trace=y; trace<=endY;trace++) {
    		bmpFile.seek(backdrop->filePtr(x,srcY++));
    		backdrop->drawLine(bmpFile,x,trace);
  			label::drawSelf();
  			//screen->drawRect(x,y,width,height,&red);
  		} 
  		bmpFile.close();
  	}
  }      
}


