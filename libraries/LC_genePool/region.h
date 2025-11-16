#ifndef region_h
#define region_h


#include	"baseGraphics.h"
#include "lists.h"

class regionRectObj;

class region :	public linkList {

	public:
						region(void);
	virtual			~region(void);
	
				
				void				addRect(rect* inRect);
				void				addRegion(region* inRegion);
				bool				pointInRegion(point* inPoint);
				bool				rectOverlapRegion(rect* inRect);
				bool				regionOverlapRegion(region* inRegion);
				rect				getEnclosingRect(void);
	virtual	regionRectObj*	getFirstRectObj(void);
	
	protected:
				void	checkTail(regionRectObj* current);
				
				rect	enclosingRect;
};



class regionRectObj :	public linkListObj,
								public rect {

	public:
						regionRectObj(rect* inRect);
						regionRectObj(int inX,int inY,int inWidth,int inHeight);
	virtual			~regionRectObj(void);
	
				long				getArea(void);
	virtual	regionRectObj*	getNextRectObj(void);
				
	private:
				long	area;
};
						
#endif				
									