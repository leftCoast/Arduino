#include <stlObj.h>
//#include <math.h>

#include <debug.h>

triDVector		normalVect;
	triDTriangle	original;
	point				cornerA;
	point				cornerB;
	point				cornerC;
	
	
void print2DPt(point* aPt) {

	Serial.print("Point x, y: ");
	Serial.print(aPt->x);
	Serial.print(", ");
	Serial.println(aPt->y);
}


viewFacet loadViewFacet(triDFacet* aFacet) {

	viewFacet	res;
	
	res.normalVect.setVector(&(aFacet.normVect));
	res.original.corners = aFacet->facet.corners;
	res.cornerA.x = 0;
	res.cornerA.y = 0;
	res.cornerB.x = 0;
	res.cornerB.y = 0;
	res.cornerC.x = 0;
	res.cornerC.y = 0;
	return res;
}


// Move your result triangle by "this much".
void offset2DFacet(viewFacet* aFacet,int x,int y) {

	aFacet->cornerA.x += x;
	aFacet->cornerA.y += y;
	
	aFacet->cornerB.x += x;
	aFacet->cornerB.y += y;
	
	aFacet->cornerC.x += x;
	aFacet->cornerC.y += y;
}


	
void printViewFacet(viewFacet* aFacet) {

	Serial.print("Normal vector x,y,z : ");
	aFacet->normalVect.printVector();
	Serial.println("Original facet : ");
	printTriDTriangle(&(aFacet->original));
	Serial.print("CornerA : ");
	print2DPt(&(aFacet->cornerA));
	Serial.print("CornerB : ");
	print2DPt(&(aFacet->cornerB));
	Serial.print("CornerC : ");
	print2DPt(&(aFacet->cornerC));
}


objCenter::objCenter(void) {  pointSet = false; }

				
objCenter::~objCenter(void) { }
	
	
void objCenter::addFacet(triDFacet* aFacet) {

	double	x;
	double	y;
	double	z;
	
	x = aFacet->facet.corners[0].x;
	y = aFacet->facet.corners[0].y;
	z = aFacet->facet.corners[0].z;
	addItem(x,y,z);
	x = aFacet->facet.corners[1].x;
	y = aFacet->facet.corners[1].y;
	z = aFacet->facet.corners[1].z;
	addItem(x,y,z);
	x = aFacet->facet.corners[2].x;
	y = aFacet->facet.corners[2].y;
	z = aFacet->facet.corners[2].z;
	addItem(x,y,z);
}


void objCenter::addItem(double x,double y,double z) {

	if (!pointSet) {
		xMin = x;
		xMax = x;
		yMin = y;
		yMax = y;
		zMin = z;
		zMax = z;
		pointSet = true;
	} else {
		if (x < xMin) xMin = x;
		else if (x > xMax) xMax = x;
		if (y < yMin) yMin = y;
		else if (y > yMax) yMax = y;
		if (z < zMin) zMin = z;
		else if (z > zMax) zMax = z;
	}
}


triDPoint objCenter::getCenterPt(void) {

	triDPoint	aCenter;
	
	aCenter.x = (xMin+xMax)/2.0;
	aCenter.y = (yMin+yMax)/2.0;
	aCenter.z = (zMin+zMax)/2.0;
	return aCenter;
}

	
void objCenter::printObjCenter(void) {

	triDPoint	aCenter;
	
	Serial.println("----------  objCenter  ----------");
	Serial.print("x min, max   : ");
	Serial.print(xMin);Serial.print(", ");
	Serial.print(xMax);Serial.println();
	Serial.print("y min, max   : ");
	Serial.print(yMin);Serial.print(", ");
	Serial.print(yMax);Serial.println();	
	Serial.print("z min, max   : ");
	Serial.print(zMin);Serial.print(", ");
	Serial.print(zMax);Serial.println();
	aCenter = getCenterPt();
	Serial.print("Center x,y,z : ");
	Serial.print(aCenter.x);Serial.print(", ");
	Serial.print(aCenter.y);Serial.print(", ");
	Serial.print(aCenter.z);Serial.println();
	Serial.println("---------------------------------");
}
			
				

//****************************************************************************************


indexItem::indexItem(long index,double distance)
	: linkListObj() {
	
	facetIndex	= index;
	zDistance	= distance;
}
	
indexItem::~indexItem(void) {  }


long indexItem::getIndex(void) {  return facetIndex; }



//****************************************************************************************


triDEngine::triDEngine(void)
	: linkList(),
	facetList() {
	
	init		= false;
	haveList	= false;
	currItem	= NULL;
}
					
					
triDEngine::~triDEngine(void) { }


// Do the things we hold off 'till the code starts. Reading how many facets we have and
// calculating the center point of our 3D object.
bool triDEngine::begin(void) {
	
	if (facetList::begin()) {		// If the facet list is happy..
		init = true;					// And we'll call that a success!
	}										//
	return init;						// return our result.
}


// Run down the list of facets in the file and only save those that are possibly in view.
// Keeping them sorted as to depth as they come in.	
bool triDEngine::createList(renderSetup* setup) {

	indexItem*	newItem;
	double		zDist;
	triDFacet	aFacet;
	long			numFacets;
	
	if (init) {
		dumpList();
		haveList = false;
		numFacets = getNumFacets();
		for(long i=0;i<numFacets;i++) {
			aFacet = getFacet(i);
			zDist = inView(setup,&aFacet);
			if (zDist>0) {
				newItem = new indexItem(i,zDist);
				addIndexItem(newItem);
			}
		}
		haveList = true;
	}
	return haveList;
}


// Its ok to just stamp a NULL here to "reset" the list. It don't "own" anything, Its
// just a pointer into a managed linked list.
void triDEngine::resetList(void) { currItem = NULL; }


// Of our remaining visible list. Lets give back the next facet..
viewFacet triDEngine::getNextViewFacet(renderSetup* setup) {
	
	triDFacet	aTriDFacet;										// The facet we can get.
	indexItem*	anItem;											// The thing with the index in it.
	viewFacet	aFacet;											// The facet we want.
	
	aFacet.normalVect.setVector(0,0,0);						// A NULL normal can be flag for fail.
	if (haveList) {												// If we got a file and have a current list..
		if (currItem) {											// If the current item is pointing to an item..
			anItem = (indexItem*)currItem->getNext();		// Grab the item after that one.
		} else {														// Else, pointing at NULL..
			anItem = (indexItem*)getFirst();					// Means we want the first one.
		}																//
		currItem = anItem;										// Works for all cases. Auto resets on NULL.
		if (anItem) {												// If we got a valid item..
			aTriDFacet = getFacet(anItem->facetIndex);	// Grab a Facet facet from the file.
			doTransformations(setup,&aTriDFacet);			// Do the flip and move all around thing.
			aFacet = calcViewFacet(setup,&aTriDFacet);	// Transform the 3D facet to 2 Space.
		}																//
	}																	//
	return aFacet;													// And we return our stuffed triangle.
}
	
	
				
// protected

	

// I have a completely ready to go triDFacet. I have the screen to render it on. I have the
// camera location to get angles. Map the x,y, points using the z locatrion to the screen.
viewFacet  triDEngine::calcViewFacet(renderSetup* setup,triDFacet* aTriDFacet) {
	
	viewFacet	aFacet;
	int			x;
	int			y;
	
	aTriDFacet
	ptMapper.setValues(aTriDFacet->facet.corners[0].z,setup->camera.z,fileFacet->vertex1[0],setup->camera.x);
	x = round(ptMapper.map(0));
	ptMapper.setValues(aTriDFacet->facet.corners[0].z,setup->camera.z,fileFacet->vertex1[1],setup->camera.y);
	y = round(ptMapper.map(0));
	aFacet.corner[0].x = x;
	aFacet.corner[0].y = y;
		
	ptMapper.setValues(aTriDFacet->facet.corners[1].z,setup->camera.z,fileFacet->vertex2[0],setup->camera.x);
	x = round(ptMapper.map(0));
	ptMapper.setValues(aTriDFacet->facet.corners[1].z,setup->camera.z,fileFacet->vertex2[1],setup->camera.y);
	y = round(ptMapper.map(0));
	aFacet.corner[1].x = x;
	aFacet.corner[1].y = y;
	
	ptMapper.setValues(fileFacet->vertex3[2],setup->camera.z,fileFacet->vertex3[0],setup->camera.x);
	x = round(ptMapper.map(0));
	ptMapper.setValues(fileFacet->vertex3[2],setup->camera.z,fileFacet->vertex3[1],setup->camera.y);
	y = round(ptMapper.map(0));
	aFacet.corner[2].x = x;
	aFacet.corner[2].y = y;

	return aFacet;
}


// Add new index items, sorted deep to shallow z values.
void triDEngine::addIndexItem(indexItem* newItem) {

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


// Scale seems to work.
void triDEngine::setScale(renderSetup* setup,triDFacet* aFacet) {

	aFacet->facet.corners[0].x = aFacet->facet.corners[0].x * setup->scale;
	aFacet->facet.corners[0].y = aFacet->facet.corners[0].y * setup->scale;
	aFacet->facet.corners[0].z = aFacet->facet.corners[0].z * setup->scale;
	
	aFacet->facet.corners[1].x = aFacet->facet.corners[1].x * setup->scale;
	aFacet->facet.corners[1].y = aFacet->facet.corners[1].y * setup->scale;
	aFacet->facet.corners[1].z = aFacet->facet.corners[1].z * setup->scale;
	
	aFacet->facet.corners[2].x = aFacet->facet.corners[2].x * setup->scale;
	aFacet->facet.corners[2].y = aFacet->facet.corners[2].y * setup->scale;
	aFacet->facet.corners[2].z = aFacet->facet.corners[2].z * setup->scale;
}


void triDEngine::setRotation(renderSetup* setup,triDFacet* aFacet) {

	triDVector	aVect;
	triDVector	bVect;
	triDVector	nVect;
	triDPoint	ptA;
	triDPoint	ptB;
	triDPoint	ptC;
	
	aVect.setVector(aFacet->facet.corners[0].x,aFacet->facet.corners[0].y,aFacet->facet.corners[0].z);
	aVect.rotateVect(&(setup->orientation));
	aFacet->facet.corners[0].x	= aVect.getX();
	aFacet->facet.corners[0].y	= aVect.getY();
	aFacet->facet.corners[0].z	= aVect.getZ();
	
	aVect.setVector(aFacet->facet.corners[1].x,aFacet->facet.corners[1].y,aFacet->facet.corners[1].z);
	aVect.rotateVect(&(setup->orientation));
	aFacet->facet.corners[1].x	= aVect.getX();
	aFacet->facet.corners[1].y	= aVect.getY();
	aFacet->facet.corners[1].z	= aVect.getZ();
	
	aVect.setVector(aFacet->facet.corners[2].x,aFacet->facet.corners[2].y,aFacet->facet.corners[2].z);
	aVect.rotateVect(&(setup->orientation));
	aFacet->facet.corners[2].x	= aVect.getX();
	aFacet->facet.corners[2].y	= aVect.getY();
	aFacet->facet.corners[2].z	= aVect.getZ();
	
	ptA.x = aFacet->facet.corners[0].x;
	ptA.y = aFacet->facet.corners[0].y;
	ptA.z = aFacet->facet.corners[0].z;
	
	ptA.x = aFacet->facet.corners[1].x;
	ptA.y = aFacet->facet.corners[1].y;
	ptA.z = aFacet->facet.corners[1].z;
	
	ptA.x = aFacet->facet.corners[2].x;
	ptA.y = aFacet->facet.corners[2].y;
	ptA.z = aFacet->facet.corners[2].z;
	
	aVect.setVector(&ptA,&ptB);
	bVect.setVector(&ptB,&ptC);
	nVect = aVect.crossProd(&bVect);
	nVect.normalize();
	aFacet->normVect.setVector(&nVect);
}


void triDEngine::setLocation(renderSetup* setup,triDFacet* aFacet) {

	aFacet->facet.corners[0].x += setup->location.x;
	aFacet->facet.corners[0].y += setup->location.y;
	aFacet->facet.corners[0].z += setup->location.z;
	
	aFacet->facet.corners[1].x += setup->location.x;
	aFacet->facet.corners[1].y += setup->location.y;
	aFacet->facet.corners[1].z += setup->location.z;
	
	aFacet->facet.corners[2].x += setup->location.x;
	aFacet->facet.corners[2].y += setup->location.y;
	aFacet->facet.corners[2].z += setup->location.z;
}


void triDEngine::doTransformations(renderSetup* setup,triDFacet* aFacet) {
	
	setRotation(setup,aFacet);
	setScale(setup,aFacet);
	setLocation(setup,aFacet);
}


// We check to see if this facet is facing the viewer or away from the viewer. We don't
// draw ones that are facing away.
double triDEngine::inView(renderSetup* setup,triDFacet* aFacet) {

	triDVector		facetNormal;
	triDVector		cameraVect;
	viewFacet 		theFacet;
	triDPoint		midPt;
	double				maxDist;
	
	maxDist = 0;
	doTransformations(setup,aFacet);
	facetNormal.setVector(aFacet->normVect);
	theFacet = loadViewFacet(aFacet);
	midPt = getCentPt(aFacet);
	cameraVect.setVector(&midPt,&(setup->camera));
	if (cameraVect.dotProduct(&facetNormal)>0) {
		maxDist = aFacet->facet.corners[0].z
		maxDist = max(maxDist,aFacet->facet.corners[1].z);
		maxDist = max(maxDist,aFacet->facet.corners[2].z);
	}
	return maxDist;
}




//****************************************************************************************
// stlObj:
//
//****************************************************************************************

stlObj::stlObj(int inX,int inY,int inWidth,int inHeight,const char* stlPath)
	: drawObj(inX,inY,inWidth,inHeight) {
	
	savedPath = NULL;
	heapStr(&savedPath,stlPath);
	init = false;
}
	
	
stlObj::stlObj(rect* inRect,const char* stlPath)
	: drawObj(inRect) {
	
	savedPath = NULL;
	heapStr(&savedPath,stlPath);
	init = false;
}
	

// Destructor, just in case no one else recycles the path..	
stlObj::~stlObj(void) {  freeStr(&savedPath); }


// Setup all the initial and default values.
void stlObj::begin(void) {
	
	ourFacetList.begin();							// Load up the path into the triDEngine.
	setupChange	= false;								// Cause' really there's nothing useful here, just defaults.
	ambientColor.setColor(LC_CHARCOAL);			// Grey is a good ambiant color to start with.
	spotPos.x	= 0;									// Default location of the spot light.
	spotPos.y	= 0;
	spotPos.z	= -200;
	spotColor.setColor(&white);					// Default spot light color.
	calcLightMapper();								// lIghts are all setup. We can do the light mapper now.
	setup.location.x			= width/2;		// Set the side to side location of the 3D object in our rect.
	setup.location.y			= height/2;	// Set the height location of the 3D object in our rect.
	setup.location.z			= 220;				// Set the z location. Now, bigger numbers are further "back".
	setup.orientation.xRad	= 0;					// Set PITCH angle of the 3D object.
	setup.orientation.yRad	= 0;					// Set YAW angle of the 3D object.
	setup.orientation.zRad	= 0;					// Set ROLL angle of the 3D object.
	setup.scale					= 1;					// Multiplier for dimensions. Sets objet size.
	setup.camera.x				= width/2;			// Camera, view point center of screen in x.
	setup.camera.y				= height/2;			// Center of screen in y.
	setup.camera.z				= -300;				// Back out of screen 300 pts. (Basically pixels.)
	setup.viewWidth			= width;				// Width of the drawObj that we are.
	setup.viewHeight			= height;			// Height of the drawObj that we are.
	init = true;										// And now, we are initialized.
}


// Set the light that you see when there is no light source shining on whatever.	
void stlObj::setAmbiantlight(colorObj* inColor) {

	ambientColor.setColor(inColor);
	calcLightMapper();
	setNeedRefresh();
}


// At this moment we get one Point light source. Here is the location, and where its
// pointing..
void stlObj::setlightLoc(triDPoint* lightLoc) {

	spotPos = *lightLoc;
	calcLightMapper();
	setNeedRefresh();
}


// And it's color.
void stlObj::setlightColor(colorObj* color) {

	spotColor.setColor(color);
	calcLightMapper();
	setNeedRefresh();
}
				
				
// Set the scale of the 3D model.	
void stlObj::setObjScale(double scale) {

	setup.scale = scale;
	setupChange = true;
}


// Set the location of the 3D model.
void stlObj::setObjLoc(triDPoint* loc) {

	setup.location.x	= loc->x;
	setup.location.y	= loc->y;
	setup.location.z	= loc->z;
	setupChange = true;
}


// Set the angle that the 3D modle is held.
void stlObj::setObjAngle(triDRotation* angle) {

	setup.orientation.xRad	= angle->xRad;
	setup.orientation.yRad	= angle->yRad;
	setup.orientation.zRad	= angle->zRad;
	setupChange = true;
}


// Set the location of the camera.
void stlObj::setCamera(triDPoint* cam) {

	setup.camera.x	= cam->x;
	setup.camera.y	= cam->y;
	setup.camera.z	= cam->z;
	setupChange = true;
}


void stlObj::drawSelf(void) {

	viewFacet	aFacet;
	colorObj		aColor;
	bool			done;
	
	screen->drawRect(this,&blue);
	if (ourFacetList.openForBatchRead()) {
		if (setupChange) {
			if (ourFacetList.createList(&setup)) {
				setupChange = false;
			}
		}
		ourFacetList.resetList();
		done = false;
		do {
			aFacet = ourFacetList.getNextViewFacet(&setup);
			if (aFacet.normalVect.isNullVector()) {
				done = true;
			} else {
				aColor = calcColor(&aFacet);
				offset2DFacet(&aFacet,x,y);
				screen->fillTriangle(&(aFacet.cornerA),&(aFacet.cornerB),&(aFacet.cornerC),&aColor);
			}
		} while(!done);
		ourFacetList.closeBatchRead();
		screen->drawRect(this,&yellow);
	}
}


void stlObj::calcLightMapper() {
	
	lightMapper.clearMap();
	lightMapper.addColor(2*M_PI,&spotColor);
	lightMapper.addColor(M_PI/2.0,&ambientColor);
}


// Calcualate the color to paint this one triangle.
colorObj  stlObj::calcColor(viewFacet* aFacet) {
	
	double		deltaAngle;
	colorObj		aColor;
	triDVector	spotVect;
	triDPoint	centerPt;
	
	centerPt = getCentPt(&(aFacet->original));
	spotVect.setVector(&spotPos,&centerPt);
	deltaAngle = spotVect.angleBetween(&(aFacet->normalVect));
	aColor = lightMapper.map(deltaAngle);
	return aColor;
}
		
				