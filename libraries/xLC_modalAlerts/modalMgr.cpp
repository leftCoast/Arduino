#include <modalMgr.h>

//#include <debug.h>

modalMgr	ourModalMgr;

// *****************************************************
// *******************     modal    ********************
// *****************************************************


// Create a new modal object using a rectangle..
modal::modal(rect* inRect,eventSet inEventSet)
	: drawGroup(inRect,inEventSet) {  init(); }


// Create a new modal object using a the standard location & size values..	
modal::modal(int x, int y, int width,int height,eventSet inEventSet)
	: drawGroup(x,y,width,height,inEventSet) {  init(); }
	

// Here we are "Ensured" ourLink will be valid. Why? Because if the modal linked list is
// deleted, that will delete us first. So this should go tikkey boo. If we get deleted by
// someone else? The list is still there. So Again, no worries! Go unlink ourselves.
modal::~modal(void) {  if (ourLink) ourLink->ourModal = NULL; }


// We have different constructors so lets tie them together with a common init routine.
void modal::init(void) {

	drawing	= false;									// We're keeping quiet 'till we're ready.
	done		= false;									// We're not done yet. Heck, not even started.
	ourLink	= ourModalMgr.addLink(this);		// Tell the global modal manager that we have arrived.
}


// Do what it takes to not have things draw over you. (Like the button that created you.)
void  modal::checkIfReady(void) {

	if (!drawing) {						// Just in case some Bozo calls this..
		if (!ourEventMgr.active()) {	// If the event mgr. says its all quiet out there..
			drawing = true;				// We'll call this good. Allow drawing.
		}
	}
}


// We only draw when we're ready!
void	modal::draw(void) { 

	if (drawing) {
		drawGroup::draw();
	} else {
		checkIfReady();
	}
}
	
	
// Is this event for us? Well yes, they ALL are.
bool modal::acceptEvent(event* inEvent,point* locaPt) {

	drawGroup::acceptEvent(inEvent,locaPt);				// We do our "normal" stuff. (We are a drawGroup)
	return true;													// No Matter what, the buck stops here.
}



// *****************************************************
// ******************    modalLink   *******************
// *****************************************************


// Our modla link constructor..	
modalLink::modalLink(modal* inModal)
	: linkListObj() {
 
	ourModal		= inModal; 		// We're passed in the address of the modal we're tracking.
	viewList.addObj(inModal);	// And we add it to the global view list.
}
	

// Our destructor. If we still have a link to our modal, we delete that first. And that
// will cause our link to our modla to be NULLed out by the death of the modal. Shouldn't
// be an issue because we are not going to be around to acces it again anyway.
modalLink::~modalLink(void) {

	if (ourModal) {					// If we have a modal out there we're watching..
		ourModal->ourLink = NULL;	// We knock off its link to us. Keep stuff from getting weird.
		delete(ourModal);				// And NOW we delete the modal.
	}
}


// Returns if the modal is ready to be deleted, or has already been deleted.
bool modalLink::complete(void) {

	if (ourModal) return ourModal->done;	// If we have a modal, see if it's done.
	else return true;								// Else, no modal.. Then it IS done.
}



// *****************************************************
// *****************    ourModalMgr   ******************
// *****************************************************
	
	

modalMgr::modalMgr(void)
	:linkList(),
	idler(){  count = 0; }
	
	
modalMgr::~modalMgr(void) { }


// This is NOT called by the user. This is called by the modal. During the modal's
// constructor it calls this method to link itself into the modal manager's list. The
// modal manager creates a modalLink object to track this new modal. THEN the new modal
// gets a pointer the new modalLink object so it can unlink itself it someone deletes it.
//
// So..
//
// A) IF the modal completes normally. The list modalMgr picks up on this during idle and
//    deletes the modalLink object.
//
// B) The modalLink object, that we created here, then deletes the modal.
//
//    BUT..
//
// C) The modal may get deleted by something else. (Scary world out there.) WHEN it's
//    deleted, it NULLs out the modalLink Object's pointer to itself. Breaking the link
//    and casing its demise to be flagged.
//
// D) Hence : when the modalLink object deletes itself, it first checks to see if the link
//    to the modal is valid. If so, it will delete the modal. If not, it assumes someone
//    else did its job for it and goes on its merry way being deleted without re-deleting
//    the modal.
//
// The idea is that in this way, it doens't matter who deletes the modal. Both work fine.
//
// THE BOTTOM LINE? Don't call this yourself! Just create your modal and let this
// automatically be called in due time to link all the nonsense together.
modalLink* modalMgr::addLink(modal* inModal) {

	modalLink* newLink;
	
	hookup();									// Well, this is a good time to start idling.
	newLink = new modalLink(inModal);	// Create a new link item with this sparkling fresh modal item.
	addToTop(newLink);						// As it says, add this new link to the top of our modal link list.
	count++;
	return newLink;							// Pass back the new link to the calling object. (inModal) Then it can control it!
}


// Check through the list and see if any need to be deleted.
void modalMgr::idle(void) {
	
	modalLink* trace;

	if (count) {
		trace = (modalLink*)getFirst();				// Point at the top of the list.
		while(trace) {										// While we don't have a NULL pointer..
			if(trace->complete()) {						// If this node is complete.
				unlinkObj(trace);							// Unlink it from the list. (Leaving it.. "Alive"?)
				delete(trace);								// Recycle the memory.
				count--;
				return;										// And for now, we exit. Are there more? We'll get 'em next time.
			}
			trace = (modalLink*)trace->getNext();	// Jump to the nex on the list.
		}
	}
}
	
	

