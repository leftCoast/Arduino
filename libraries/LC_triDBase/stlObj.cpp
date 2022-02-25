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

	
viewFacet loadViewFacet(STLFacet* fileFacet) {

	viewFacet	res;
	
	res.normalVect.setVector(fileFacet->normVect[0],fileFacet->normVect[1],fileFacet->normVect[2]);
	
	res.original.corners[0].x = fileFacet->vertex1[0];
	res.original.corners[0].y = fileFacet->vertex1[1];
	res.original.corners[0].z = fileFacet->vertex1[2];
	
	res.original.corners[1].x = fileFacet->vertex2[0];
	res.original.corners[1].y = fileFacet->vertex2[1];
	res.original.corners[1].z = fileFacet->vertex2[2];
	
	res.original.corners[2].x = fileFacet->vertex3[0];
	res.original.corners[2].y = fileFacet->vertex3[1];
	res.original.corners[2].z = fileFacet->vertex3[2];
	
	res.cornerA.x = 0;
	res.cornerA.y = 0;
	
	res.cornerB.x = 0;
	res.cornerB.y = 0;
	
	res.cornerC.x = 0;
	res.cornerC.y = 0;
	
	return res;
}


// Move your result  triangle by "this much".
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
	
	
void objCenter::addSTLFacet(STLFacet* fileFacet) {

	double	x;
	double	y;
	double	z;
	
	x = fileFacet->vertex1[0];
	y = fileFacet->vertex1[1];
	z = fileFacet->vertex1[2];
	addItem(x,y,z);
	x = fileFacet->vertex2[0];
	y = fileFacet->vertex2[1];
	z = fileFacet->vertex2[2];
	addItem(x,y,z);
	x = fileFacet->vertex3[0];
	y = fileFacet->vertex3[1];
	z = fileFacet->vertex3[2];
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


indexItem::indexItem(uint32_t index,double distance)
	: linkListObj() {
	
	facetIndex	= index;
	zDistance	= distance;
}
	
indexItem::~indexItem(void) {  }


uint32_t indexItem::getIndex(void) {  return facetIndex; }



//****************************************************************************************


triDEngine::triDEngine(void)
	: linkList(),
	stlFile() {
	
	init		= false;
	haveList	= false;
	currItem	= NULL;
}
					
					
triDEngine::~triDEngine(void) { }


// Do the things we hold off 'till the code starts. Reading how many facets we have and
// calculating the center point of our 3D object.
bool triDEngine::begin(const char* stlPath) {

	STLFacet		fileFacet;
	objCenter	objSpan;
	
	setPath(stlPath);										// Load up the file.
	if (checkFile()) {									// Can we open and read this file? Make sense?
		if (getNumFacets()) {							// Read and remember the valid number of Facets from the file..
			for(uint32_t i=0;i<numFacets;i++) {		// For every facet..
				fileFacet = getFacet(i);				// Grab a facet.
				objSpan.addSTLFacet(&fileFacet);		// Add its info to the objCenter Obj.
			}													//
			init = true;									// And we'll call that a success!
		}														//
	}															//
	return init;											// return our result.							
}


// Run down the list of facets in the file and only save those that are possibly in view.
// Keeping them sorted as to depth as they come in.	
bool triDEngine::createList(renderSetup* setup) {

	indexItem*	newItem;
	double		zDist;
	STLFacet		fileFacet;
	
	if (init) {
		dumpList();
		haveList = false;
		for(uint32_t i=0;i<numFacets;i++) {
			fileFacet = getFacet(i);
			zDist = inView(setup,&fileFacet);
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
	
	STLFacet		fileFacet;										// The facet we can get.
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
			fileFacet = getFacet(anItem->facetIndex);		// Grab a Facet facet from the file.
			doTransformations(setup,&fileFacet);			// Do the flip and move all around thing.
			aFacet = calcViewFacet(setup,&fileFacet);		// Transform the 3D facet to 2 Space.
		}																//
	}																	//
	return aFacet;													// And we return our stuffed triangle.
}
	
	
				
// protected

	
	
// I have a completely ready to go STLFacet. I have the screen to render it on. I have the
// camera location to get angles. Map the x,y, points using the z locatrion to the screen.
viewFacet  triDEngine::calcViewFacet(renderSetup* setup,STLFacet* fileFacet) {
	
	viewFacet	aFacet;
	int			x;
	int			y;
	
	aFacet = loadViewFacet(fileFacet);
	
	ptMapper.setValues(fileFacet->vertex1[2],setup->camera.z,fileFacet->vertex1[0],setup->camera.x);
	x = round(ptMapper.map(0));
	ptMapper.setValues(fileFacet->vertex1[2],setup->camera.z,fileFacet->vertex1[1],setup->camera.y);
	y = round(ptMapper.map(0));
	aFacet.cornerA.x = x;
	aFacet.cornerA.y = y;
		
	ptMapper.setValues(fileFacet->vertex2[2],setup->camera.z,fileFacet->vertex2[0],setup->camera.x);
	x = round(ptMapper.map(0));
	ptMapper.setValues(fileFacet->vertex2[2],setup->camera.z,fileFacet->vertex2[1],setup->camera.y);
	y = round(ptMapper.map(0));
	aFacet.cornerB.x = x;
	aFacet.cornerB.y = y;
	
	ptMapper.setValues(fileFacet->vertex3[2],setup->camera.z,fileFacet->vertex3[0],setup->camera.x);
	x = round(ptMapper.map(0));
	ptMapper.setValues(fileFacet->vertex3[2],setup->camera.z,fileFacet->vertex3[1],setup->camera.y);
	y = round(ptMapper.map(0));
	aFacet.cornerC.x = x;
	aFacet.cornerC.y = y;

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
void triDEngine::setScale(renderSetup* setup,STLFacet* fileFacet) {

	fileFacet->vertex1[0] = fileFacet->vertex1[0] * setup->scale;
	fileFacet->vertex1[1] = fileFacet->vertex1[1] * setup->scale;
	fileFacet->vertex1[2] = fileFacet->vertex1[2] * setup->scale;
	
	fileFacet->vertex2[0] = fileFacet->vertex2[0] * setup->scale;
	fileFacet->vertex2[1] = fileFacet->vertex2[1] * setup->scale;
	fileFacet->vertex2[2] = fileFacet->vertex2[2] * setup->scale;
	
	fileFacet->vertex3[0] = fileFacet->vertex3[0] * setup->scale;
	fileFacet->vertex3[1] = fileFacet->vertex3[1] * setup->scale;
	fileFacet->vertex3[2] = fileFacet->vertex3[2] * setup->scale;
}

void triDEngine::setRotation(renderSetup* setup,STLFacet* fileFacet) {

	triDVector	aVect;
	triDVector	bVect;
	triDVector	nVect;
	triDPoint	ptA;
	triDPoint	ptB;
	triDPoint	ptC;
	
	aVect.setVector(fileFacet->vertex1[0],fileFacet->vertex1[1],fileFacet->vertex1[2]);
	aVect.rotateVect(&(setup->orientation));
	fileFacet->vertex1[0] = aVect.getX();
	fileFacet->vertex1[1] = aVect.getY();
	fileFacet->vertex1[2] = aVect.getZ();
	
	aVect.setVector(fileFacet->vertex2[0],fileFacet->vertex2[1],fileFacet->vertex2[2]);
	aVect.rotateVect(&(setup->orientation));
	fileFacet->vertex2[0] = aVect.getX();
	fileFacet->vertex2[1] = aVect.getY();
	fileFacet->vertex2[2] = aVect.getZ();
	
	aVect.setVector(fileFacet->vertex3[0],fileFacet->vertex3[1],fileFacet->vertex3[2]);
	aVect.rotateVect(&(setup->orientation));	
	fileFacet->vertex3[0] = aVect.getX();
	fileFacet->vertex3[1] = aVect.getY();
	fileFacet->vertex3[2] = aVect.getZ();
	
	ptA.x = fileFacet->vertex1[0];
	ptA.y = fileFacet->vertex1[1];
	ptA.z = fileFacet->vertex1[2];
	
	ptB.x = fileFacet->vertex2[0];
	ptB.y = fileFacet->vertex2[1];
	ptB.z = fileFacet->vertex2[2];
	
	ptC.x = fileFacet->vertex3[0];
	ptC.y = fileFacet->vertex3[1];
	ptC.z = fileFacet->vertex3[2];
	
	aVect.setVector(&ptA,&ptB);
	bVect.setVector(&ptB,&ptC);
	nVect = aVect.crossProd(&bVect);
	nVect.normalize();
	fileFacet->normVect[0] = nVect.getX();
	fileFacet->normVect[1] = nVect.getY();
	fileFacet->normVect[2] = nVect.getZ();
}


void triDEngine::setLocation(renderSetup* setup,STLFacet* fileFacet) {

	fileFacet->vertex1[0] += setup->location.x;
	fileFacet->vertex1[1] += setup->location.y;
	fileFacet->vertex1[2] += setup->location.z;
	
	fileFacet->vertex2[0] += setup->location.x;
	fileFacet->vertex2[1] += setup->location.y;
	fileFacet->vertex2[2] += setup->location.z;
	
	fileFacet->vertex3[0] += setup->location.x;
	fileFacet->vertex3[1] += setup->location.y;
	fileFacet->vertex3[2] += setup->location.z;
}


void triDEngine::doTransformations(renderSetup* setup,STLFacet* fileFacet) {
	
	setRotation(setup,fileFacet);
	setScale(setup,fileFacet);
	setLocation(setup,fileFacet);
	
}


// We check to see if this facet is facing the viewer or away from the viewer. We don't
// draw ones that are facing away.
double triDEngine::inView(renderSetup* setup,STLFacet* fileFacet) {

	triDVector		facetNormal;
	triDVector		cameraVect;
	viewFacet 		theFacet;
	triDPoint		midPt;
	double				maxDist;
	
	maxDist = 0;
	doTransformations(setup,fileFacet);
	facetNormal.setVector(fileFacet->normVect[0],fileFacet->normVect[1],fileFacet->normVect[2]);
	theFacet = loadViewFacet(fileFacet);
	midPt = getCentPt(&(theFacet.original));
	cameraVect.setVector(&midPt,&(setup->camera));
	//cameraVect.scaleBy(-1);
	if (cameraVect.dotProduct(&facetNormal)>0) {
		maxDist = fileFacet->vertex1[2];
		maxDist = max(maxDist,fileFacet->vertex2[2]);
		maxDist = max(maxDist,fileFacet->vertex3[2]);
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
	
	ourFacetList.begin(savedPath);				// Load up the path into the triDEngine.
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
		
				