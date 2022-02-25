#ifndef stlObj_h
#define stlObj_h

#include <triDBase.h>
#include <stlFile.h>
#include <triDVector.h>
#include <lists.h>
#include <drawObj.h>
#include <strTools.h>

class stlObj;

//****************************************************************************************
// renderSetup: (used internally)
//
// Render setup is the information needed to calculate the set of visable viewFacets from
// a list of triDFacets.  In order to do that we need to "see" just what the viewer "sees".
// 
// The origin is the location, in three space, of of where to locate the centner of the 3D
// object. The location of the object, as it comes to us, is undefined. So placing this
// point in the center of the view and back some, will at least get you something you can
// see.
//
// Orientation is the yaw(x), roll(y), pitch(z) of the object around its center point.
// These values are in radians.  IE 360 deg = 2Pi radians etc.
// 
// Scale is a multiplier of the given point values. The size of the object is also
// undefined. So you'll need the scaler to size it to a resonable size. (And who knows
// what that'll be.)
//
// The camera point is where the viewer's eye is thought to be. Typically, middle of the
// view and a large (Maybe -300 ish) set back in the z axes. This gives the perspective of
// the resulting image.
//
// viewWidth and viewHeight These give the bounding rectangle that the user is viewing the
// resulting object through. Used for clipping facets that can't be seen. But more
// importantly, used as the 2 dimensional plane that the rendering is projected on. The
// top left of the viewing rectangle is your system's origin point.
//
//****************************************************************************************


struct renderSetup {

	triDPoint		location;
	triDRotation	orientation;
	double				scale;
	triDPoint		camera;
	int				viewWidth;
	int				viewHeight;
};



//****************************************************************************************	
// viewFacet: (used internally)
//
// view Facet is the output of the triDEngine. viewFacets are generated as an ordered
// list from back to front. This is so that the calling program can start at 0 and just
// run down the list drawing ready formed triangles to the screen.
//
// viewFacets are only shapes with a triDvector to give their three D angle. The calling
// program has the task of deciding what color to "paint" these triangles.
//
// normalVect, used to compare with light sources to choose facet color.
//
// Corners A,B,C These are the three points of a 3D triangle mapped to a 2D image of a
// triangle ready to be drawn to the display. Meaning, that they are already in view
// coordinates, rounded to integers with 0,0 as the upper left hand corner of the view.
// Ready to go!
//
//****************************************************************************************

 
struct viewFacet {

	triDVector		normalVect;
	triDTriangle	original;
	point				cornerA;			// These three are int points, ready to draw.
	point				cornerB;
	point				cornerC;
};

viewFacet loadViewFacet(STLFacet* fileFacet);
void offset2DFacet(viewFacet* aFacet,int x,int y);
void printViewFacet(viewFacet* aFacet);

//****************************************************************************************
// objCenter: (used internally)
//
// Add all the facets of a 3D object into this and it calculates the center point of the
// object.
//
//****************************************************************************************


class objCenter {

	public:
				objCenter(void);
	virtual	~objCenter(void);
	
				void			addSTLFacet(STLFacet* aFacet);
				void			addItem(double x,double y,double z);
				triDPoint	getCenterPt(void);
				void			printObjCenter(void);
				
				bool	pointSet;
				double	xMin;
				double	xMax;
				double	yMin;
				double	yMax;
				double	zMin;
				double	zMax;	
};
	
	
//****************************************************************************************
// indexItem: (used internally)
//
// The user should not interact with this class directly. indexItem is used by triDEngine
// to track facets in the .stl file.
//
// Why is this?
//
// We can't keep the facet list in memory. It can be just too big! So, to help, the
// triDEngine class goes though the file and prunes out the facets the user can't possibly
// see. The remaining facets are stored only by their index in the .stl file. And, this
// list of indexes is sorted by their distance from the user's eye. An indexItem is one of
// the links of this list.
//****************************************************************************************


class indexItem : public linkListObj {

	public:
				indexItem(uint32_t index,double distance);
	virtual	~indexItem(void);
	
	uint32_t	getIndex(void);
	
				uint32_t	facetIndex;
				double		zDistance;
};



//****************************************************************************************
// triDEngine: (used internally)
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


class triDEngine :	public linkList,
							public stlFile {
	public:
				triDEngine(void);
	virtual	~triDEngine(void);
	
				bool			begin(const char* stlPath);
				bool			createList(renderSetup* setup);			// Build the list of visible facets.						
				void			resetList(void);								// Reset the viewFacet list to the back for a new read.
				viewFacet	getNextViewFacet(renderSetup* setup);	// Get the next facet off the visible list. (visible list index)
				
	//protected:
	
				viewFacet	calcViewFacet(renderSetup* setup,STLFacet* fileFacet);
				void			addIndexItem(indexItem* newItem);
				void			setScale(renderSetup* setup,STLFacet* aFacet);
				void			setRotation(renderSetup* setup,STLFacet* aFacet);
				void			setLocation(renderSetup* setup,STLFacet* fileFacet);
				void			doTransformations(renderSetup* setup,STLFacet* aFacet);
				double		inView(renderSetup* setup,STLFacet* aFacet);
	
				bool			init;				// We ready to go?
				bool			haveList;		// Is list current to orientation?
				indexItem*	currItem;		// Tracks the next facet handed back.
				mapper		ptMapper;		// mapper for doing? Mapping points to 2D.
};



//****************************************************************************************
// stlObj: (You can mess with this one.)
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
				void			setlightLoc(triDPoint* lightLoc);
				void			setlightColor(colorObj* color);
				void			setObjScale(double scale);
				void			setObjLoc(triDPoint* loc);
				void			setObjAngle(triDRotation* angle);
				void			setCamera(triDPoint* cam);
	virtual	void			drawSelf(void);
	
	protected:
				void			calcLightMapper(void);
				colorObj		calcColor(viewFacet* aFacet);
				
				bool				init;
				bool				setupChange;
				char*				savedPath;
				renderSetup		setup;
				colorObj			ambientColor;
				triDPoint		spotPos;
				colorObj 		spotColor;
				colorMultiMap	lightMapper;
				triDEngine		ourFacetList;
};


#endif