#ifndef triDRender_h
#define triDRender_h

#include <drawObj.h>
#include <facetList.h>



//****************************************************************************************	
// viewFacet: (used internally)
//
// view Facet is the output of the triDEngine. viewFacets are generated as an ordered
// list from back to front. This is so that the calling program can start at 0 and just
// run down the list drawing ready formed triangles to the screen.
//
// viewFacets are shapes with their calculated color.
//
// normalVect, used to compare with light sources to choose facet color.
//
// Corner[3] These are the three points of a 3D triangle mapped to a 2D image of a
// triangle ready to be drawn to the display. Meaning, that they are already in view
// coordinates, rounded to integers with 0,0 as the upper left hand corner of the view.
// Ready to go!
//
//****************************************************************************************

 
struct viewFacet {

	triDVector	normalVect;
	triDPoint	midPoint;		
	point			corner[3];
};

void offset2DFacet(viewFacet* aFacet,int x,int y);
void printViewFacet(viewFacet* aFacet);



//****************************************************************************************
// indexItem: (used internally)
//
// The user should not interact with this class directly. indexItem is used by triDEngine
// to track facets in the facet list.
//
// Why is this?
//
// We don't want to keep the facet list in memory. It can be just too big! So, to help,
// the triDRender class goes though the list and prunes out the facets the user can't
// possibly see. The remaining facets are stored only by their index in the list. And,
// this list of indexes is sorted by their distance from the user's eye. An indexItem
// is one of the links of this list.
//****************************************************************************************


class indexItem : public linkListObj {

	public:
				indexItem(long index,double distance);
	virtual	~indexItem(void);
	
				long	getIndex(void);
	
				long		facetIndex;
				double	zDistance;
};



//****************************************************************************************
// triDRender: (You can mess with this one.)
//
// This is the class the user uses to render a 3D object to be draw on the screen. It
// stores all the information needed to setup the drawing environment. Ambient light,
// directed (spot/flod) light. frame size and location on display.
//
// This also contains functions for the setup of the object itself for rendering. Location
// in the frame, orientation to the frame, and scale for drawing.
//****************************************************************************************


class triDRender :	public linkList,
							public drawObj {
					
	public:
				triDRender(int inX,int inY,int inWidth,int inHeight);
				triDRender(rect* inRect);
	virtual	~triDRender(void);
				
				
				bool	begin(facetList* inModel);
				void	setAmbiantlight(colorObj* inColor);
				void	setlightLoc(triDPoint* lightLoc);
				void	setlightColor(colorObj* color);
				void	setObjScale(double inScale);
				void	setObjLoc(triDPoint* loc);
				void	setObjAngle(triDRotation* angle);
				void	setCamera(triDPoint* cam);
				rect	getLastRect(void);
	virtual	void	drawSelf(void);
	
	protected:
				void			add2DPointToRect(viewFacet* aFacet,bool firstFacet);
				bool			createList(void);										// Build the list of visible facets.						
				void			resetList(void);										// Reset the viewFacet list for a new read.
				viewFacet	getNextViewFacet(void);								// Get the next facet off the visible list. (visible list index)
				viewFacet	calcViewFacet(triDFacet* aFacet);
				void			addIndexItem(indexItem* newItem);
				void			setRotationOffset(void);
				void			doTransformations(triDFacet* aFacet);
				double		inView(triDFacet* aFacet);
				void			calcLightMapper(void);
				colorObj		calcColor(viewFacet* aFacet);
				
				bool				init;				// We make it through our fire up checklist?
				facetList*		ourModel;		// The dumb thing we're supposed to draw.
				bool				haveList;		// Is list current to orientation?
				indexItem*		currItem;		// Tracks the next facet handed back.
				mapper			ptMapper;		// mapper for? Mapping 3D points to 2D.
				bool				setupChange;	// Something has changed. refresh the list.
				triDPoint		location;		// Inputted location offset.
				triDRotation	orientation;	// Inputted rotation.
				triDPoint		adjLocation;	// Calculated location offset.
				double			scale;			// Inputted scale factor.
				triDPoint		camera;			// Inputted viewpoint location.
				colorObj			ambientColor;	//	The color of "shade".
				triDPoint		spotPos;			// Single floodlight position.
				colorObj 		spotColor;		// Single floodlight color.
				colorMultiMap	lightMapper;	// Maps light stuff & angles to facet color.
				point				topLeft;			// Of our drawing. (For erasing)
				point				botRight;		// Of our drawing. (For erasing)
};

				
#endif