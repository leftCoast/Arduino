#include "lists.h"



// *************** linkListObj ********************
// A none too bright node with a tail.

linkListObj::linkListObj(void) {  
  next = NULL; 
}

linkListObj::~linkListObj(void) {

  // Bit of trouble here. We are single linked, so we
  // have no idea who's pointing to us. Therefore we must
  // Trust whomever is killing us off to take care of
  // unlinking us from our list.
}


// We're given a pointer to one of our instances and told to link in after this guy.
// We can do that.
void linkListObj::linkAfter(linkListObj* present) {

  if (present) {              // Little sanity, don't link to NULL pointer.
    next = present->next;
    present->next = this;
  }
}


void linkListObj::linkToEnd(linkListObj* present) {

  if (present) {                      // If they're handing us NULL pointers, this is not our problem.
    while(present->next!=NULL) {      // There's more?!
      present = present->next;        // jump over to the next one!
    }                                 // We should be sitting on the last one now..
    present->next = this;             // link in!
  }
}


// There are times when its nice to just ask.
linkListObj* linkListObj::getNext(void) { return next; }



//********************* linkList *************************
// your basic linked list. Good base for linked list things, you know.

// When we recieve an object to link up. Do we own it? Or are
// we just tracking other people's laundry?
linkList::linkList(void) { theList = NULL; }


linkList::~linkList(void) { 
  // it seems that there is no way to delete
  // list items. Why? Because we don't know
  // what superclass they are. When we call
  // delete() on them everything crashes.
}


void linkList::addToTop(linkListObj* newObj) {

	/*
	Serial.print("addToTop list: ");
	Serial.print((unsigned long)this);
	Serial.print("  newObj: ");
	Serial.println((unsigned long) newObj);
	*/
	newObj->next = theList;  // Empty or not, it does the right thing.
	theList = newObj;
}


void linkList::addToEnd(linkListObj* newObj) {

  /*
   Serial.print("addToEnd list: ");
  Serial.print((unsigned long)this); 
  Serial.print("  newObj: "); 
  Serial.println((unsigned long) newObj);
   */
  if (theList==NULL) {          // No list huh?
    theList = newObj;
  } 
  else {
    newObj->linkToEnd(theList);  // Run along little object, go find your friends and link yourself in.
  }
}


void linkList::unlinkTop(void) {

  if (theList) {                  // if we have something there.
    theList = theList->next;      // unlink.
  }
}


void linkList::unlinkObj(linkListObj* oldObj) {

  linkListObj* temp;
  
  if (oldObj) {                      // They didn't hand us a NULL pointer.
    if(theList==oldObj) {            // Were poiting at it.
      theList = oldObj->next;        // unlink..
    }  else {                        // We ain't pointing at it..
      temp = theList;                // We're going to have to go look for it.
      while(temp->next!=oldObj && temp->next!=NULL) {
        temp = temp->next;
      }
      if (temp->next==oldObj) {      // Found it!
        temp->next = oldObj->next;   // unlink..
      }
    }
  }     
}


bool linkList::isEmpty(void) { return(theList == NULL); }


linkListObj* linkList::getList(void) { return (theList); }


int linkList::getCount(void) {

		linkListObj*	trace;
		long					count;
	
		count = 0;
		trace = theList;
		while(trace) {
			count++;
			trace = trace->next;
		}
		return count;
	}
	
	
// And there are times it would be nice to grab one by index.
// Like an array. Returns NULL if not found.
linkListObj* linkList::getByIndex(int index) {

	linkListObj*	trace;
	
	trace = theList;
	while(trace && index) {
		trace = trace->next;
		index--;
	}
	return trace;
}
	
// ********** stack ****************
// Your basic stack. Mostly pass throughs with the usual names.


stack::stack(void) : linkList() { }

stack::~stack(void) {  }

void stack::push(linkListObj* newObj) { addToTop(newObj); }

void stack::pop(void) { unlinkTop(); }

linkListObj* stack::top(void) { return getList(); }


// ********** queue ****************
// Your basic queue, we own 'em. Mostly pass throughs with the usual names.


queue::queue(void) : linkList() { }

queue::~queue(void) { }

void queue::push(linkListObj* newObj) { addToEnd(newObj); }

void queue::pop(void) { unlinkTop(); }

linkListObj* queue::top(void) { return getList(); }


// ********** double linked list ****************


dblLinkListObj::dblLinkListObj(void) {
    
    prev = NULL;
    next = NULL;
}


dblLinkListObj::~dblLinkListObj(void) { unhook(); }
    

// Given a pointer to a node, link yourself after it.
void dblLinkListObj::linkAfter(dblLinkListObj* present) {
    
    if (present) {
        next = present->next;
        prev = present;
        present->next = this;
    }
}


// Given a pointer to a node, link yourself before it.
void dblLinkListObj::linkBefore(dblLinkListObj* present) {
    
    if (present) {
        prev = present->prev;
        next = present;
        present->prev = this;
    }
}


dblLinkListObj* dblLinkListObj::getFirst(void) {
    
    dblLinkListObj* trace = this;
    while(trace->prev) {
        trace = trace->prev;
    }
    return trace;
}


dblLinkListObj* dblLinkListObj::getLast(void) {
    
    dblLinkListObj* trace = this;
    while(trace->next) {
        trace = trace->next;
    }
    return trace;
}


// Given a pointer to a node, link yourself after the last in the chain.
void dblLinkListObj::linkToEnd(dblLinkListObj* present) { if (present) linkAfter(present->getLast()); }


// Given a pointer to a node, link yourself before the first in the chain.
void dblLinkListObj::linkToStart(dblLinkListObj* present) { if (present) linkBefore(present->getFirst()); }


// Unlink us and, if in list, patch the hole.
void dblLinkListObj::unhook(void) {
    
    if (prev) prev->next = next;
    if (next) next->prev = prev;
    next = NULL;
    prev = NULL;
}


