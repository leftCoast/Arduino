#ifndef waypoints_h
#define waypoints_h

"WORK IN PROGRESS!!!"

#include <globalPos.h>
#include <lists.h>

enum {
	
	isWaypoint,
	isRout,
	


// Named positions on the globe.
class waypoint : public globalPos {

	public:
				waypoint(void);
	virtual	~waypoint(void);

				char*	getName(void);
				
				char*	name;
};


class routItem :	public dblLinkListObj {

	public:
				routItem(void);
	virtual	~routItem(void);

				waypoint*	ourWaypoint;
};


// Bidirectonal list of waypoints & routes. Each direction has unique name. There is fwd
// name and rev name. When you add waypoints into a route, you put them and the beginning
// or at the end. The fwdName starts at the beginning and goes to the end. revName starts
// at the end and goes to the beginning.

enum direction {
	fwdDirection,
	revDirection
};


class routeMgr :	public idler {

	public:
				route(void);
	virtual	~route(void);
	
	virtual	void			addWaypoint(waypoint* inWaypoint, bool toEnd=true);
	virtual	void			startRout(bool fromStart=true);
	virtual	waypoint*	getWaypoint(int inIndex);
	
				int			routIndex;
				routeItem*	ourRout;			
};



#endif