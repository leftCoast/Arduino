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
    
    virtual	void 				linkAfter(linkListObj* present);			// Given a pointer to a node, link yourself after it.
    virtual	void 				linkToEnd(linkListObj* present);			// Given a pointer to a node, link yourself after the last in the chain.
    virtual linkListObj*	getNext(void);									// Pass back the next pointer.
    virtual	void				setNext(linkListObj* ptr);					// Point somewhere else.
    virtual void				deleteTail(void);								// Call delete on everyone hooked to us.
    virtual bool				isGreaterThan(linkListObj* compObj);	// Are we greater than the obj being passed in? Primary sorting function.
    virtual bool				isLessThan(linkListObj* compObj);		// Are we less than the obj being passed in? Primary sorting function.
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
    virtual void				unlinkTop(void);						// Push off the first one.
    virtual void         	unlinkObj(linkListObj* oldObj);	// Find it and push it off.
    virtual	void		 		dumpList(void);						// Call delete on everyone.
    virtual bool				isEmpty(void);
    virtual linkListObj*	getFirst(void);
    virtual linkListObj*	getLast(void);
    virtual linkListObj*	findMax(linkListObj* present);
    virtual linkListObj*	findMin(linkListObj* present);
    virtual	void				sort(bool decending);
    
    		int			 		getCount(void);
    		linkListObj* 		getByIndex(int index);										
 
 protected :           
    linkListObj* theList;
};


//*******************************************************
// stack
// In the stack we have push, pop, peek & isEmpty.
//*******************************************************

class stack : public linkList {

  public:
    			stack(void);
    virtual	~stack(void);
    
    virtual void				push(linkListObj* newObj);
    virtual linkListObj* 	pop(void);
    virtual linkListObj*	peek(void);	
  };


//*******************************************************
// queue
// Just like in the stack, in the queue we have push, peek,
// pop & isEmpty. But, when we add an object we don't
// add it to the top of the list. We add it to the end.
//*******************************************************

class queue : public linkList {

  public:
    			queue(void);
    virtual	~queue(void);
    
    virtual void         	push(linkListObj* newObj);
    virtual linkListObj*   pop(void);
    virtual	linkListObj* 	peek(void);
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
    			dblLinkListObj*	getTailObj(int index);						// Hand back the "nth" one of our tail. Starting at 0;
    			void					unhook(void);									// Unhook myself.
    			void					dumpTail(void);								// Delete entire tail.
    			void					dumpHead(void);								// Delete entire head section..
    			void					dumpList(void);								// Dump both head & tail.
    			int					countTail(void);								// How many nodes long is our tail?
    			int					countHead(void);								// How many nodes long is our head?
    			
    			dblLinkListObj*	dllPrev;
    			dblLinkListObj*	dllNext;
};

#endif
