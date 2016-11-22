
#include "liteLine.h"


liteLine::liteLine(neoPixel* inLites,int inLength) {
 
 	lites = inLites;
 	lineLength = inLength;
}
  

// I don't think we "own" the neoPixel object. So don't delete it.   
liteLine::~liteLine(void) { }
     
 
// Forgot how long it was?     
int liteLine::getLength(void) { return lineLength; }


// Well, maybe they want to change it?
void liteLine::setLength(int inLength) { lineLength = inLength; }	


// How many pixels are out there?
int liteLine::getNumPixels(void) {
	
	if (lites)
		return lites->numPixels();
	else
		return 0;
}


// What color is already here?
colorObj liteLine::getColor(int index) { 

	if (lites)
		return lites->getPixelColor(index);
	else 
		return BLACK;
}
 
 
//inherited and filled out to figure each pixel. 
colorObj  liteLine::calcColor(int index,int i) { 

	colorObj aColor(10,10,10);
  	return aColor;
}


// This is used to "draw" the line. Wrap is used by things like circles.
void liteLine::setLights(int index,boolean wrap) {

   int liteIndex;
   colorObj color;
   
   if (lites) {
   	if (wrap) {
      	for(int i=0;i<lineLength;i++) {
         	color = calcColor(index,i);
         	liteIndex = index-i;
         	if (liteIndex<0) {
            	liteIndex = liteIndex + lites->numPixels();
         	}
         	lites->setPixelColor(liteIndex, &color);
      	}
   	} else {
      	for(int i=0;i<lineLength;i++) {
         	color = calcColor(index,i);
         	lites->setPixelColor(index-i, &color);
      	}
   	}
   }
}

   