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
// Corners A,B,C These are the three points of a 3D triangle mapped to a 2D image of a
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
				void	setObjAngle(triDRotation* angle,triDPoint* centerPt=NULL);
				void	setCamera(triDPoint* cam);
	virtual	void	drawSelf(void);
	
	protected:
				bool			createList(void);										// Build the list of visible facets.						
				void			resetList(void);										// Reset the viewFacet list for a new read.
				viewFacet	getNextViewFacet(void);								// Get the next facet off the visible list. (visible list index)
				viewFacet	calcViewFacet(triDFacet* aFacet);
				void			addIndexItem(indexItem* newItem);
				void			doTransformations(triDFacet* aFacet);
				double		inView(triDFacet* aFacet);
				void			calcLightMapper(void);
				colorObj		calcColor(viewFacet* aFacet);
				
				bool				init;
				facetList*		ourModel;
				bool				haveList;		// Is list current to orientation?
				indexItem*		currItem;		// Tracks the next facet handed back.
				mapper			ptMapper;		// mapper for doing? Mapping points to 2D.
				bool				setupChange;
				triDPoint		location;
				triDRotation	orientation;
				triDPoint		rotationCenter;
				double			scale;
				triDPoint		camera;
				colorObj			ambientColor;
				triDPoint		spotPos;
				colorObj 		spotColor;
				colorMultiMap	lightMapper;
};

				
#endif