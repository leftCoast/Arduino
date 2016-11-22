#ifndef liteLine_h
#define liteLine_h

#include <colorObj.h>
#include <Adafruit_NeoPixel.h>
#include <neoPixel.h>


// ****************** liteLine ******************
//   Basic mechanix for drawing a line segment.
//		Doesn't know where its being drawn, just 
//		Keeps track of positions.
// **********************************************

class liteLine {
   
   public:
      liteLine(neoPixel* inLites,int inLength);
      ~liteLine(void);
      
      			int      getLength(void);									// How long was our line?
      			void     setLength(int inLength);						// Set the line to this length.
      			int 		getNumPixels(void);								// How many pixels are we dealing with here.
      virtual 	colorObj calcColor(int index,int i);     				// What color goes here? index is head, i is this pixel.
      			colorObj getColor(int index);								// What color is here now?
      virtual 	void     setLights(int index,boolean wrap=false); 	// Draw a line at this position.
      
   private:
   	neoPixel* lites;   								
      int       lineLength;
};

   
#endif