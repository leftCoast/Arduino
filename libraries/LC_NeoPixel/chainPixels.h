#ifndef chainPixels_h
#define chainPixels_h

#include <colorObj.h>
#include <neoPixel.h>
#include <idlers.h>
#include <lists.h>

// If you have separate pixel groups that want to act independently but
// are wired in daisy chain fashion. This is beyond handy.
//
// You program each pixelGroup as if it was the only thing on the chain.
// Then, you add all the pixelGroup objects to the global pixelGroup in
// the order that they were wired. All of the addresses are delt with in
// the background automatically.
//
// For example I did a Quad copter that had a taillight that reflected 
// how the machine was being flown. Power settings, forward, reverse, left
// right. And a couple of floodlights in front. All neoPixle groups wired
// up on one daisy chain data line.
// 
// ON another quadcopter each arm had a glowing ring of lights. So we just
// added an instance of the same object four times.

class pixelGroup;


class chainPixels : public queue, public idler {
  
  public:
            chainPixels(byte inPin);
	virtual	~chainPixels(void);
              
            void		resetChain(void);
	virtual  void		push(linkListObj* item);
	virtual  void		pop(void);
				void		addGroup(pixelGroup* inGroup);
	virtual  void		idle(void);
				colorObj	getPixelColor(word index);
				void		setPixelColor(word index,colorObj* inColor);
             
  protected:
  
    			byte			pin;
    			word			numPixels;
    			neoPixel*	pixelDriver;
    			boolean		dirtyBit;
};


class pixelGroup : public linkListObj {

	public:
            pixelGroup(word inNumPixels);
	virtual	~pixelGroup(void);
            
				void		setIndex(word inIndex);
				void		setChain(chainPixels* inChain);
				word		getNumPixels(void);
              
				colorObj	getPixelColor(word pixelNum);							// What color is THIS pixel now?
				void		setPixelColor(word pixelNum, colorObj* color);	// Set THIS pixel this color.
				void		setPixels(colorObj* color);							// Set ALL our pixles this color.
				colorObj	shiftPixels(bool toEnd=true);							// Even handier!
				void		roll(bool clockwise=true);
	virtual	void		draw(void);													// This will be called repeatedly.
    
	protected:
      		chainPixels*	ourChain;
      		word				numPixels;
      		word				index;
};

#endif
