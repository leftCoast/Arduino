#ifndef stlObj_h
#define stlObj_h

#include <triDBase.h>
#include <stlFile.h>
#include <triDVector.h>
#include <lists.h>
#include <drawObj.h>


class stlObj;

class triDObj {

	public:
				triDObj(void);
	virtual	~triDObj(void);
	
	virtual	triDPoint	getCenter(void);
	virtual	void			setRotation(triDRotation* inAngle);
	
				triDRotation	angle;
};


class triangle :	public triDObj {

	public:
				triangle(triDPoint* inCorner1,triDPoint* inCorner2,triDPoint* inCorner3);
	virtual	~triangle(void);
	
	virtual	triDPoint	getCenter(void);
};


class space :	public triDObj {

	public:
				space(void;
	virtual	~space(void);
	
				void			addPt(triDPoint* inPt);
				void			clearSpace(void);
	virtual	triDPoint	getCenter(void);
	
				bool		noX;
				bool		noY;
				bool		noZ;
				float		xMin;
				float		xMax;
				float		yMin;
				float		yMax;
				float		zMin;
				float		zMax;
};


//****************************************************************************************
// indexItem:
//
// The user should not interact with this class directly. indexItem is used by facetList
// to track facets in the .stl file.
//
// Why is this?
//
// We can't keep the facet list in memory. It can be just too big! So, to help, the
// facetList class goes though the file and prunes out the facets the user can't possibly
// see. The remaining facets are stored only by their index in the .stl file. And, this
// list of indexes is sorted by their distance from the user's eye. An indexItem is one of
// the links of this list.
//****************************************************************************************


class indexItem : public linkListObj {

	public:
				indexItem(int32_t index,float distance);
	virtual	~indexItem(void);
	
	int32_t	getIndex(void);
	
				int32_t	facetIndex;
				float		zDistance;
};



//****************************************************************************************
// facetList
//
// The user should not interact with this class directly. This is used by the strObj class
// to set up the orientation of the 3D object for rendering. Prunes and sorts the facet
// list for drawing.
//
// On creation it will find and store number of facets and the the min & max of all
// facets.
//
// With the min/max info it can calculate the center of the space the 3D object
// resides in.
// 
// Once all the orientation info. is in, it can calculate the possible visible list.
//
// Possible visible list is sorted from back to front.
//
// Each visible facet can be read out, in order, with all the transformations complete in
// the drawObj's local coordinates.
//
//****************************************************************************************


class facetList : public linkList,
						public stlFile {
	public:
				facetList(char* stlPath);
	virtual	~facetList(void);
	
				void				setCamera(triDVector* inCamera);
				void				setScale(float scale);
				void				setObjOrent(triDPoint* inObjLoc,triDRotation* inObjAngle);
				bool				createVisList(void);								// Build the list of visible facets.
				int32_t			getNumVisFacets(void);							// How many visible facets are on the list?
				triDFacet		getVisFacet(int32_t index);					// Get this faces off the visible list. (visible list index)
				
	protected:
	
	virtual	STLFacet			getFacet(int32_t index);
				void				clearObjSpan(void);
				void				addPtToObjSpan(float x,float y,float z);
				void				addToObjSpan(STLFacet* aFacet);
				void				locateOrigin(void);
				void				addIndexItem(indexItem* newItem);
				bool				inView(STLFacet* aFacet);
				float				getDistance(STLFacet* aFacet);
	
				bool				init;				// We ready to go?
				bool				haveList;		// Is list current to orientation?
				triDPoint		origin;			// The center point of the 3D object
				triDPoint		objLoc;			// The x,y,z offset from the top right corner of the view.
				triDRotation	objAngle;		// The pitch(x) yaw(y) roll(z) around the origin point of the 3D object.
				float				scale;			// Object scale.
				triDSpace		objSpan;			// The retangular solid that the 3D object fits into.
				triDVector		camera;			// The camera location.
};



//****************************************************************************************
// stlObj
//
// This is the class the user uses to place a 3D object in the view list to be draw on the
// screen. It stores all the information needed to setup the drawing environment. Ambient
// light, directed (spot) light. frame size and location on display.
//
// This also contains functions for the setup of the object itself for rendering. Location
// in the frame, orientation to the frame, scale for drawing.
//****************************************************************************************


class stlObj :	public drawObj {
					
	public:
				stlObj(int inX,int inY,int inWidth,int inHeight,const char* stlPath);
				stlObj(rect* inRect,const char* stlPath);
	virtual	~stlObj(void);
				
				void			begin(void);
				void			setAmbiantlight(colorObj* inColor);
				void			setlightSource(triDPoint* lightLoc,colorObj* color);
				void			setObjOrent(triDPoint* inObjLoc,triDRotation* inObjAngle);
				void			setScale(float inScale);
				
				triDFacet	getTriDFacet(int32_t index);
	virtual	void			drawSelf(void);
	
	protected:
				void			calcLightMapper(void);
				colorObj		calcColor(triDFacet* aFacet);
				void			scaleValues(triDFacet* aFacet,float scale);
				
				bool				init;
				colorObj			ambientColor;
				triDVector		spotPos;
				colorObj 		spotColor;
				colorMultiMap	lightMapper;
				facetList*		indexList;
};



#endif