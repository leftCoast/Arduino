#ifndef lists_h
#define lists_h

#include <Arduino.h>


// **********************
// Your basic single linked list. Good for lists and stacks.
// One issue is that the list objects can not unlink themselves.
// This is because they have no idea who points at them. 
// So, unlike the double linked list, the list owner must manage 
// the list objects.
// **********************

class linkListObj {
  
  public:
    linkListObj(void);
    ~linkListObj(void);
    
    void linkAfter(linkListObj* present); // Given a pointer to a node, link yourself after it.
    void linkToEnd(linkListObj* present); // Given a pointer to a node, link yourself after the last in the chain.
    
    linkListObj* next;
};


class linkList {
  
  public:
    linkList(boolean doWeOwnThem);
    ~linkList(void);
    
    virtual void         addToTop(linkListObj* newObj);
    virtual void         addToEnd(linkListObj* newObj);
    virtual void         deleteFromTop(void);             // Push off or kill the first one.
    virtual void         deleteObj(linkListObj* oldObj);  // Find it and push it off. Maybe kill it?
    virtual boolean      isEmpty(void);
    virtual void         dumpList(void);
    virtual linkListObj* getList(void);
            
    linkListObj* theList;
    boolean      weOwn;
};


//*******************************************************
// stack
// In the stack we have push, pop, top, dump & empty.
// We also assume we own the objecs. So, when we pop one,
// we delete it. This is the most common way I've seen them
// used. If you want something else, there's enough legos
// here to make it up.
//*******************************************************

class stack : public linkList {

  public:
    stack(void);
    ~stack(void);
    
    virtual void         push(linkListObj* newObj);
    virtual void         pop(void);
    virtual linkListObj* top(void);
    virtual void         dump(void);
    virtual boolean      empty(void);
  };


//*******************************************************
// queue
// Just like in the stack, in the queue we have push, pop, 
// top, dump & empty. But, when we add an object we don't
// add it to the top of the list, we add it to the end. 
// Like the stack, we also assume we own the objecs. So,
// when we pop one, we delete it.
//*******************************************************

class queue : public linkList {

  public:
    queue(void);
    ~queue(void);
    
    virtual void         push(linkListObj* newObj);
    virtual void         pop(void);
    virtual linkListObj* top(void);
    virtual void         dump(void);
    //virtual boolean      empty(void);
  };


//*******************************************************
// Double linked list. Handy if you'd like to traverse both
// ways. Also handy 'cause it can be self managing.
// Deleting nodes will, first pull them from the list
// patching the hole, then delete them.
//*******************************************************

class dblLinkListObj {
    
public:
    dblLinkListObj(void);
    ~dblLinkListObj(void);
    
    
    void            linkAfter(dblLinkListObj* present); // Given a pointer to a node, link yourself after it.

    void            linkBefore(dblLinkListObj* present); // Given a pointer to a node, link yourself before it.
    dblLinkListObj* getFirst(void);
    dblLinkListObj* getLast(void);
    void            linkToEnd(dblLinkListObj* present); // Given a pointer to a node, link yourself after the last in the chain.
    void            linkToStart(dblLinkListObj* present); // Given a pointer to a node, link yourself before the first in the chain.
    void            unhook(void);
    
    dblLinkListObj* prev;
    dblLinkListObj* next;
};

#endif
