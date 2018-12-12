#ifndef lists_h
#define lists_h

#include <Arduino.h>	// Do we need this here?


// **********************
// Your basic single linked list. Good for lists and stacks.
// One issue is that the list objects can not unlink themselves.
// This is because they have no idea who points at them. 
// So, unlike the double linked list, a list owner must manage 
// the list objects.
// **********************

// This is the base for the things in the list..
class linkListObj {
  
  public:
    			linkListObj(void);
    virtual	~linkListObj(void);
    
    virtual	void 				linkAfter(linkListObj* present);	// Given a pointer to a node, link yourself after it.
    virtual	void 				linkToEnd(linkListObj* present);	// Given a pointer to a node, link yourself after the last in the chain.
    virtual linkListObj*	getNext(void);							// Pass back the next pointer.
    virtual	void				setNext(linkListObj* ptr);			// Point somewhere else.
    virtual void				deleteTail(void);						// Call delete on everyone hooked to us.
  
  //protected : 		// Protected seems broken. 
    linkListObj* next;
};


// This is the manager for a list.
class linkList {
  
  public:
    			linkList(void);
    virtual	~linkList(void);
    
    virtual void         	addToTop(linkListObj* newObj);
    virtual void         	addToEnd(linkListObj* newObj);
    virtual void         	unlinkTop(void);						// Push off the first one.
    virtual void         	unlinkObj(linkListObj* oldObj);	// Find it and push it off.
    virtual	void		 		dumpList(void);
    virtual bool				isEmpty(void);
    virtual linkListObj*	getList(void);
    
    		int			 		getCount(void);
    		linkListObj* 		getByIndex(int index);										
 
 protected :           
    linkListObj* theList;
};


//*******************************************************
// stack
// In the stack we have push, pop, top & empty.
//*******************************************************

class stack : public linkList {

  public:
    			stack(void);
    virtual	~stack(void);
    
    virtual void         push(linkListObj* newObj);
    virtual void         pop(void);
    virtual linkListObj* top(void);
    virtual bool	     empty(void);
  };


//*******************************************************
// queue
// Just like in the stack, in the queue we have push, pop, 
// top & empty. But, when we add an object we don't
// add it to the top of the list, Like the stack. We 
//  add it to the end.
//*******************************************************

class queue : public linkList {

  public:
    			queue(void);
    virtual	~queue(void);
    
    virtual void         	push(linkListObj* newObj);
    virtual void         	pop(void);
    virtual linkListObj* 	top(void);
    virtual bool				empty(void);
  };


//*******************************************************
// Double linked list. Handy if you'd like to traverse both
// ways. Also handy 'cause it can be self managing.
//*******************************************************

class dblLinkListObj {
    
public:
    			dblLinkListObj(void);
	virtual	~dblLinkListObj(void);
    
    			void					linkAfter(dblLinkListObj* present); 	// Given a pointer to a node, link yourself after it.
    			void					linkBefore(dblLinkListObj* present); 	// Given a pointer to a node, link yourself before it.
    			dblLinkListObj*	getFirst(void);
    			dblLinkListObj*	getLast(void);
    			void					linkToEnd(dblLinkListObj* present);		// Given a pointer to a node, link yourself after the last in the chain.
    			void					linkToStart(dblLinkListObj* present);	// Given a pointer to a node, link yourself before the first in the chain.
    			void					unhook(void);
    			
    			dblLinkListObj*	dllPrev;
    			dblLinkListObj*	dllNext;
};

#endif
