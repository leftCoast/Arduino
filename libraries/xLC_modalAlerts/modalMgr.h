#ifndef modalMgr_h
#define modalMgr_h

#include <lists.h>
#include <idlers.h>
#include <drawObj.h>


// Why this manager for modals? Here's the issue. Modals are typically placed "above" the
// rest of your screen items, and that causes all sorts of redraw and click through 
// issues. Also, there is the issue of modal's needing to be deleted because they also
// tend to be temporary in nature. So this gives us three issues to deal with.
//
// 1) Need to draw after everything below is completed drawing.
// 2) Need to block other screen items below from getting clicks.
// 3) Need to have a mechanism for auto deletion. Because deleting oneself leads to
//    nasty crashing.


class modalLink;

// Class for building modal dialog boxes.
class modal :	public drawGroup {

	public:
				modal(rect* inRect,eventSet inEventSet=noEvents);
				modal(int x, int y, int width,int height,eventSet inEventSet=noEvents);
	virtual	~modal(void);
	
				void	init(void);
	virtual	void  checkIfReady(void);
	virtual	void	draw(void);
	virtual	bool	acceptEvent(event* inEvent,point* locaPt);
				
				modalLink*	ourLink;
				bool			drawing;
				bool			done;
};


// A link list link to track modals.
class modalLink :	public linkListObj {
	
	public:
				modalLink(modal* inModal);
	virtual	~modalLink(void);
				
				bool	complete(void);
				
				modal*	ourModal;
};


// A link list manager to do the actual tracking.
class modalMgr :	public linkList,
						public idler {

	public:				
				modalMgr(void);
	virtual	~modalMgr(void);
	
	virtual	modalLink*	addLink(modal* inModal);
	virtual	void 			idle(void);
	
				int	count;
};


extern modalMgr	ourModalMgr;

#endif