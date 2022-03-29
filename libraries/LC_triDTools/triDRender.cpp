#include <triDRender.h>

#include <debug.h>




			
				




//****************************************************************************************
// offset2DFacet
//
// Move your result triangle by "this much".
//****************************************************************************************


void offset2DFacet(viewFacet* aFacet,int x,int y) {

	for (byte i=0;i<3;i++) {
		aFacet->corner[i].x += x;
		aFacet->corner[i].y += y;
	}
}

	
void printViewFacet(viewFacet* aFacet) {
	
	Serial.print("Corner A: (");
	Serial.print(aFacet->corner[0].x);Serial.print(", ");Serial.print(aFacet->corner[0].y);Serial.println(")");
	
	Serial.print("Corner B: (");
	Serial.print(aFacet->corner[1].x);Serial.print(", ");Serial.print(aFacet->corner[1].y);Serial.println(")");
	
	Serial.print("Corner B: (");
	Serial.print(aFacet->corner[2].x);Serial.print(", ");Serial.print(aFacet->corner[2].y);Serial.println(")");
	Serial.println("---------------------------");
}



//****************************************************************************************
// indexItem:
//
//****************************************************************************************


indexItem::indexItem(long index,double distance)
	: linkListObj() {
	
	facetIndex	= index;
	zDistance	= distance;
}
	
indexItem::~indexItem(void) {  }


long indexItem::getIndex(void) {  return facetIndex; }



//****************************************************************************************
// triDRender:
//
//****************************************************************************************


triDRender::triDRender(int inX,int inY,int inWidth,int inHeight)
	: linkList(),
	drawObj(inX,inY,inWidth,inHeight) {
	
	init		= false;
	ourModel	= NULL;
}
	
	
triDRender::triDRender(rect* inRect)
	: linkList(),
	drawObj(inRect) {
	
	init		= false;
	ourModel	= NULL;
}
	
	
triDRender::~triDRender(void) { if (ourModel) delete ourModel; }
				

bool triDRender::begin(facetList* inModel) {
	
	if (inModel) {
		ourModel = inModel;
		if (ourModel->begin()) {
			setupChange	= false;						// Cause' really there's nothing useful here, just defaults.
			//ambientColor.setColor(LC_CHARCOAL);	// Grey is a good ambiant color to start with.
			ambientColor.setColor(LC_BLACK);
			spotPos.x	= -100;							// Default location of the spot light.
			spotPos.y	= -200;
			spotPos.z	= -200;
			spotColor.setColor(&white);			// Default spot light color.
			calcLightMapper();						// lIghts are all  We can do the light mapper now.
			location.x			= width/2;			// Set the side to side location of the 3D object in our rect.
			location.y			= height/2;			// Set the height location of the 3D object in our rect.
			location.z			= 220;				// Set the z location. Now, bigger numbers are further "back".
			orientation.xRad	= 0;					// Set PITCH angle of the 3D object.
			orientation.yRad	= 0;					// Set YAW angle of the 3D object.
			orientation.zRad	= 0;					// Set ROLL angle of the 3D object.
			adjLocation.x	= 0;						// Place holder for the final location given the rotation.
			adjLocation.y	= 0;
			adjLocation.z	= 0;
			scale					= 1;					// Multiplier for dimensions. Sets objet size.
			camera.x				= width/2;			// Camera, view point center of screen in x.
			camera.y				= height/2;			// Center of screen in y.
			camera.z				= -300;				// Back out of screen 300 pts. (Basically pixels.)
			init = true;								// And now, we are initialized.
		}
	}
	return init;
}


// This sets the ambiant light color and brightness. Basically the light you see in the
// shadows.
void triDRender::setAmbiantlight(colorObj* inColor) {

	ambientColor.setColor(inColor);
	calcLightMapper();
	setNeedRefresh();
}
	

// Sets the location of the spot/flood light. Think of this as an uncovered lightbulb.
void triDRender::setlightLoc(triDPoint* lightLoc) {

	spotPos = *lightLoc;
	calcLightMapper();
	setNeedRefresh();
}


// Set the color and brightness of the spot/flood light.
void triDRender::setlightColor(colorObj* color) {

	spotColor.setColor(color);
	calcLightMapper();
	setNeedRefresh();
}


// Set the scaling factor of the model.
void triDRender::setObjScale(double inScale) {

	scale = inScale;
	setupChange = true;
}


// Set the location in relation to our drawObj of our mdel.
void triDRender::setObjLoc(triDPoint* loc) {
	location = *loc;
	setupChange = true;
}


// Sets up the orientation of the model.
void triDRender::setObjAngle(triDRotation* angle) {

	orientation = *angle;
	setupChange = true;
}


// Sets position of the camera. Camera always points at the model. (So far)
void triDRender::setCamera(triDPoint* cam) {
	camera = *cam;
	setupChange = true;
}


// This is where it all gets put together. Can we actually draw this thing?
void triDRender::drawSelf(void) {

	viewFacet	aFacet;
	colorObj		aColor;
	bool			done;
	//colorMapper	rainbow(&green,&red);
	//mapper		percent;
	//long			count;
	//float			pecentVal;
	
	//percent.setValues(0,ourModel->getNumFacets()-1,0,100);
	//count = 0;
	if (!init) {
		screen->drawRect(this,&red);
		return;
	}
	if (ourModel->openList()) {
		if (setupChange) {
			if (createList()) {
				setupChange = false;
			}
		}
		resetList();
		done = false;
		do {
			aFacet = getNextViewFacet();
			if (aFacet.normalVect.isNullVector()) {
				done = true;
			} else {
				aColor = calcColor(&aFacet);
				//pecentVal = percent.map(count);
				//aColor = rainbow.map(pecentVal);
				offset2DFacet(&aFacet,x,y);
				screen->fillTriangle(&(aFacet.corner[0]),&(aFacet.corner[1]),&(aFacet.corner[2]),&aColor);
				//db.trace("Facet: ",(int)count,true);
				//count++;
			}
		} while(!done);
		ourModel->closeList();
		screen->drawRect(this,&yellow);
	}
}


	
	
//protected



// Build the list of visible facets.
bool triDRender::createList(void) {

	indexItem*	newItem;
	double		zDist;
	triDFacet	aFacet;
	long			numFacets;
	
	if (init) {													// If we have been initialized..
		linkList::dumpList();								// Dump any sorted index list we may have.
		haveList = false;										// We have no list now.
		numFacets = ourModel->getNumFacets();			// Grab the number of facets.
		setRotationOffset();									// Recalculate the location, after rotation.
		for(long i=0;i<numFacets;i++) {					// For each available facet..
			aFacet = ourModel->getTriDFacet(i);			// Grab a "raw" facet.
			zDist = inView(&aFacet);						// Do the transitions then return distance. (If any)
			if (zDist>0) {										// If distance is >0, it's possibly visible..
				newItem = new indexItem(i,zDist);		// Create a link to this facet.
				addIndexItem(newItem);						// Add it into the list sorted by distance to camera.
			}														//
		}															//
		haveList = true;										// We have a current list of facets ready to render.
	}																//
	return haveList;											// User may be too lazy to check, we'll pass it back as well.
}	


// Reset the viewFacet list for a new read.															
void triDRender::resetList(void) { currItem = NULL; }


// Get the next facet off the visible list. (visible list index)															
viewFacet triDRender::getNextViewFacet(void) {

	triDFacet	aTriDFacet;														// The facet we can get.
	indexItem*	anItem;															// The thing with the index in it.
	viewFacet	aFacet;															// The facet we want.
	
	aFacet.normalVect.setVector(0,0,0);										// A NULL normal can be flag for fail.
	if (haveList) {																// If we got a file and have a current list..
		if (currItem) {															// If the current item is pointing to an item..
			anItem = (indexItem*)currItem->getNext();						// Grab the item after that one.
		} else {																		// Else, pointing at NULL..
			anItem = (indexItem*)linkList::getFirst();					// Means we want the first one.
		}																				//
		currItem = anItem;														// Works for all cases. Auto resets on NULL.
		if (anItem) {																// If we got a valid item..
			aTriDFacet = ourModel->getTriDFacet(anItem->facetIndex);	// Grab a Facet facet from the file.
			doTransformations(&aTriDFacet);									// Do the flip and move all around thing.
			aFacet = calcViewFacet(&aTriDFacet);							// Transform the 3D facet to 2 Space.
		}																				//
	}																					//
	return aFacet;																	// And we return our stuffed triangle.
}

								
viewFacet triDRender::calcViewFacet(triDFacet* aFacet) {

	viewFacet	result;
	int			x;
	int			y;
	
	ptMapper.setValues(aFacet->facet.corners[0].z,camera.z,aFacet->facet.corners[0].x,camera.x);
	x = round(ptMapper.map(0));
	ptMapper.setValues(aFacet->facet.corners[0].z,camera.z,aFacet->facet.corners[0].y,camera.y);
	y = round(ptMapper.map(0));
	result.corner[0].x = x;
	result.corner[0].y = y;
		
	ptMapper.setValues(aFacet->facet.corners[1].z,camera.z,aFacet->facet.corners[1].x,camera.x);
	x = round(ptMapper.map(0));
	ptMapper.setValues(aFacet->facet.corners[1].z,camera.z,aFacet->facet.corners[1].y,camera.y);
	y = round(ptMapper.map(0));
	result.corner[1].x = x;
	result.corner[1].y = y;
	
	ptMapper.setValues(aFacet->facet.corners[2].z,camera.z,aFacet->facet.corners[2].x,camera.x);
	x = round(ptMapper.map(0));
	ptMapper.setValues(aFacet->facet.corners[2].z,camera.z,aFacet->facet.corners[2].y,camera.y);
	y = round(ptMapper.map(0));
	result.corner[2].x = x;
	result.corner[2].y = y;
	
	result.midPoint = getCentPt(&(aFacet->facet));
	result.normalVect = aFacet->getNormVect();
	
	return result;
}



void triDRender::addIndexItem(indexItem* newItem) {

	indexItem*	trace;																	
	if (isEmpty()) {																					// If this is the first one..
		addToTop(newItem);																			// Add it to the top.
	} else if (newItem->zDistance>((indexItem*)theList)->zDistance) {					// Else the new one is bigger..
		addToTop(newItem);																			// Add it to the top.
	} else {																								// Else, new item is smaller than the top..
		trace = (indexItem*)linkList::getFirst();												// Point trace at the top item.
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


void  triDRender::setRotationOffset(void) {

	triDVector	aVector;
	triDPoint	rotationCenter;
	triDPoint	newPt;
	
	rotationCenter = ourModel->getModelCenter();
	aVector.setVector(&rotationCenter);
	aVector.rotateVect(&orientation);
	newPt = aVector.getPoint();
	adjLocation.x = rotationCenter.x - newPt.x + location.x;
	adjLocation.y = rotationCenter.y - newPt.y + location.y;
	adjLocation.z = rotationCenter.z - newPt.z + location.z;
}


void triDRender::doTransformations(triDFacet* aFacet) {
	
	aFacet->rotate(&orientation);
	aFacet->offset(adjLocation.x,adjLocation.y,adjLocation.z);
	aFacet->scale(scale);
}


double triDRender::inView(triDFacet* aFacet) {

	triDVector		cameraVect;
	triDPoint		midPt;
	double			maxDist;
	
	maxDist = 0;
	doTransformations(aFacet);
	midPt = aFacet->getCenterPt();
	cameraVect.setVector(&midPt,&(camera));
	if (cameraVect.dotProduct(&(aFacet->normVect))>0) {
		maxDist = aFacet->facet.corners[0].z;
		maxDist = max(maxDist,aFacet->facet.corners[1].z);
		maxDist = max(maxDist,aFacet->facet.corners[2].z);
	}
	return maxDist;
}


// Given an angle to your eye from the spot/flood light off the model.. For now it's a
// mapping to  the angle of this. From a reflected spot to the ambiant light.
void triDRender::calcLightMapper(void) {

	lightMapper.clearMap();
	lightMapper.addColor(2*M_PI,&spotColor);
	lightMapper.addColor(M_PI/2.0,&ambientColor);
}


// Angles and transformations are done. We are left with this 2D triangle. Here's where we
// figure out what color to draw it.
colorObj triDRender::calcColor(viewFacet* aFacet) {

	double		deltaAngle;
	colorObj		aColor;
	triDVector	spotVect;
	
	spotVect.setVector(&spotPos,&(aFacet->midPoint));
	deltaAngle = spotVect.angleBetween(&(aFacet->normalVect));
	aColor = lightMapper.map(deltaAngle);
	return aColor;
}