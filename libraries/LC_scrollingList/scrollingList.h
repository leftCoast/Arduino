#ifndef scrollingList_h
#define scrollingList_h

// Base class for our scrollable collection of.. drawObj(s) living
// on drawList(s). You fill up a drawList with objects and this
// is the interface used to control scrolling it.
//
// Now, there are a few different ways to do this.
//
// First is the pushPotGUI. The pushPotGUI is a simple rotating
// potentiometer that you can push to click. A very simple GUI
// that is only really suited to really simple applications.
//
// Second is button iterface. But I've not written that one yet.
//
// Third is single touch, touchscreen. And that's what we're
// working on today!


#include <drawObj.h>
#include <timeObj.h>


enum scrollType { touchScroll, dialScroll, dSOpenTop, dSOpenBtm, dSOpenBoth, buttonScroll };

enum locType { onList, aboveList, belowList };

class scrollingList : public drawList {

	public:
				scrollingList(int x, int y, int width,int height,scrollType sType,eventSet inEventSet=noEvents,bool vertical=true);
	virtual	~scrollingList(void);

				void		setScrollValue(float percent);  // Who do we want to see next?
	virtual	void		offList(void);                  // We off list? If so, here is where you find out.         

				void		dragVertical(event* inEvent);
				void		dragHorizontal(event* inEvent);
	virtual	void		doAction(event* inEvent,point* locaPt);
  
				scrollType	mType;
				locType		mLoc;
				point			mStartPt;
};


#endif
