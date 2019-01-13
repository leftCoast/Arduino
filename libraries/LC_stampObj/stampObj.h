#ifndef stampObj_h
#define stampObj_h

#include <drawObj.h>


// stampObj, is a lightweight package of drawing commands. stamObj
// DOES NOT rely on drawObj or its drawing environment to function.
// This means it can be used in simple Arduino sketches that don't
// incorporate the drawObj drawing framework. 

// When creating drawing commands for a stampObj, its important to
// use it's internal x,y coordinates as your origin. Then, whomever
// uses your stamp object, can position it anywhere on the screen.
// This also allows it to seamlessly function IN the drawObj drawing
// environment, as an add in by other drawObj(s) for their drawSelf()
// methods.
//
// Cheap, reusable, flexible, simple to create and use.. Who wouldn't
// love it?!
//
// So then.. It works perfectly well in or out of the drawing environment,
// what is the difference between a stampObj and a drawObj?
//
// Both drawObj(s) and stampObj(s) use the global screenObj, "screen" to do
// their drawing. The difference lie in the fact that drawObject(s) "live"
// in an automated redraw system. Things can effect them that will causes
// their needRefresh flag to be set. All of their drawing is done
// automatically "behind the scenes" during idle time. Along with this
// automatic refresh system, they also support groups and subgroups with
// all the management that goes along with it.
// 
// stampObj(s) have none of this automated management. They are just packs
// of drawing commands. Just like a stamp, you aim it at the screen and
// stamp out its drawing commands, Bam! You still use the global screen
// object to do the actual drawing, but that's about it.
//
// Want a smily face icon here or there? Create a stamp for it and you can 
// stamp smily faces wherever on the screen you like. Or, you can add it into
// a drawObj's drawSelf() method and have that as part of its drawing package.


class stampObj :	public rect {

	public:
				stampObj(rect* aRect);
	virtual	~stampObj(void);
	
	virtual	void stamp(void);
};

#endif	
				