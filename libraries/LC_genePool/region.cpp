#include "region.h"

// Create an empty region.
region::region(void)
	: linkList() { }


// Delete a region..
region::~region(void) { }


// Go through the tail nodes and knock out any that are a sub rect of this one.
void region::checkTail(regionRectObj* current) {

	regionRectObj* trace;
	regionRectObj* next;
	
	if (current) {										// Always check for NULL. They are NUTS out there!
		trace = current->getNextRectObj();				// trace starts and the next node.
		while(trace) {									// While trace is not NULL..
			next = trace->getNextRectObj();				// next get the next node after trace.
			if(trace->isSubRectOf(current)) {	// If trace is a subset of current..
				unlinkObj(trace);						// We unlink trace.
				delete(trace);							// Delete trace.
			}
			trace = next;								// In all cases we now point trace to next.
		}													// And start over.
	}
}


// We do what we need to do to add a rectangle to our region. Keeping enclosingRect up to
// date as we do it.
void region::addRect(rect* inRect) {

	regionRectObj*	newRectObj;
	regionRectObj*	trace;
	
	if (inRect) {																									// Sanity, if they really gave us a valid pointer..
		newRectObj = new regionRectObj(inRect);															// Create a shiny new rect object using inRect as template.
		if (newRectObj) {																							// If we got the new rectObj..
			if (isEmpty()) {																						// If this is our first rect..
				enclosingRect.setRect(inRect);																// Set the enclosing rect to inRect.
				addToTop(newRectObj);																			// We add it to the top of the list.
			} else {																									// Else NOT our first rect..
				enclosingRect.addRect(newRectObj);															// We add inRect to our enclosing rect.
				trace = getFirstRectObj();																		// Point at the first rect on the list.
				while(trace) {																						// While we are pointing to a non-NULL object..
					if (trace->getArea()>=newRectObj->getArea()) {										// If we are looking at a bigger rectangle than the new one..
						if (newRectObj->isSubRectOf(trace)) {												// If the new rect is a subset of the rect we are pointing at..
							delete(newRectObj);																	// No point to put this one in. Delete it.
							return;																					// Nothing left to do here. Bail.
						} else if (!trace->getNextRectObj()) {												// If we have no next node..
							newRectObj->linkAfter(trace);														// Tell the new node to link in after trace.
							return;																					// Again, nothing left to do here. Bail.
						} else if (trace->getNextRectObj()->getArea()<newRectObj->getArea()) {	// If the next one after trace has a smaller area than the new rect..
							newRectObj->linkAfter(trace);														// Link the new rect between the two.
							checkTail(newRectObj);																// Check the tail of the list for subrects and clean them out.
							return;																					// And that's it. Off we go.
						} else {																						// Else, the next node is not smaller than us..
							trace = trace->getNextRectObj();													// We move to the next node and start all over.
						}
					} else {																							// Else, the current node is smaller than us.. (Only can happen initial node.)
						addToTop(newRectObj);																	// Since its the initial node, we add this node to the top of the list.
						checkTail(newRectObj);																	// Then we do the tail check.
						return;																						// And our work is done.
					}	
				}
			}
		}
	}
}


// Add a region to our region. This only copies the inputed region. Does not change it in
// any way.
void region::addRegion(region* inRegion) {

	regionRectObj*	trace;
	
	if (inRegion) {
		trace = inRegion->getFirstRectObj();
		while(trace) {
			addRect(trace);
			trace = trace->getNextRectObj();
		}
	}
}
	

// Pass in a point and this will pass back if the point is contained in this regon or not.	
bool region::pointInRegion(point* inPoint) {

	regionRectObj*	trace;
	
	if (inPoint) {
		if (isEmpty()) {
			return false;
		}
		if (!enclosingRect.inRect(inPoint)) {
			return false;	
		}
		trace = getFirstRectObj();
		while(trace) {
			if (trace->inRect(inPoint)) {
				return true;
			}
			trace = trace->getNextRectObj();
		}
	}
	return false;
}


// Pass in a rect and this will pass back if the rect overlaps our region or not.
bool region::rectOverlapRegion(rect* inRect) {

	regionRectObj*	trace;
	
	if (inRect) {
		if (isEmpty()) {
			return false;
		}
		if (!enclosingRect.overlap(inRect)) {
			return false;	
		}
		trace = getFirstRectObj();
		while(trace) {
			if (trace->overlap(inRect)) {
				return true;
			}
			trace = trace->getNextRectObj();
		}
	}
	return false;
}
	
	
// Pass in a region and this will pass back if the two regions overlap or not.
bool region::regionOverlapRegion(region* inRegion) {

	regionRectObj*	traceA;
	regionRectObj*	traceB;
	rect				theirEnclosingRect;
	
	if (inRegion) {
		if (isEmpty()||inRegion->isEmpty()) {
			return false;
		}
		theirEnclosingRect = inRegion->getEnclosingRect();
		if (!enclosingRect.overlap(&theirEnclosingRect)) {
			return false;	
		}
		traceA = getFirstRectObj();
		while(traceA) {
			traceB = inRegion->getFirstRectObj();
			while(traceB) {
				if (traceA->overlap(traceB)) {
					return true;
				}
				traceB = traceB->getNextRectObj();
			}
			traceA = traceA->getNextRectObj();
		}
	}
	return false;
}


rect region::getEnclosingRect(void) { return enclosingRect; }


// Always pass back a regionRectObj pointer.
regionRectObj* region::getFirstRectObj(void) { return (regionRectObj*)linkList::getFirst(); }


// Create a new regionRectObj using an inputted rect.
regionRectObj::regionRectObj(rect* inRect)
	: linkListObj(),
	rect(inRect) { area = width * height; }
	
	
// Create a new regionRectObj using x,y width and height values.
regionRectObj::regionRectObj(int inX,int inY,int inWidth,int inHeight)
	: linkListObj(),
	rect(inX,inY,inWidth,inHeight) { area = width * height; }
	
	
// Your standard destructor..
regionRectObj::~regionRectObj(void) {  }


// Return the pre-calculated area.
long regionRectObj::getArea(void) { return area; }


// Always pass back a regionRectObj pointer.
regionRectObj* regionRectObj::getNextRectObj(void) { return (regionRectObj*)regionRectObj::getNext(); }


