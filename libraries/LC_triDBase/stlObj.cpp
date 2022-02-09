#include <stlObj.h>
//#include <math.h>

#include <debug.h>

//****************************************************************************************


indexItem::indexItem(int32_t index,float distance)
	: linkListObj() {
	
	facetIndex	= index;
	zDistance	= distance;
}
	
indexItem::~indexItem(void) {  }


int32_t indexItem::getIndex(void) { return facetIndex; }



//****************************************************************************************


facetList::facetList(stlObj* inObj)
	: linkList() {
	
	ourObj				= inObj;
	haveList				= false;
	init					= false;
	origin.x				= 0;
	origin.y				= 0;
	origin.z				= 0;
	rotation.xRad		= 0;
	rotation.yRad		= 0;
	rotation.zRad		= 0;
	location.x			= 0;
	location.y			= 0;
	location.z			= 0;
	camera.setStartpoint(0,0,-500);	// Distance is in pixels.
}
					
					
facetList::~facetList(void) { }

void facetList::begin(void) {

	locateOrigin();
	init = true;
}

// Run down the list of facets in the file and only save those that are possibly in view.
// Keeping them sorted as to depth as they come in.	
bool facetList::createVisList(void) {

	indexItem*	newItem;
	float			zDist;
	int32_t		numFacets;
	STLFacet		aFacet;
	bool			success;
	
	success = false;
	if (init) {
		for(int32_t i=0;i<numFacets;i++) {
			aFacet = ourObj->getFacet(i);
			if (inView(&aFacet)) {
				zDist = getDistance(&aFacet);
				newItem = new indexItem(i,zDist);
				addIndexItem(newItem);
			}
		}
		haveList = true;
		success = true;
	}
	return success;
}


// Set the current rotation around the origin point.
void facetList::setRotation(triDRotation* inRotation) {

	rotation = *inRotation;
	haveList = false;
}


// Set the current location of the origin point, in respect to our view..
void facetList::setLocation(triDPoint* inLocation) {

	location = *inLocation;
	haveList = false;
}


// We read the file and only grabbed the index of facets that "can be" visible. Return the
// number of facets that we kept.
int32_t facetList::getNumVisFacets(void) {

	if (haveList) {
		return getCount();
	} else {
		return 0;
	}
}


// Of our remaining visible list. Lets give back a facet triangle at index..
triDFacet facetList::getVisFacet(int32_t index) {

	triDFacet		aFacet;								// The facet we want.
	
	if (haveList) {										// If we got a file and have a current list..
		aFacet = ourObj->getTriDFacet(index);		// Grab a triD facet from the list.
	} else {													// Else, we're missing the file or list..
		aFacet.normVect.setStartpoint(0,0,0);		// A zero normal can be flag for fail.
		aFacet.normVect.setEndpoint(0,0,0);
	}
	return aFacet;											// And we return our stuffed triangle.
}
	
	
				
// protected


// This guy flips everything around to match the display coordinates. y & z are flipped.
// This is where ALL the info from the file comes through, so it ALL gets flopped here.
STLFacet facetList::getFacet(int32_t index) {

	STLFacet aFacet;
	aFacet = stlFile::getFacet(index);
	aFacet.normVect[1] = -aFacet.normVect[1];
	aFacet.normVect[2] = -aFacet.normVect[1];
	aFacet.vertex1[1] = -aFacet.vertex1[1];
	aFacet.vertex1[2] = -aFacet.vertex1[2];
	aFacet.vertex2[1] = -aFacet.vertex2[1];
	aFacet.vertex2[2] = -aFacet.vertex2[2];
	aFacet.vertex3[1] = -aFacet.vertex3[1];
	aFacet.vertex3[2] = -aFacet.vertex3[2];
	return aFacet;
}


void facetList::clearObjSpan(void) {

	objSpan.xMin	= 0;
	objSpan.xMax	= 0;
	objSpan.yMax	= 0;
	objSpan.yMin	= 0;
	objSpan.zMax	= 0;
	objSpan.zMin	= 0;
}


void facetList::addPtToObjSpan(float x,float y,float z) {

	if (x < objSpan.xMin) objSpan.xMin = x;
	else if (x > objSpan.xMax) objSpan.xMax = x;
	if (y < objSpan.yMin) objSpan.yMin = y;
	else if (y > objSpan.yMax) objSpan.yMax = y;
	if (z < objSpan.zMin) objSpan.zMin = z;
	else if (z > objSpan.zMax) objSpan.zMax = z;
}


void facetList::addToObjSpan(STLFacet* aFacet) {

	float x;
	float y;
	float	z;
	
	x = aFacet->vertex1[0];
	y = aFacet->vertex1[1];
	z = aFacet->vertex1[2];
	addPtToObjSpan(x,y,z);
	x = aFacet->vertex2[0];
	y = aFacet->vertex2[1];
	z = aFacet->vertex2[2];
	addPtToObjSpan(x,y,z);
	x = aFacet->vertex3[0];
	y = aFacet->vertex3[1];
	z = aFacet->vertex3[2];
	addPtToObjSpan(x,y,z);
}

// Locates the origin and copies the number of facets in the .stl file.
void facetList::locateOrigin(void) {
	
	STLFacet	aFacet;
	
	numFacets = ourObj->getNumFacets();
	clearObjSpan();
	for(uint32_t i=0;i<numFacets;i++) {
		aFacet = ourObj->getFacet(i);
		addToObjSpan(&aFacet);
	}
	origin.x = (objSpan.xMin+objSpan.xMax)/2.0;
	origin.y = (objSpan.yMin+objSpan.yMax)/2.0;
	origin.z = (objSpan.zMin+objSpan.zMax)/2.0;
}


// Add new index items, sorted deep to shallow z values.
void facetList::addIndexItem(indexItem* newItem) {

	indexItem*	trace;																	
	if (isEmpty()) {																					// If this is the first one..
		addToTop(newItem);																			// Add it to the top.
	} else if (newItem->zDistance>((indexItem*)theList)->zDistance) {					// Else the new one is bigger..
		addToTop(newItem);																			// Add it to the top.
	} else {																								// Else, new item is smaller than the top..
		trace = (indexItem*)getFirst();															// Point trace at the top item.
		while(trace->getNext()!=NULL) {															// While there IS a next item..
			if (((indexItem*)trace->getNext())->zDistance<=newItem->zDistance) {		// If the next item is smaller than the new item..
				newItem->linkAfter(trace);															// We link after trace. Between trace and next item.
				return;																					// And we just bolt!
			}																								//
			trace = ((indexItem*)trace->getNext());											// We jump to the next item, 'cause we didn't bolt.
		}																									// Never found a smaller one and we're looking at the end.
		newItem->linkAfter(trace);																	// Link to the end.
	}
}


// We check to see if this facet is facing the viewer or away from the viewer. We don't
// draw ones that are facing away.
bool facetList::inView(STLFacet* aFacet) {

	triDVector	facetNormal;
	float					angle;
	
	facetNormal.setEndpoint(aFacet->normVect[0],aFacet->normVect[1],aFacet->normVect[2]);
	angle = camera.angleBetween(&facetNormal);
	return angle <= M_PI/2.0;
}


// We check all three corners of the triangle for the max value of z. This is the distance
// from the viewer we're looking for. Used to sort from back to front for drawing.
float facetList::getDistance(STLFacet* aFacet) {
	
	float maxDist;
	
	maxDist = aFacet->vertex1[2];
	maxDist = max(maxDist,aFacet->vertex2[2]);
	maxDist = max(maxDist,aFacet->vertex3[2]);
	return maxDist;
}


//****************************************************************************************
// stlObj:
//
//****************************************************************************************

stlObj::stlObj(int inX,int inY,int inWidth,int inHeight,const char* stlPath)
	: drawObj(inX,inY,inWidth,inHeight),
	stlFile(stlPath) { init = false; }
	
	
stlObj::stlObj(rect* inRect,const char* stlPath)
	: drawObj(inRect),
	stlFile(stlPath) { init = false; }
	

// Destructor, but nothing to delete.	
stlObj::~stlObj(void) {  }


// Setup all the initial and default values.
void stlObj::begin(void) {

	colorObj				aColor;
	triDPoint			lightLoc;
	triDPoint			location;
	triDRotation		angle;
	
	aColor.setColor(LC_GREY);
	setAmbiantlight(&aColor);
	
	lightLoc.x = 0;
	lightLoc.x = 0;
	lightLoc.x = -20;
	
	aColor.setColor(&blue);
	setlightSource(&lightLoc,&aColor);
	
	location.x = 0;
	location.y = 0;
	location.z = 0;
	
	angle.xRad = 0;
	angle.yRad = 0;
	angle.zRad = 0;
	setObjOrent(&location,&angle);
	
	indexList = new facetList(this);
	if(indexList) {
		openForBatchRead();
		indexList->begin();
		closeBatchRead();
	}
	
	init = true;
}


// Set the light that you see when there is no light source shining on whatever.	
void stlObj::setAmbiantlight(colorObj* inColor) {

	ambientColor.setColor(inColor);
	calcLightMapper();
	setNeedRefresh();
}


// At this moment we get one Point light source. Here is the location, and where its
// pointing.
void stlObj::setlightSource(triDPoint* inLightPt,colorObj* inColor) { 
	
	spotPos.setStartpoint(inLightPt);
	spotColor.setColor(inColor);
	calcLightMapper();
	setNeedRefresh();
}


void stlObj::setObjOrent(triDPoint* locatin,triDRotation* angle) { indexList->setObjOrent(locatin,angle); }
	
	
	
// The outside world will probably want a full facet. IE with normal vector.
triDFacet stlFile::getTriDFacet(int32_t index) {
	
	STLFacet		aFileFacet;
	triDFacet	ourFacet;
	
	aFileFacet = getFacet(index);
	ourFacet.normVect.setFreeVector(aFileFacet.normVect[0],aFileFacet.normVect[1],aFileFacet.normVect[2]);
	
	ourFacet.corners.corners[0].x = aFileFacet.vertex1[0];
	ourFacet.corners.corners[0].y = aFileFacet.vertex1[1];
	ourFacet.corners.corners[0].z = aFileFacet.vertex1[2];
	
	ourFacet.corners.corners[1].x = aFileFacet.vertex2[0];
	ourFacet.corners.corners[1].y = aFileFacet.vertex2[1];
	ourFacet.corners.corners[1].z = aFileFacet.vertex2[2];
	
	ourFacet.corners.corners[2].x = aFileFacet.vertex3[0];
	ourFacet.corners.corners[2].y = aFileFacet.vertex3[1];
	ourFacet.corners.corners[2].z = aFileFacet.vertex3[2];
	return ourFacet;
}


void stlObj::drawSelf(void) {
	
	uint32_t		numFacets;
	triDFacet	aFacet;
	colorObj		aColor;
	float 		scale;
	point			pt1;
	point			pt2;
	point			pt3;
	
	if (openForBatchRead()) {
		if (indexList->createVisList(this)) {
			numFacets = indexList->getNumVisFacets();
			scale = calcScale();
			for (uint32_t i=0;i<numFacets;i++) {
				aFacet = indexList->getVisFacet(this,i);
				aColor = calcColor(&aFacet);
				scaleValues(&aFacet,scale);
				pt1.x = round(aFacet.corners.corners[0].x);
				pt1.y = round(aFacet.corners.corners[0].y);
				pt2.x = round(aFacet.corners.corners[1].x);
				pt2.y = round(aFacet.corners.corners[1].y);
				pt3.x = round(aFacet.corners.corners[2].x);
				pt3.y = round(aFacet.corners.corners[2].y);
				screen->fillTriangle(&pt1,&pt2,&pt3,&aColor);
			}
		}
		closeBatchRead();
	}
}
	
	
float stlObj::calcScale(void) {
	
	triDSpace	limits;
	float			xScale;
	float			yScale;
	
	limits = indexList->getObjSpan();
	xScale = width/(limits.xMax-limits.xMin);
	yScale = height/(limits.yMax-limits.yMin);
	return min(xScale,yScale);
}


void stlObj::calcLightMapper() {
	
	lightMapper.clearMap();
	lightMapper.addColor(2*M_PI,&lightColor);
	lightMapper.addColor(M_PI/2.0,&ambiantLight);
}


colorObj  stlObj::calcColor(triDFacet* aFacet) {
	
	float		deltaAngle;
	colorObj aColor;
	
	deltaAngle = angleBetween(&lightDir,&(aFacet->normVect));
	aColor = lightMapper.map(deltaAngle);
	return aColor;
}


void  stlObj::scaleValues(triDFacet* aFacet,float scale) {
	
	aFacet->corners.corners[0].x *= scale;
	aFacet->corners.corners[0].y *= scale;
	aFacet->corners.corners[1].x *= scale;
	aFacet->corners.corners[1].y *= scale;
	aFacet->corners.corners[2].x *= scale;
	aFacet->corners.corners[2].y *= scale;
}
		
				