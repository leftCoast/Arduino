#ifndef drawObj_h
#define drawObj_h

#include <lists.h>
#include <idlers.h>
#include <baseGraphics.h>
#include <screen.h>

// ***************************************************************
// Base class for an object that can be drawn on the screen.
// Possibly clicked on.
// ***************************************************************


class drawObj : public rect, public dblLinkListObj {

	public:
  				drawObj();
  				drawObj(rect* inRect,bool inClicks=false);
  				drawObj(int inLocX, int inLocY, int inWidth,int inHeight,bool inClicks=false);
	virtual	~drawObj();
    
   virtual	bool		wantRefresh(void);
   virtual	void		setNeedRefresh(void);
	virtual	void		setLocation(int x,int y);
   virtual	void  	draw(void);								// Call this one. Don't inherit this one.
   virtual	void		eraseSelf(void);						// Mostly you can ignore this one. Used for animation.
   virtual 	void  	drawSelf(void);						// Inherit this one and make it yours.
	virtual	void		setFocus(bool setLoose);			// We are either getting or loosing focus.
  		    	void		clickable(bool inWantsClicks);
   virtual 	bool		acceptClick(point where);
   virtual 	void  	clickOver(void);
   virtual	void  	doAction(void);						// Override me for action!
          	void		setCallback(void(*funct)(void));	// Or use a callback? Forgot how to use this.
  
protected:
  bool	needRefresh;
  bool	focus;
  bool	wantsClicks;
  bool	clicked;
  void	(*callback)(void);
  int		lastX;					// Yes, these are where we were before we moved.
  int		lastY;
};


// ***************************************************************
// Here we have the management of the screen. Get the clicks,
// find drawObjs to act on the clicks and tell the objects when to
// redraw themselves.
//
// "viewList" is the global that does all these things during
// idle time.
// ***************************************************************


class viewMgr : public idler {
    
public:
            viewMgr(void);
	virtual	~viewMgr(void);
    
	virtual	void		addObj(drawObj* newObj);
    			void		dumpDrawObjList(void);
            bool		checkClicks(void);			// Ends up being a bit recursive.
	virtual	void    	checkRefresh(void);
				int		numObjInList(void);
				drawObj*	getObj(int index);
				drawObj*	theList(void);
    virtual void    	idle(void);
    
            drawObj	listHeader;				// Header of the drawObj list;
};

extern	viewMgr 	viewList;								// Our global GUI manager.
extern 	drawObj*	theTouched;								// Who's accepted a finger touch on the screen?
extern	drawObj*	currentFocus;							// Focus goes hand in hand with view management.
extern 	void		setFocusPtr(drawObj* newFocus);	// Anyone can set focus by calling this function.



// ***************************************************************
// The idea here is to have a draw object that manages a group of
// drawObjects. This is the base of all lists & popups. Or at least,
// that's the plan.
// ***************************************************************

class drawGroup : public drawObj, public viewMgr {

	public:
				drawGroup(rect* inRect,bool clicks=false);
				drawGroup(int x, int y, int width,int height,bool clicks=false);
  	virtual	~drawGroup();

	virtual	bool		checkGroupRefresh(void);
	virtual	void		setLocation(int x,int y);
	virtual	void		setGroupRefresh(void);
	virtual	bool		wantRefresh(void);
	virtual	void		setNeedRefresh(void);
	virtual	bool		acceptClick(point where);
  	virtual	void  	addObj(drawObj* newObj);
  	virtual	void  	draw(void);
};			
		
		
		
class drawList : public drawGroup {
 
	public:
				drawList(rect* inRect,bool clicks=false,bool vertical=true);
				drawList(int x, int y, int width,int height,bool clicks=false,bool vertical=true);
  	virtual	~drawList();
  					
  	virtual	void		addObj(drawObj* newObj);
  				void		resetPositions(void);
  				bool		canMove(bool upLeft);
  				void		movelist(bool upLeft);
  				int		lastY(void);
  				bool		isVisible(drawObj* theItem);
				void		showItem(drawObj* theItem);
				drawObj*	findItem(point* where);
				
  				int	itemHeight;
  				int	itemWidth;
  				bool	mVertical;
  	
};


#endif

