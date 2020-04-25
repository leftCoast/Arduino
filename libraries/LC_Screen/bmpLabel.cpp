
#include "bmpLabel.h"

bmpLabel::bmpLabel(int inX,int inY,int width,int height,char* inText,bmpPipe* inBackdrop) 
  : label(inX,inY,width,height,inText) { backdrop = inBackdrop; fastDraw = true; }


bmpLabel::~bmpLabel(void) {  }

  
void bmpLabel::setFastDraw(bool fast) { fastDraw = fast; }


// We go through all this nonsense so we can redraw ourselves on every line.
// This actually hides the major flicker issue we would have otherwise.
void bmpLabel::smoothDraw(void) {
  
  File	bmpFile;
  int 	trace;
  int 	endY;
  int 	srcY;
  rect 	temp;
  
  if (backdrop) {
  	bmpFile = backdrop->getFile();
  	if (bmpFile) {
  		temp = (rect) *this;											// Get our local rectangle.
  		temp.x = screen->gX(temp.x);								// Convert to global.
			temp.y = screen->gY(temp.y);
    	backdrop->setSourceRect(&temp);							// Reading background is global.                                                                                     
  		endY = y+height;
  		srcY = temp.y;													// Anything for background global.
  		for (trace=y; trace<endY;trace++) {						// Drawing y, local.
    		bmpFile.seek(backdrop->filePtr(temp.x,srcY++));	// These have to be forced global.
    		backdrop->drawLine(bmpFile,x,trace);				// Drawing is all local.
  			label::drawSelf();										// Local.
  			//screen->drawRect(x,y,width,height,&red);
  		} 
  		bmpFile.close();
  	}
  }      
}


void bmpLabel::fasterDraw(void) {
	
	rect 	temp;
			
	temp = (rect) *this;					// Get our local rectangle.
	temp.x = screen->gX(temp.x);		// Convert to global.
	temp.y = screen->gY(temp.y);
	backdrop->setSourceRect(&temp);
	backdrop->drawImage(x,y);			// Draw in local.
	label::drawSelf();
}


void bmpLabel::drawSelf(void){

	if (fastDraw) 
		fasterDraw();
	else
		smoothDraw();
}