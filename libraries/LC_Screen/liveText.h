#ifndef liveText_h
#define liveText_h

#include <timeObj.h>
#include <idlers.h>
#include <colorObj.h>
#include <label.h>

// Live text is text that can change over time in the background. This could be blinking?
// Or color changes or fade out. You pretty much have complete control over the line of text
// as one line. You don't have a char by char control here. Just color over time on a string.
// 
// You have your standard x,y,length, height paramiters. Then you have framerateMs.

// framerateMs : sets how long between changes it waits. This is not a perfect clock. 
// Things can effect it like the program doing something else somewhere else. But it 
// does the best it can and really all this is for is just looks & sizzle. So it should
// be fine.
//
// Loop : Tells us true - this should go on over and over forever. Or false - Do it once and stop.
//
// How long does the animation go on for? No parameter for that. Ah! This is the cool bit.
// The colorMultiMap sets this.
//
// addAColor() takes a time & color. Time is an integer representing milisoncds
// and color is a coloObj. You want your first color to start at time 0. Then you
// add them whatever color you want at wheatever time. It will sort them out in
// time order and blend over time from color to color.
// 
// For example : Show a string for 4 seconds then fade to black over 1 second.
// 
// addAColor(0,&white);
// addAColor(4000,&white);
// addAColor(5000,&black);
//
// That will do it.
//
// hold() and release() are for triggering starts and stops of animation.
// release() can reset the animation to 0 frame for you if you'd like.
//
// NOTE : Best to use this over solid colors. Trying to animate on a .bmp
// bakckground is a flashy mess. Too slow.


class liveText : public label,
                 public idler, 
                 public colorMultiMap,
                 public timeObj {

  public:
				liveText(int x,int y, int width, int height,int framerateMs=100,bool loop=false);
	virtual	~liveText(void);
                
				void		addAColor(int timeMs,colorObj* color);
				void		hold(void);
				void		release(bool reset=true);

	virtual	bool		wantRefresh(void);        
	virtual	void		idle(void);
				void		setCalcColor(void);
  
				bool		loop;
				int		maxTime;
				int		frame;
				bool		holding;
};

#endif
